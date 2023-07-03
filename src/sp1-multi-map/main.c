#include <stdint.h>

#include "build.h"

void main( void ) {
    uint16_t viewport_x, viewport_y;

    init_perfmeter();
    init_screen_address_tables();
    init_tile_map();

    // initial setup
    viewport_x = 16;
    viewport_y = 16;

    // initial draw
    scroll_map_set_viewport_xy( viewport_x, viewport_y );
    scroll_map_draw_viewport();

    reset_perfmeter();
    while (1) {

        if ( ! ( viewport_x % SCROLL_MAP_TILE_WIDTH_PIX ) )
            scroll_map_draw_hidden_right_col();
        offscreen_scroll_left_pixels( 1 );
        viewport_x++;
        scroll_map_set_viewport_xy( viewport_x, viewport_y );

        redraw_scroll_area();
    	do_perf_accounting();
    }
}
