#ifndef _OFFSCREEN_H
#define _OFFSCREEN_H

#include <stdint.h>

// data
extern uint8_t offscreen[];

// functions

// Returns the pointer into the extended offscreen for a given row and column
// These are extended coords, i.e. [ 0 .. (SCROLL_AREA_EXTENDED_WIDTH - 1) ], etc.
uint8_t *offscreen_cell_address( uint8_t row, uint8_t col );

// scrolling driver functions
// number of pixels to scroll in each direction is passed as a parameter
// the driver functions select the fastest scroll routine according to num_pix
void offscreen_scroll_right_pixels( uint16_t num_pix );
void offscreen_scroll_left_pixels( uint16_t num_pix );
void offscreen_scroll_up_pixels( uint16_t num_pix );
void offscreen_scroll_down_pixels( uint16_t num_pix );

void offscreen_scroll_left_Npx_tile_row( void *start, uint16_t num_pix ) __smallc __z88dk_callee;
void offscreen_scroll_up_Npx_tile_col( void *start, uint16_t num_pix ) __smallc __z88dk_callee;

#endif // _OFFSCREEN_H
