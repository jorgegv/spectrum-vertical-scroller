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

void init_tile_map( void ) {
    uint8_t r,c;

    // initialize SP1
    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
      PAPER_BLACK | INK_GREEN, ' ');

    // setup the SP1 tile map to point to the cells in the offscreen buffer
    // the top rows are reserved for drawing and not visible
    // the tile array is configured in columns!
    for ( c = 0; c < SCROLL_AREA_WIDTH; c++ )
      for ( r = 0; r < SCROLL_AREA_HEIGHT; r++ )
        sp1_PrintAt( SCROLL_AREA_TOP + r, SCROLL_AREA_LEFT + c,          // screen position
          PAPER_YELLOW | INK_BLACK,
//          ( ( (r+c) % 2) ? PAPER_BLACK : PAPER_RED ) | INK_WHITE | BRIGHT,      // attr
          ( uint16_t ) offscreen_cell_address( SCROLL_MAP_TILE_HEIGHT + r, SCROLL_MAP_TILE_WIDTH + c ) );       // pointer
}

struct sp1_Rect scroll_area = {
    .row	= SCROLL_AREA_TOP,
    .col	= SCROLL_AREA_LEFT,
    .width	= SCROLL_AREA_WIDTH,
    .height	= SCROLL_AREA_HEIGHT,
};

void redraw_scroll_area( void ) {
    sp1_Invalidate( &scroll_area );
    sp1_UpdateNow();
}
