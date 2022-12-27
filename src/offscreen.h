#ifndef _OFFSCREEN_H
#define _OFFSCREEN_H

#include <stdint.h>

// scroll window dimensions on real screen
#define SCROLL_COLS	24
#define SCROLL_ROWS	24
#define SCROLL_LINES	( SCROLL_ROWS * 8 )

// scroll window position on real screen
#define SCROLL_POS_ROW	0
#define SCROLL_POS_COL	0

// extra rows at the top for drawing new tiles entering screen
#define SCROLL_EXTRA_ROWS	2
#define SCROLL_EXTRA_LINES	( SCROLL_EXTRA_ROWS * 8 )

// utility functions
void init_offscreen( void );

#endif // _OFFSCREEN_H
