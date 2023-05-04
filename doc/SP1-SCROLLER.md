# ZX Spectrum scroller games using Z88DK SP1 library

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

It can be found in the `src/sp1-randomtiles` directory. This is a real scroller, using the real workflow that would be used for a real game: tiles are drawn on some non-visible top rows and are brought into view by the scrolling process. In this example, the logic for printing tiles on the top is trivial, but in a real game those would come from a map.

Keep in mind that the top row of invisible tiles is only drawn once in a while (when a full tile row has been scrolled down) and not on every scroll cycle.

The scrolling routine has been unrolled (multiple LDDs instead of one LDDR) for better speed.

The number of pixels to be scrolled down on each scroll-cycle can be configured at the top of the demo. Less pixels = slower, but smoother. More pixels = faster but clunkier. 1-pixel scrolling is smooth but it could be apropriate (or not) for the game.

The initial numbers for this scroller are around 12.5 FPS, so we are spending 3 frames and some more just for the graphics handling. We could aim for a 10 FPS target, having some 1 frame and something for our game calculations, sprite movements, checks, etc.

Also worth considering: SP1 graphics algorithm is designed so as to not have any flickering, so we can try to run at full tilt avoiding the HALT and waiting for Vsync. This would make the game run slower or faster depending on the number of elements on screen, which may be not desired, but it's worth a try when we have sprites moving on the screen (on next tests).
