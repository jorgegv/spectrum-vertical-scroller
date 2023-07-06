#ifndef _SCROLL_MAP_H
#define _SCROLL_MAP_H

#include <stdint.h>

// scroll directions
#define DIR_UP		0x01
#define DIR_DOWN	0x02
#define DIR_LEFT	0x04
#define DIR_RIGHT	0x08

// main scroll map definition
struct scroll_map_s {
    uint8_t width;
    uint8_t height;
    uint8_t *data;
    struct {
        uint16_t x;
        uint16_t y;
    } viewport_pos;
};
extern struct scroll_map_s scroll_map;

// a global scroll_map object exists, but it should be manipulated only via
// these functions
void init_scroll_map( void );

// sets the viewport coordinates
// Map coordinates are specified in pixels
void scroll_map_set_viewport_xy( uint16_t x, uint16_t y );

// full redraw of the viewport, only needed at startup, or if the hero
// teleports to another part of the map :-)
void scroll_map_draw_viewport( void );

void scroll_map_scroll_viewport( uint8_t dir, uint8_t num_pix );

// utility functions to draw the hidden areas just before entering into view
// these functions should be called in sequence: draw-hidden-xxxx -> scroll-offscreen -> set-new-viewport-pos
void scroll_map_draw_hidden_top_row( void );
void scroll_map_draw_hidden_bottom_row( void );
void scroll_map_draw_hidden_left_col( void );
void scroll_map_draw_hidden_right_col( void );

// gets the tile addr for the given map coordinate
struct tile_s *scroll_map_get_tile_at( uint8_t row, uint8_t col );

#endif // _SCROLL_MAP_H
