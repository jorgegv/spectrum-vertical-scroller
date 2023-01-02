# Tuning the routines

------------------------------
**WARNING:** These tuning instructions are only valid for the C and LDI/LDIR
versions of the scrolling routine.

The stack version is a proof of concept and is specifically designed for a
16-column wide scrolling area.  It is well commented, though, and should be
easy to adapt for other widths.
------------------------------

Changing the dimensions and position of the scrolling area:

- Edit `offscreen.h`, change the `SCROLL_COLS`, `SCROLL_ROWS`, and
  `SCROLL_POS_*` constants

- In the `asm_offscreen_ldi.asm` file, make sure the number of LDIs is
  `SCROLL_COLS`, and the line counter is set to `SCROLL_ROWS * 8`

- In the `asm_offscreen_ldir.asm` file, make sure the LDIR counter in BC is
  `SCROLL_COLS`, and the line counter is set to `SCROLL_ROWS * 8`

## Tuning results

All tests done using a 16x16 cell scrolling area (128 lines, each 16 bytes wide)

- When using `offscreen_show_frame`, the system cannot keep up and spends
  more than one display cycle to redraw the scrolling frame, working at
  25fps

- When using `asm_offscreen_show_frame_ldir`, the system can work at 50fps,
  but it uses most of the CPU time to redraw the scrolling frame

- When using `asm_offscreen_show_frame_ldi`, the system can work at 50fps,
  and there is spare CPU time to do other things

- Routine `asm_offscreen_show_frame_stack` gives the better results, but it
  is currently designed for a (fixed) 16 column scrolling area.  See source
  code for adapting to other widths.
