# Tuning the routines

Changing the dimensions and position of the scrolling area:

- Edit `offscreen.h`, change the `SCROLL_COLS`, `SCROLL_ROWS`, and
  `SCROLL_POS_*` constants

- In the `asm_offscreen_ldi.asm` file, make sure the number of LDIs is
  `SCROLL_COLS`, and the line counter is set to `SCROLL_ROWS * 8`

- In the `asm_offscreen_ldir.asm` file, make sure the LDIR counter in BC is
  `SCROLL_COLS`, and the line counter is set to `SCROLL_ROWS * 8`

# Tuning results

All tests done using a 16x16 cell scrolling area (128 lines, each 16 bytes wide)

- When using `offscreen_show_frame`, the system cannot keep up and spends
  more than one display cycle to redraw the scrolling frame, working at
  25fps

- When using `asm_offscreen_show_frame_ldir`, the system can work at 50fps,
  but it uses most of the CPU time to redraw the scrolling frame

- When using `asm_offscreen_show_frame_ldi`, the system can work at 50fps,
  and there is spare CPU time to do other things

- Implementation of `asm_offscreen_show_frame_stack` still pending for
  better results

