#ifndef _OFFSCREEN_H
#define _OFFSCREEN_H

#include <stdint.h>

// UNITS: rows, cols: character cells (8x8)
// lines: horizontal lines 1 pixel high

// scroll window dimensions on real screen
#define SCROLL_COLS	16
#define SCROLL_ROWS	8
#define SCROLL_LINES	( SCROLL_ROWS * 8 )

// scroll window position on real screen
#define SCROLL_POS_ROW	0
#define SCROLL_POS_COL	0

// extra rows at the top for drawing new tiles entering screen
#define SCROLL_EXTRA_ROWS	2
#define SCROLL_EXTRA_LINES	( SCROLL_EXTRA_ROWS * 8 )

// utility functions
void init_offscreen( void );
void offscreen_show_frame( void );
void offscreen_draw_tile( uint8_t row, uint8_t col, uint8_t *udg );

#endif // _OFFSCREEN_H
