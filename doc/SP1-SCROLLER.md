# ZX Spectrum scroller games using Z88DK SP1 library

## Ideas to test

- [x] Reusing SP1 tile pointers to point into offscreen buffer
- [x] Do a trivial scroller but updating all tiles and with real new tiles coming from the top
- [x] Previous test but add sprites moving on top of the scrolling background
- [x] Try to create a stack-based scrolling routine
- [ ] Prepare a super-simple IM2 handler which only increments a 32 bit value so that ROM int routine is not run. We can use this counter to better measure the execution time.
- [ ] Run with INTs disabled and sync with Vsync with floating bus trick
- [ ] Scroll down with attributes
- [ ] Parallax scrolling by scrolling the leftmost and rightmost columns at a different higher speed than the center ones. Allow for a different scroll speed per column. Modify scroll routine to receive 3 params: address, num of scroll pixels, height of the scroll window. Computed jump for the correct number od LDDs.

- With the previous ideas, we are using two exhaustive loops: a) when scrolling a column, we do it for all lines; and b) when invalidating, we do it for the whole scroll area. We can try to optimize each of them (or both) to not do them exhaustively, but selectively.

- Scrolling all columns takes less than a frame, while updating SP1 background takes 2 frames and some more. So it makes sense to first try to optimize invalidations (biggest gain will be probably here), and then scrolling (this gain will be lower).

Optimizations:

- [ ] Try to _not_ update all tiles, but only the ones that have some content: keep track of the columns that have content (=tiles) and only scroll/update those. Since the BG is scrolling, this will change in time, but we should get a real optimization (much less drawing, if the BG is simple)

- [ ] In line with the previous point, try to _not_ scroll all the vertical column, but just an address range. Since we have the information about which cells have tiles, we can selectively scroll them. We can select the number of LDDs to skip in the scroll routine with some self-modifying code: a computed jump `JP xxxx` where the value of `xxxx` is modified with the initial LDD position to jump to.

## Context

My previous proofs of concept for vertical scrollers on the ZX spectrum (see [DESIGN-NOTES](DESIGN-NOTES.md)) were based on double buffering, and the offscreen buffer had a linear memory layout. This layout very much simplifies the scrolling and screen transfer routines, and also the sprite routines (had they been written!). Linear memory layouts are simple and trivial to work with.

Having done the initial POC for the buffer, scrolling and transfer routines, and having the scroller already working on the screen (see tests in `src/technique-{1,2}` directories), I found myself having to write the sprite routines if I wanted to do any games. Sprite routines would have been simple to write for the linear buffer, but the point was that I already had extensive experience using the SP1 library (a great sprite library for the Spectrum written by Alvin Albrecht), and even fixing and modifying it; SP1 already has the capabilities for handling the most important layers in a game: the background layer and the sprite layer. I felt it could be really good if SP1 could be used for handling those layers in my Speccy scroller games.

Then I remembered that when specifying the background tiles in SP1 for each character position, you use a 16-bit value that can specify 2 tile types:

- If the value is 0-255 (high byte is 0), then the low byte is used as an index on a global tile table (8 bytes per tile), which needs to be predefined elsewhere, normally during game loading or initialization.

- If the value is greater than 255 (high byte != 0), then the full 16-bit value is used as a pointer to the raw 8 bytes that should go into that screen position.

I use the second mode in my RAGE1 game library as a way to escape the 255 tile limit, and I am quite familiar with it, so I realized that this method could be used in the following way:

- The offscreen buffer is divided in 8-byte character cells (8x8 pixels) than can be used as SP1 tiles. That is, instead of a linear memory buffer by horizontal lines, graphic data is stored as an array of (M rows x N cols) character tiles.

- The SP1 background tile map is setup so that for each screen character cell, its tile pointer is set to the corresponding cell in the offscreen buffer.

- Scrolling is done on the offscreen buffer, and then the scrolling area is fully SP1-invalidated on each frame (`sp1_Invalidate()` function), so that all of it is redrawn when `sp1_UpdateNow()` function is called.

- Since the sprites can also be managed by SP1, the net result is that we can have a scrolling background with our SP1 sprites moving over it.

- Finally, as an optimization, the cell layout in the offscreen buffer is done by columns (instead of by rows): in (row,col) cell coords we have (0,0), then (1,0), then (2,0), and after the first column ends, (0,1), then (1,1), then (2,1), etc. With this storage schema, vertical scrolling on a given column of character cells can be done with just 8 x M LDI/LDD instructions (unrolled), or a single LDIR/LDDR (loop).

I have made a simple POC (see `src/sp1-baseline`directory) which configures all SP1 tile pointers in the scrolling area to a single tile which I trivially manipulate on each frame to get a feel of the scrolling movement; just to verify if SP1 is quick enough to do this.

I found that of course it can't do 50 fps (I expected that), but so far I have found it quite capable, so I'll do more experiments by tweaking e.g. the scrolling area, the scrolling speed, number of sprites, etc.

## SP1 scrolling test 1

It can be found in the `src/sp1-randomtiles` directory. This is a real scroller, using the workflow that would be used for a real game: tiles are drawn on some non-visible top rows and are brought into view by the scrolling process. In this example, the logic for printing tiles on the top is trivial, but in a real game those would come from a map.

Keep in mind that the top row of invisible tiles is only drawn once in a while (when a full tile row has been scrolled down) and not on every scroll cycle.

The scrolling routine has been unrolled (multiple LDDs instead of one LDDR) for better speed.

The number of pixels to be scrolled down on each scroll-cycle can be configured at the top of the demo. Less pixels = slower, but smoother. More pixels = faster but clunkier. 1-pixel scrolling is smooth but it could be apropriate (or not) for the game.

The initial numbers for this scroller are around 12.5 FPS, so we are spending 3 frames and some more just for the graphics handling. We could aim for a 10 FPS target, having some 1 frame and a bit more for our game calculations, sprite movements, checks, etc.

Also worth considering: SP1 graphics algorithm is designed so as to not have any flickering, so we can try to run at full tilt avoiding the HALT and not waiting for Vsync. This would make the game run slower or faster depending on the number of elements on screen, which may be not desired, but it's worth a try when we have sprites moving on the screen (on next tests).

## SP1 scrolling test 2

It can be found in the `src/sp1-sprites` directory. It's the previous Test 1 real scroller, but this time with sprites running all over the place, while the background is scrolling down.

The source is fully parameterized, so that different configurations can be tested and conclusions drawn.

The following parameters can be modified by just changing the #define's at the top of the C file:

- Size and position of the scrolling area (dimensions in char cells)

- Number of pixels to scroll down on a single scroll cycle (pixels)

- Size of the top non-visible tile row - the one that brings new tiles from the game map (char cells). This is, practically, the size of the biggest tile in the map.

- Number of sprites (a maximum of 16 have been defined, but only the number #define'd will be used for the demo)

My findings so far:

- SP1 seems _very_ capable of running a game with a scrolling background and several sprites moving on the screen

- The number of simultaneous sprites affects speed, but not as much as I expected. A possible explanation may be that since the whole scroll area is invalidated (currently) the drawing of all sprites does not invalidate more cells when more of them are on screen, so just their calculations and positioning matter.

- I have the definive impression that not using the HALT to wait for Vsync makes the demo go smoother (opinions?)

My next optimizations will be oriented to find the way of _not_ invalidating all the screen, but only the affected cells. This will probably force me to keep track of the current tiles on screen and their position.

P.S. Assembler functions and loops have been modified to not require any manual adjustment - just change the #define; and also: I have switched to SDCC compiler for better C syntax support :-)

## SP1 scrolling test 3

It can be found in the `src/sp1-partial-inv-1` directory. Based on Test 2, but this one keeps track of the tiles that are present on each column, and only invalidates the cells that are affected by them. It also keeps track of how the tiles walk down the screen, and propery moves the cells to invalidate.

The information on drawn tiles is known when printing tiles on the top non-visible row.

The basic idea to explore is column cell-range invalidation:

- Instead of invalidating the whole scroll area with a single `sp1_Invalidate()` call, we will run an invalidation per column.

- On each column, only cells that have moving content will be invalidated, taking into account that e.g. a 2-row tile will most of the time invalidate a 3-row area due to vertical pixel offset.

- Initially, only one range per column will be invalidated (for efficiency reasons). That is, if we have tiles in rows 4-5 and 10-11, the whole 4-11 range will be invalidated.

- For each column, there is a (min,max) pair which holds the current range of cell invalidations to be done for that column. This pairs are moved down for all columns every 8 scrolled pixels (cell changes occur every 8 scrolled lines). So if scroll is 1-pixel, it will be done every 8 scroll cycles; if scroll is 2-pixel, every 4 scroll cycles; etc.

- Since these `min` and `max` values will be cell-based, and in ranges 0-31/0-23, they will be of type `int8_t` (signed) and we will use `min` = -128 (`NO_RANGE`) to signal that there is no invalidation to be done on that column.

- Initially, each column starts with a (NO_RANGE,NO_RANGE) pair (no invalidation)

- When printing a tile on the top non-visible row:
  - Set MIN to -(height of top non-visible row)
  - If MAX is NO_RANGE, set MAX to 0

- Every 8 scrolled pixels, do the following for each of the columns:
  - Adjust MIN and MAX (add 1 to each of them, saturating at the maximum row value)
  - If MIN is out of the scroll area, set both MIN and MAX to NO_RANGE

- The previous adjustments and movements will have to take into account 1 additional cell due to the pixel offsets mentioned above.

We could keep track of all tiles on each columns and only invalidate the strictly necesary, but this would imply having a changing list for each column, which means more processing on a critical code path. If we have several tiles on the same column, I have assumed that it's probably cheaper to simply invalidate everything in-between than trying to cherry-pick cells to invalidate.

An enhanced version of this schema will be tested in Test 4, though.

My findings with this test:

- The idea of only invalidating the needed areas is good and adds speed to the test

- The current algorithm is not so good: given that on any given column, only a single range of rows can be tracked and invalidated, it quickly degenerates into the full-column invalidation case. E.g.: when the map has few and vertically sparse tiles, the algorithm works well (it can be appreciated at the beginning of the demo, when only a few background tiles can be seen on the screen); but when more tiles start to appear, suddenly all the columns have several tiles and almost the full column is invalidated, defeating the optimization.

- A better algorithm which really keeps track of all the on-screen tiles and their invalidating rectangles will be explored in Test 4.

## New stack-based scrolling routine

Also in Test 3, a new stack-based scrolling function has been developed and tested.

I have found that the regular unrolled LDD version is hard to beat, mainly due to the fact that when moving the bytes, the source and destination ranges mostly overlap, and this makes it more difficult to optimize the stack transfer.

The approximate T-state count for each of the versions is commented in the code.

## SP1 scrolling test 4 (WIP)

It can be found in the `src/sp1-partial-inv-2` directory. Based on Test 3, but changing the algorithm which keeps track of the cells to invalidate. Conceptually, this method keeps track of the position of each and every tile which is seen on screen, and periodically adjusts the cells that it invalidates in lockstep with the scrolling routine.

Basic ideas:

- We keep a global list of printed tiles. This list can be a ring buffer for efficiency, since all objects in that list will added and removed in FIFO fashion

- When printing a tile on the top row, it is also added to this list, together with its invalidating rectangle

- The list is updated every 8 scrolled pixels to move down the invalidating rectangles of all live tiles

- When tiles reach the bottom row and go out of sight, they are removed from the global list

- The maximum size for the global list is fixed and depends strictly of the scroll area and the tile sizes, so no heap is required. It can be declared statically with maximum size.

- The invalidation routine can run pretty fast: it only has to go over the global list of printed tiles and unconditionally invalidate their rectangles.
