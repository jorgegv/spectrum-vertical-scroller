#include "build.h"

///////////////////
// main function
///////////////////

void main( void ) {

    zx_border(INK_BLACK);

    // initializations
    init_heap();
    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
      PAPER_BLACK | INK_GREEN, ' ');
    init_address_tables();
    init_scroll_area();
    init_tile_map();
    init_sprites();
    init_column_invalidation_ranges();

    // make sure the scroll area is cleared at start
    sp1_Invalidate( &scroll_area );

    // main loop
    while (1) {

//      dump_invalidations();

      // draw tiles if top row has been completely scrolled down
      // this updates the invalidation ranges for all affected columns
      if ( ! ( scroll_counter % (SCROLL_AREA_TOP_TILE_HEIGHT * 8 ) ) )
        draw_top_row_of_tiles();

      // do whatever we want with the background
      // this increments scroll_counter!
      scroll_down_area();

      // invalidate dirty areas in scroll area
      invalidate_dirty_scrollarea();

      // move the sprites
      move_sprites();

      // move down the column invalidation ranges if needed
      if ( ! ( scroll_counter % 8 ) )
        move_down_column_invalidation_ranges();

      // now sync with vert retrace and redraw
      intrinsic_halt();
      sp1_UpdateNow();
    }
}
