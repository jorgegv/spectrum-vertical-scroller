#include "build.h"

// some tile definitions for the map

// blank tile
uint8_t blank_tile_2x2_data[ 32 ] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
struct tile_s blank_tile_2x2 = { 2, 2, blank_tile_2x2_data };

// other tiles
struct tile_s diamond	= { 2, 2, diamond_tile };
struct tile_s clubs	= { 2, 2, clubs_tile };
struct tile_s spades	= { 2, 2, spades_tile };
struct tile_s hearts	= { 2, 2, hearts_tile };

struct tile_s *tile_bank[ 4 ] = {
    &diamond,	// 'A'
    &clubs,	// 'B'
    &spades,	// 'C'
    &hearts,	// 'D'
};

// the scroll map data - must be SCROLL_MAP_WIDTH * SCROLL_MAP_HEIGHT chars
// a space represents the blank tile above
// a letter A-Z is translated to tile[ code - 65 ] (A=0, B=1, C=2, etc.)
// all the map has to be surrounded by a background 1-tile wide band for efficiency
uint8_t scrollmap_data[] =
    "                                                  "
    " AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA "
    " AB A C D  B   C  D       C C C C  B    A B     A "
    " A B A C   B   C  D  AAAA   C C C  B    A       A "
    " A  C A C  B   C    A    A    C C  B    A   C   A "
    " A   C A   B        A    A      C  B    A     D A "
    " A    D A  B        A    A        B    A  A     A "
    " A     D    B       A    A        B    A        A "
    " A AAAAA     B       AAAA        B     A   B    A "
    " A      AA    B                 B     A         A "
    " A        A    BB             BB      A  C    D A "
    " A  B   B  A     BBBB     BBBB       A          A "
    " A    B     A        BBBBB         AA     A     A "
    " A    B      AA                 AAA         B   A "
    " A             AAA          AAAA     C  D     A A "
    " A  BBBBB         AAAAAAAAAA                    A "
    " A     CCCCC                           CCC      A "
    " A    CCCCCCC     AAA       BBB       C   C CCC A "
    " A   CC AAA CC   A   A     B   B     C     C    A "
    " A   CC AAA CC        A   A     B   B         BBA "
    " A   CCCCCCCCC         AAA       BBB       CCC  A "
    " A   CCCCCCCCC                         DDDD     A "
    " A   D D D D D   AAAAA       BBBBBCCCCC        AA "
    " A                    DDDDAAA                   A "
    " AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA "
    "                                                  "
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
    gotoxy( 0,18 );
    printf( "Top: r=%-2d c=%-2d", row, col );
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
    gotoxy( 0,19 );
    printf( "Bot: r=%-2d c=%-2d", row, col );
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
    gotoxy( 0,20 );
    printf( "Lef: r=%-2d c=%-2d", row, col );
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
    gotoxy( 0,21 );
    printf( "Rig: r=%-2d c=%-2d", row, col );
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
