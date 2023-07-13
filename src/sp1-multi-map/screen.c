#include <arch/zx/spectrum.h>
#include <sprites/sp1.h>

#include "build.h"

uint8_t *scroll_area_screen_line_address[ SCROLL_AREA_HEIGHT * 8 ];
uint8_t *scroll_area_screen_line_end_address[ SCROLL_AREA_HEIGHT * 8 ];

void init_screen_address_tables( void ) {
    uint8_t i, *addr;
    for ( i = 0; i < SCROLL_AREA_HEIGHT_LINES; i++ ) {
    	addr = zx_py2saddr( SCROLL_AREA_TOP * 8 + i ) + SCROLL_AREA_LEFT;
    	scroll_area_screen_line_address[ i ] = addr;
    	scroll_area_screen_line_end_address[ i ] = addr + SCROLL_AREA_WIDTH;
    }
}

struct sp1_Rect full_screen = {
    .row = 0,
    .col = 0,
    .width = 32,
    .height = 24,
};

void init_tile_map( void ) {
    uint8_t r,c;

    // initialize SP1
    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
      DEFAULT_ATTR, ' ');

    // set default background and paper for all screen
    sp1_Invalidate( &full_screen );
    sp1_UpdateNow();

    // setup the SP1 tile map to point to the cells in the offscreen buffer
    // the top rows are reserved for drawing and not visible
    // the tile array is configured in columns!
    for ( c = 0; c < SCROLL_AREA_WIDTH; c++ )
      for ( r = 0; r < SCROLL_AREA_HEIGHT; r++ )
        sp1_PrintAt( SCROLL_AREA_TOP + r, SCROLL_AREA_LEFT + c,          // screen position
          SCROLL_AREA_ATTR,
          ( uint16_t ) offscreen_cell_address( SCROLL_MAP_TILE_HEIGHT + r, SCROLL_MAP_TILE_WIDTH + c ) );       // pointer
}

void set_hidden_tile_band_attr( uint8_t attr ) {
    uint8_t r,c;
    // top hidden bar
    for ( c = 0; c < SCROLL_AREA_EXTENDED_WIDTH; c++ )
      for ( r = 0; r < SCROLL_MAP_TILE_HEIGHT; r++ )
        sp1_PrintAt( SCROLL_AREA_EXTENDED_TOP + r, SCROLL_AREA_EXTENDED_LEFT + c,          // screen position
          attr,
          ( uint16_t ) offscreen_cell_address( r, c ) );       // pointer

    // bottom hidden bar
    for ( c = 0; c < SCROLL_AREA_EXTENDED_WIDTH; c++ )
      for ( r = 0; r < SCROLL_MAP_TILE_HEIGHT; r++ )
        sp1_PrintAt( SCROLL_AREA_BOTTOM + 1 + r, SCROLL_AREA_EXTENDED_LEFT + c,          // screen position
          attr,
          ( uint16_t ) offscreen_cell_address( SCROLL_MAP_TILE_HEIGHT + SCROLL_AREA_HEIGHT + r, c ) );       // pointer

    // left hidden bar
    for ( c = 0; c < SCROLL_MAP_TILE_WIDTH; c++ )
      for ( r = 0; r < SCROLL_AREA_EXTENDED_HEIGHT; r++ )
        sp1_PrintAt( SCROLL_AREA_EXTENDED_TOP + r, SCROLL_AREA_EXTENDED_LEFT + c,          // screen position
          attr,
          ( uint16_t ) offscreen_cell_address( r, c ) );       // pointer

    // right hidden bar
    for ( c = 0; c < SCROLL_MAP_TILE_WIDTH; c++ )
      for ( r = 0; r < SCROLL_AREA_EXTENDED_HEIGHT; r++ )
        sp1_PrintAt( SCROLL_AREA_EXTENDED_TOP + r, SCROLL_AREA_RIGHT + 1 + c,          // screen position
          attr,
          ( uint16_t ) offscreen_cell_address( r, SCROLL_MAP_TILE_WIDTH + SCROLL_AREA_WIDTH + c ) );       // pointer
}

struct sp1_Rect scroll_area_debug = {
    .row	= SCROLL_AREA_EXTENDED_TOP,
    .col	= SCROLL_AREA_EXTENDED_LEFT,
    .width	= SCROLL_AREA_EXTENDED_WIDTH,
    .height	= SCROLL_AREA_EXTENDED_HEIGHT,
};

struct sp1_Rect scroll_area_nodebug = {
    .row	= SCROLL_AREA_TOP,
    .col	= SCROLL_AREA_LEFT,
    .width	= SCROLL_AREA_WIDTH,
    .height	= SCROLL_AREA_HEIGHT,
};

struct sp1_Rect *scroll_area_p = &scroll_area_nodebug;

void redraw_scroll_area( void ) {
    sp1_Invalidate( scroll_area_p );
    sp1_UpdateNow();
}

void enable_hidden_band_view( void ) {
    set_hidden_tile_band_attr( HIDDEN_BAND_VISIBLE_ATTR );
    scroll_area_p = &scroll_area_debug;
    redraw_scroll_area();
}

void disable_hidden_band_view( void ) {
    set_hidden_tile_band_attr( HIDDEN_BAND_INVISIBLE_ATTR );
    redraw_scroll_area();
    scroll_area_p = &scroll_area_nodebug;
}

void init_screen( void ) {
    init_screen_address_tables();
    init_tile_map();

    if ( debug_enabled )
        enable_hidden_band_view();
    else
        disable_hidden_band_view();
}

