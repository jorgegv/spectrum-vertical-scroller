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
