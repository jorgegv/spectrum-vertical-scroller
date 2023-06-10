#include "build.h"

///////////////////
// main function
///////////////////

void main( void ) {

    zx_border(INK_BLACK);

    // initializations
    init_interrupts();
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
    reset_perfmeter();
    while (1) {

//      if ( ! ( scroll_counter_1 % 8 ) )
//        dump_invalidations();

      // draw tiles if top row has been completely scrolled down
      // this updates the invalidation ranges for all affected columns
      draw_top_row_of_tiles();

      // do whatever we want with the background
      // this increments scroll_counters!
      scroll_down_area();

      // invalidate dirty areas in scroll area
      invalidate_dirty_scrollarea();

      // move the sprites
      move_sprites();

      // move down the column invalidation ranges if needed
      move_down_column_invalidation_ranges();

      // redraw
      sp1_UpdateNow();
      do_perf_accounting();
    }
}
