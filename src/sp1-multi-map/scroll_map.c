#include "build.h"

// some tile definitions for the map

// blank tile
uint8_t blank_tile_2x2_data[ 32 ] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
struct tile_s blank_tile_2x2 = { 2, 2, blank_tile_2x2_data };

// diamond tile
struct tile_s diamond = { 2, 2, diamond_tile };

struct tile_s *tile_bank[ 1 ] = {
    &diamond,
};

// the scroll map data - must be SCROLL_MAP_WIDTH * SCROLL_MAP_HEIGHT chars
// a space represents the blank tile above
// a letter A-Z is translated to tile[ code - 65 ] (A=0, B=1, C=2, etc.)
uint8_t *scrollmap_data =
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
    "                                                "
;

// main scroll map definition
struct scroll_map_s {
    uint8_t width;
    uint8_t height;
    uint8_t *data;
    struct {
        uint8_t row;
        uint8_t col;
    } viewport_pos;
};

struct scroll_map_s scroll_map = {
    SCROLL_MAP_WIDTH,
    SCROLL_MAP_HEIGHT,
    &scrollmap_data[0],
    { SCROLL_MAP_VIEWPORT_INITIAL_ROW, SCROLL_MAP_VIEWPORT_INITIAL_COL },		// viewport position
};

void init_scroll_map( void ) {
}

void scroll_map_set_viewport( uint8_t row, uint8_t col ) {
    scroll_map.viewport_pos.row = row;
    scroll_map.viewport_pos.col = col;
}

struct tile_s *scroll_map_get_tile_at( uint8_t row, uint8_t col ) {
    uint8_t tile_code = scroll_map.data[ row * SCROLL_MAP_WIDTH + col ];
    switch ( tile_code ) {
        case ' ':
            return &blank_tile_2x2;
            break;
        default:
            return tile_bank[ tile_code - 'A' ];
            break;
    }
}
