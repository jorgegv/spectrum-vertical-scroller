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
uint8_t scrollmap_data[] =
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "AA A A A  A   A  A       A A A A  A    A A     A"
    "A A A A   A   A  A  AAAA   A A A  A    A       A"
    "A  A A A  A   A    A    A    A A  A    A   A   A"
    "A   A A   A        A    A      A  A    A     A A"
    "A    A A  A        A    A        A    A  A     A"
    "A     A    A       A    A        A    A        A"
    "A AAAAA     A       AAAA        A     A   A    A"
    "A      AA    A                 A     A         A"
    "A        A    AA             AA      A  A    A A"
    "A  A   A  A     AAAA     AAAA       A          A"
    "A    A     A        AAAAA         AA     A     A"
    "A    A      AA                 AAA         A   A"
    "A             AAA          AAAA     A  A     A A"
    "A  AAAAA         AAAAAAAAAA                    A"
    "A     AAAAA                           AAA      A"
    "A    AAAAAAA     AAA       AAA       A   A AAA A"
    "A   AA  A  AA   A   A     A   A     A     A    A"
    "A   AA AA AAA        A   A     A   A         AAA"
    "A   AAAAAAAAA         AAA       AAA       AAA  A"
    "A   AAAAAAAAA                         AAAA     A"
    "A   A A A A A   AAAAA       AAAAAAAAAA        AA"
    "A                    AAAAAAA                   A"
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
;

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

struct scroll_map_s scroll_map = {
    SCROLL_MAP_WIDTH,
    SCROLL_MAP_HEIGHT,
    &scrollmap_data[0],
    { SCROLL_MAP_VIEWPORT_INITIAL_X, SCROLL_MAP_VIEWPORT_INITIAL_Y },		// viewport position
};

void init_scroll_map( void ) {
}

void scroll_map_set_viewport_xy( uint16_t x, uint16_t y ) {
    scroll_map.viewport_pos.x = x;
    scroll_map.viewport_pos.y = y;
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

void scroll_map_draw_hidden_top_row( void ) {
    uint8_t i;
    // scroll map coordinates
    uint16_t row = scroll_map.viewport_pos.y / SCROLL_MAP_TILE_HEIGHT_PIX - 1;
    uint16_t col = scroll_map.viewport_pos.x / SCROLL_MAP_TILE_WIDTH_PIX - 1;
    for ( i = 0; i < SCROLL_AREA_EXTENDED_WIDTH / SCROLL_MAP_TILE_WIDTH; i++ )
        tile_draw_offscreen(	// offscreen cell coordinates
            0,						// row = 0
            SCROLL_MAP_TILE_WIDTH * i,			// col = loop
            scroll_map_get_tile_at( row, col + i )	// use proper tile from map
        );
}

void scroll_map_draw_hidden_bottom_row( void ) {
    uint8_t i;
    // scroll map coordinates
    uint16_t row = scroll_map.viewport_pos.y / SCROLL_MAP_TILE_HEIGHT_PIX + SCROLL_AREA_EXTENDED_HEIGHT / SCROLL_MAP_TILE_HEIGHT;
    uint16_t col = scroll_map.viewport_pos.x / SCROLL_MAP_TILE_WIDTH_PIX - 1;
    for ( i = 0; i < SCROLL_AREA_EXTENDED_WIDTH / SCROLL_MAP_TILE_WIDTH; i++ )
        tile_draw_offscreen(	// offscreen cell coordinates
            SCROLL_AREA_EXTENDED_HEIGHT - SCROLL_MAP_TILE_HEIGHT,	// row = bottom - 1 tile height
            SCROLL_MAP_TILE_WIDTH * i,					// col = loop
            scroll_map_get_tile_at( row, col + i )			// use proper tile from map
        );
}

void scroll_map_draw_hidden_left_col( void ) {
    uint8_t i;
    // scroll map coordinates
    uint16_t row = scroll_map.viewport_pos.y / SCROLL_MAP_TILE_HEIGHT_PIX - 1;
    uint16_t col = scroll_map.viewport_pos.x / SCROLL_MAP_TILE_WIDTH_PIX - 1;
    for ( i = 0; i < SCROLL_AREA_EXTENDED_HEIGHT / SCROLL_MAP_TILE_HEIGHT; i++ )
        tile_draw_offscreen(	// offscreen cell coordinates
            SCROLL_MAP_TILE_HEIGHT * i,			// row = loop
            0,						// col = 0
            scroll_map_get_tile_at( row + i, col )	// use proper tile from map
        );
}

void scroll_map_draw_hidden_right_col( void ) {
    uint8_t i;
    // scroll map coordinates
    uint16_t row = scroll_map.viewport_pos.y / SCROLL_MAP_TILE_HEIGHT_PIX - 1;
    uint16_t col = scroll_map.viewport_pos.x / SCROLL_MAP_TILE_WIDTH_PIX + SCROLL_AREA_EXTENDED_WIDTH / SCROLL_MAP_TILE_WIDTH;
    for ( i = 0; i < SCROLL_AREA_EXTENDED_HEIGHT / SCROLL_MAP_TILE_HEIGHT; i++ )
        tile_draw_offscreen(	// offscreen cell coordinates
            SCROLL_MAP_TILE_HEIGHT * i,					// row = loop
            SCROLL_AREA_EXTENDED_WIDTH - SCROLL_MAP_TILE_WIDTH,		// col = right - 1 tile width
            scroll_map_get_tile_at( row + i, col )			// use proper tile from map
        );
}

void scroll_map_draw_viewport( void ) {
    uint8_t i,j;
    // scroll map coordinates
    uint16_t row = scroll_map.viewport_pos.y / SCROLL_MAP_TILE_HEIGHT_PIX;
    uint16_t col = scroll_map.viewport_pos.x / SCROLL_MAP_TILE_WIDTH_PIX;
    for ( j = 0; j < SCROLL_AREA_WIDTH / SCROLL_MAP_TILE_WIDTH; j++ )
        for ( i = 0; i < SCROLL_AREA_HEIGHT / SCROLL_MAP_TILE_HEIGHT; i++ )
            tile_draw_offscreen(	// offscreen cell coordinates
                ( i + 1 ) * SCROLL_MAP_TILE_HEIGHT,
                ( j + 1 ) * SCROLL_MAP_TILE_WIDTH,
                scroll_map_get_tile_at( row + i, col + j )
            );
}
