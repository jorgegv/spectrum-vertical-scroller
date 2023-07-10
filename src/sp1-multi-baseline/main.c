#include <stdint.h>

#include "build.h"

void draw_initial_scroll_area( void ) {
    uint8_t i;
    for ( i = 0; i < SCROLL_AREA_WIDTH; i += SCROLL_MAP_TILE_WIDTH )
        tile_draw_offscreen_2x2( diamond_tile, SCROLL_MAP_TILE_HEIGHT + i, SCROLL_MAP_TILE_WIDTH + i );
}


void main( void ) {
    init_perfmeter();
    init_screen_address_tables();
    init_tile_map();

    draw_initial_scroll_area();

    reset_perfmeter();
    while (1) {
//        offscreen_scroll_up_pixels( 1 );
//        offscreen_scroll_down_pixels( 1 );
//        offscreen_scroll_left_pixels( 1 );
        offscreen_scroll_right_pixels( 1 );
        redraw_scroll_area();
    	do_perf_accounting();
    }
}
