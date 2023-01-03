# VERTICAL SCROLL GAME - DESIGN NOTES

## General design for scrolling screen

- Scrolling is not done full screen, but on a zone restricted to the left/right of the screen, and with a fixed width: WxH cells (e.g. 24x24, or 192x192 pixels)

- Initially, no attributes in scrolling area (monochrome), we will add them later

- Sprites and background tiles are 16x16

## Tiles

- Tiles are defined as 16x16 UDGs

## Sprites

- Sprite functions, there should be two sets:
  - Set 1: saves position and background content, draws the sprite with a mask.
  - Set 2: restores background at old position, saves new position and background content amd draws the sprite with a mask
  - Probably the Set 2 functions can end up calling the functions from Set 1
  - Explore preshifted sprites technique for out sprite drawing routines

- Sprites should de defined with 1 column more than ots size, to account for shifting right
- Example calculation of memory used for a 16x16 sprite:
  - Preshifted every 2 pixels (4 possible positions)
  - 16 lines x 3 bytes = 48 bytes for pixels
  - 16 lines x 3 bytes = 48 bytes for mask
  - 4 preshifted positions: ( 48 + 48 ) x 4 = 384 bytes
  - 16 lines x 3 bytes = 48 bytes for saved background
  - Sprite total: 384 + 48 = 432 bytes per sprite
  - Additional coordinates data not included (2 bytes for current position, 2 bytes for saved background position)

## Screen drawing

- Double-buffering to draw the screen
- An offscreen buffer is used for drawing the frame
- Size of the offscreen buffer: 24 x 192 = 4608 bytes
- The offscreen buffer is arranged in linear form, so that sprite and tile drawing routines are much simpler (i.e. to move from a line to the next, just add W bytes)
- The screen buffer is not exactly the size of the scroll window, but has an additional line of  tiles at the top (16 pixels, or the height of the tiles), so that tiles can be drawn there before "entering" the scrolling screen. Visible screen is 24 rows x 24cols, buffer is 26 rows x 24 cols.
- On each game cycle, the buffer is transferred to the screen with a high-speed transfer routine. This routine takes into account that the source is a linear buffer and the destination has the ZX screen layout

## Scrolling

For implementing the scroll, a windowing technique is used:

**Basic Technique 1:**

- The line where the transfer routine starts is moved up one pixel each time the buffer is transferred to the screen (offset), so that the next transfer starts one line earlier (and thus simulates the scroll effect).
- When the window has moved up 16 pixels (the extra row height), the line offset is reset to 0 and the whole buffer is redrawn, but this time with the tiles having moved 1 tile row up. This should make for a smooth scrolling effect.
- It is better to do a massive 16-line scroll down of the whole offscreen buffer (just one big LDIR) and just draw the tiles in the top row (16 pixels), and then erase the top 16 lines (a small LDIR with data value 0) for a clean drawing area.
- The above schema redraws the full screen once every 16 game cycles (or once every N lines, being N the number of extra lines in the offscreen buffer). This means ~3 times per second @50fps, or ~1.5 times per second @25fps.

**Enhanced Technique 2:**

Based on the previous one, but it would be great to get rid of the "redraw" or "massive scroll down" parts every 16 frames. We can do the following:

- The starting line for the frame transfer goes up one line on every transferred frame (as in the previous method)
- When the line has moved 16 lines, instead of scrolling down the whole virtual screen, we just keep moving the starting line up, but with rollover:
  - If we are at line 0 in the virtual screen, the next one should be the _last_ screen line (e.g. 192)
  - Instead of drawing new tiles always at the same position (top row, i.e. 0,0), we switch the tile drawing position to the 16 lines _before_ the current starting line.
  - Since this calculation is done every 16 frames, it is not so important for it to be extremely efficient.
- The main frame drawing routine (the one that transfers the virtual screen to the real one) needs to be a bit smarter: it will have to start drawing from an arbitrary line in the virtual screen (the starting line) downwards, and when the end of the virtual screen is reached, continue drawing from the first line (0).

Doing it this way, there is no need to to the "big scroll down" of the virtual screen, we just need to clear out the tile drawing line: a small 16-line LDIR to 0, and it can even be optimized further with stack-transfer techniques.

## Game refresh cycle

- One game cycle can be 2 frames (scroll @25fps) or 1 (scroll @50fps) (we will need to test how much can be done in a single frame)
- Tiles and sprites are erased/moved/redrawn on the buffer as needed on each game cycle
- Implement an IM2 ISR which just increments a frame counter and returns (for the moment - later we will add 128K music if needed)
- On each frame (vertical refresh @25fps):
  - Use HALT for sync
  - If frame counter is even, ignore and HALT again. If it is odd, continue
  - Transfer offscreen buffer to visible screen. This needs to be done immediately after the int has been generated (i.e. HALT completes)
  - Do game loop tasks (all drawing operations here are done on the virtual screen):
    - Erase all sprites from their current positions, restoring previous background
    - Check keyboard controller
    - Move hero sprite from old position to new
    - Manage fire shots (create new bullets/move bullets/disable offscreen bullets)
    - Move enemies
    - Check collisions (enemy-bullet, hero-enemies), process them
    - Check other conditions
    - Prepare next frame for scroll (technique 1):
      - Move starting scroll line 1 up
      - If we are at the top (== 16):
        - Reset starting scroll line to 0
        - Move tile coordinates in map 1 tile row up
        - Redraw all screen tiles
    - Redraw all sprites. Sprite drawing routines into offscreen buffer must take into account the current scroll scanline offset.

## Screen transfer routine

- Needs to be highly optimized
- Possibly fully in assembler
- Possible versions:
  - Version 1: one LDIR for each line (simplest)
  - Version 2: unrolled 24 LDIs per line (first optimization)
  - Version 3: stack transfer using POP/PUSH pairs, plus DI/EI (fastest)
- The source address (offscreen buffer) can be easily adjusted for each line: `src += 24`, since the layout is linear
- The destination address (on screen) can be precalculated for each line at program startup and stored in an array, and we can get the dst address by indexing at that array when needed

Additional possible optimizations:

- Ensure alignment of offscreen buffer to 256 so that calculations and
  checks can be done only on high byte 8-bit registers

- Convert `offscreen_scroll_down` function to asm with optimized stack
  transfer (same as frame transfer function)
