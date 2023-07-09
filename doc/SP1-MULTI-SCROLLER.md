# SP1 MULTIDIRECCIONAL SCROLLING

This document explains the different experiments with doing multidirectional scrolling wth the SP1 sprite and background management library.

It is based on my previous vertical scrolling experiments which can also be found in this very same repository.

Ideas to test (and write about):

- [ ] Create basic scroll routines, establish framerate baselines for all 8 directions (UDLR and the diagonals)
- [ ] Create scroll mapping routine: big map, viewport, positioning and scrolling
- [ ] Try assembler and C optimizations first, compare with baselines:
    - [x] Reduce instruction count per loop
    - [x] Unroll inner loops
    - [x] Replace PUSH/POP sequences by storing in additional regs when possible
    - [x] Replace local (stack) vars with static local ones
- [ ] Optimize SP1 updates
    - [ ] Apply Partial-2 technique to multiscroll (quad-linked list of tiles, etc.)
- [ ] Attribute scrolling (useful for 8-pixel scrolling)

## Basic multiscroll routines and framerate baselines

It's a fact that we can't optimize what we can't measure, so my first step at multiscrolling is to do some Proof of Concept routines and establish a code harness where I can run some accounting in the background, and run any routine I want while measuring its performance. Of course this harness is also a testbed to debug the scrolling routines during its development.

As in my previous series on SP1 vertical scrolling examples, my performance measure is the Frame Per Second (FPS).

Also needed for measuring real performance is to have some way of running a "walk" over a given map by using scrolling, so I have also developed a scroll map system that decides what what to draw in the hidden borders to get the effect of a scrolling _viewport_ over a bigger map. But this mapping system wil be described in detail in a future post.

Back to multidirectional scroll, I need to write specific routines for doing it in all 4 directions (and not just one scroll-down routine like for the vertical scroller series). This is a challenge, because although vertical scrolling is relatively simple, horizontal scrolling needs to be done bit by bit and even though there are specialized Z80 instructions for doing that, I found some obstacles in my way.

The other most important thing is the memory layout of the virtual framebuffer. In this regard, we are completely conditioned by the SP1 tile layout, since for SP1 to update the screen correctly, it expects the 8 bytes associated to a screen cell position, located contiguously in memory. This restriction heavily favours a vertical memory layout by columns, like the one we used with the vertical scrolling series.

One important problem that we'll see is the wildly different performance between vertical and horizontal scrolling. We will need to sort this out and get both performances approximately on par, so that we can have a screen that moves at similar speed in all directions (i.e. an acceptable gameplay).

The speed difference stems from the different ways the scroll is done:

- For vertical scrolling, we have a specialized instruction: LDI/LDD. Since the column is laid out vertically in memory, it's just a matter of setting a source register (HL) and a destination one (DE) with an offset equal to the number of pixels we want the column to scroll, and just issue a number of LDI/LDD instructions equal to the number of lines of the scroll area. Incrementing/decrementing the source and destination pointers is taken care of by the very LDI/LDD instruction, so we get a really fast and compact transfer loop.

- For horizontal scrolling, we also have a specialized instruction sequence: RR (HL) + INC HL, which shifts one bit in a memory position through the Carry flag to the next memory position. This roughly does the equivalent of the LDI/LDD instructions for vertical scroll. We have a problem with this sequence, though: it assumes that the memory is laid out horizontally (and not vertically), i.e. the next byte in memory should be the next one horizontally. But our screen is laid out vertically due to SP1 requirements, so instead of INC HL, we need to add a fixed offset to HL it to access the next column. This addition unfortunately destroys the Carry flag previously modified by RR (HL), so we also need to save it for later, and our inner loop then becomes more complicated: RR (HL) + PUSH AF + LD HL,DE + POP AF.

In both cases, one scroll run of the whole framebuffer is roughly proportional to the dimensions of the scrolling area, WIDTH x LINES (WIDTH measured in cells and LINES measured in pixels). But while the inner loop for vertical scroll is 16 T-states (LDI/LDD), for horizontal scroll it is 15+11+8+10=44 T-states (nearly 3 times bigger!). This is the main reason for the very different performance in each direction.

In the case of 4-pixel scrolling we have also a specialized instruction sequence: RRD (HL), which rotates 4 bits through the Accumulator. This instruction does not need the Carry flag, so we can do away with the PUSH/POP in the internal loop, and so RRD (HL) + ADD HL,DE only takes 18+8=24 T-states, which is quite near the 16 T-states for vertical scroll.

In the case of 8-pixel scroll, both horizontal and vertical inner loops can be expressed as LDI/LDD, and so the performance for both directions is identical. But 8-pixel scrolling might not be adequate for all games, though.

This currently makes 4-pixel scroll our best bet for multiscroll games... but we'll see if we can further optimize the other routines. 4-pixel scroll can be OK for some games, but smooth scrolling is... well, The Right Thing, so let's see.

These are the current FPS measurements in all scrolling directions, for a 16x16-cell scrolling viewport and for different pixel increments:

| Pixels \ Direction | U  | D  | L  | R  | UL | UR | DL | DR |
|--------------------|----|----|----|----|----|----|----|----|
| 1-pixel            | 16 | 16 | 12 | 12 | 10 | 10 | 10 | 10 |
| 2-pixel            | 16 | 16 | 7  | 7  | 6  | 6  | 6  | 6  |
| 4-pixel            | 16 | 16 | 12 | 12 | 11 | 11 | 11 | 11 |
| 8-pixel            | 15 | 15 | 15 | 15 | 11 | 11 | 11 | 11 |

The additional drop in performance for diagonal movements is due to scrolling in both directions on the same move, e.g. scroll in UP-LEFT direction = scroll UP + scroll LEFT in sequence.

The code used for this measurements and performance baselines is in directory `src/sp1-multi-map`.

The animated demos and TAP files are ones provided in my previous post.

## Scroll map management

- A scrolling system is a viewport over a bigger map that can't fit in the view, so the need appears for scrolling over it, i.e. moving a window or "viewport" over it in small steps.
- The visible zone of the map is determined at all times by the position of the viewport over the global map
- Scrolling can be done at 1-pixel resolution, so the viewport coordinates must be held in pixels, not in tiles or character cells
- 8-bit pixel coordinates are enough for just 1 full screen, so we must use 16-bit for each viewport coordinate (X and Y)
- So maximum size of the global map is 65536x65536 pixels. That's 8192 character cells in each dimension, so it seems pretty enough for a Spectrum game :-)
- With this schema, 3 coordinate systems need to be taken into account:
    - The viewport coordinates: high resolution coordinate system with 16-bit per coordinate
    - The map coordinates: measures the position of tiles in the map, also 16-bit per coordinate
    - The SP1 coordinates: these are the screen-cell coordinates, our familiar 32x24 cell array. 8-bit per coordinate

- The global map is built with tiles of a regular size (can be 8x8, 16x16, 16x24, etc.). All tiles must be the same size.
- Horizontal and vertical tile dimensions do not need to be the same, but it helps with map drawing. In our examples, our map is made of 16x16-pixel tiles (2x2 chars)
- The map is stored as a linear byte array of size MAP_WIDTH x MAP_HEIGHT (dimensions in tiles).
- Each map position in the array stores a 1-byte tile id and represents the tile at map position (ROW,COL). So for example, if the map size is 20 rows x 30 columns, the tile id for map position (4,5) is stored at position (4 x 30 + 5) = 125.
- There is a global tile table which maps the tile id to the tile data, so that it can be easily used by a tile drawing routine.

- The virtual framebuffer (the offscreen) is not the same size of the viewport, but instead it has a hidden surrounding band which is 1-tile wide all over the perimeter of the viewport. This is where new tiles are drawn (to memory) before coming into view by the scrolling process. It's the same concept that was seen in my previous SP1 vertical scroller examples for the "hidden top row", but applied to all directions (top, left, bottom, right).
- The scrolling routines for each direction detect when new graphics are needed in the hidden band (because they are going to bring it into view with the next scroll operation), and will call the proper map functions to get tile data and draw it to the proper position in the hidden band.
- Example: if we are going to show a 16x24-cell scrolling viewport, with 2x2-cell tiles, the offscreen will be 20x28 cells (16+2+2, 20+2+2).



## Assembler optimizations for scroll routines

- Reduce instruction count per loop
- Unroll inner loops
- Replace PUSH/POP sequences by storing in additional regs when possible
- Replace local vars (stack) with static local ones

## Partial-2 optimization

## Attribute scrolling

