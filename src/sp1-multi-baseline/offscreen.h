#ifndef _OFFSCREEN_H
#define _OFFSCREEN_H

#include <stdint.h>

// data
extern uint8_t offscreen[];

// functions

// Returns the pointer into the extended offscreen for a given row and column
// These are extended coords, i.e. [ 0 .. (SCROLL_AREA_EXTENDED_WIDTH - 1) ], etc.
uint8_t *offscreen_cell_address( uint8_t row, uint8_t col );

// scrolling functions
void offscreen_scroll_right_1px( void );
void offscreen_scroll_right_2px( void );
void offscreen_scroll_right_4px( void );

#endif // _OFFSCREEN_H
