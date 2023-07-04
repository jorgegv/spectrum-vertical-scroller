#include <stdint.h>

#include "build.h"

#define DIR_UP		0x01
#define DIR_DOWN	0x02
#define DIR_LEFT	0x04
#define DIR_RIGHT	0x08

uint8_t directions[8] = {
    DIR_DOWN,			// 'A'
    DIR_DOWN | DIR_RIGHT,	// 'B'
    DIR_RIGHT,			// 'C'
    DIR_RIGHT | DIR_UP,		// 'D'
    DIR_UP,			// 'E'
    DIR_UP | DIR_LEFT,		// 'F'
    DIR_LEFT,			// 'G'
    DIR_LEFT | DIR_DOWN,	// 'H'
};

uint16_t viewport_x, viewport_y;

void scroll_dir( uint8_t dir, uint8_t num_pix ) {
    if ( dir & DIR_UP ) {
        if ( viewport_y > num_pix ) {
            if ( ! ( viewport_y % SCROLL_MAP_TILE_HEIGHT_PIX ) )
                scroll_map_draw_hidden_top_row();
            offscreen_scroll_down_pixels( num_pix );
            viewport_y -= num_pix;
        }
    }
    if ( dir & DIR_DOWN ) {
        if ( viewport_y < SCROLL_MAP_HEIGHT_PIX - SCROLL_AREA_HEIGHT_PIX - num_pix ) {
            if ( ! ( viewport_y % SCROLL_MAP_TILE_HEIGHT_PIX ) )
                scroll_map_draw_hidden_bottom_row();
            offscreen_scroll_up_pixels( num_pix );
            viewport_y += num_pix;
        }
    }
    if ( dir & DIR_LEFT ) {
        if ( viewport_x > num_pix ) {
            if ( ! ( viewport_x % SCROLL_MAP_TILE_WIDTH_PIX ) )
                scroll_map_draw_hidden_left_col();
            offscreen_scroll_right_pixels( num_pix );
            viewport_x -= num_pix;
        }
    }
    if ( dir & DIR_RIGHT ) {
        if ( viewport_x < SCROLL_MAP_WIDTH_PIX - SCROLL_AREA_WIDTH_PIX - num_pix ) {
            if ( ! ( viewport_x % SCROLL_MAP_TILE_WIDTH_PIX ) )
                scroll_map_draw_hidden_right_col();
            offscreen_scroll_left_pixels( num_pix );
            viewport_x += num_pix;
        }
    }
}

#define SCROLL_STEP		8
#define SCROLL_PATH_SIZE	( 48 * 8  / SCROLL_STEP)

#if ( SCROLL_STEP == 1 )

// path for 1-pixel scrolling
uint8_t scroll_path[] = {
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
    "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"
    "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
};

#elif ( SCROLL_STEP == 2 )

// path for 2-pixel scrolling
uint8_t scroll_path[] = {
    "AAAAAAAAAAAAAAAAAAAAAAAA"
    "BBBBBBBBBBBBBBBBBBBBBBBB"
    "CCCCCCCCCCCCCCCCCCCCCCCC"
    "DDDDDDDDDDDDDDDDDDDDDDDD"
    "EEEEEEEEEEEEEEEEEEEEEEEE"
    "FFFFFFFFFFFFFFFFFFFFFFFF"
    "GGGGGGGGGGGGGGGGGGGGGGGG"
    "HHHHHHHHHHHHHHHHHHHHHHHH"
};

#elif ( SCROLL_STEP == 4 )

// path for 4-pixel scrolling
uint8_t scroll_path[] = {
    "AAAAAAAAAAAA"
    "BBBBBBBBBBBB"
    "CCCCCCCCCCCC"
    "DDDDDDDDDDDD"
    "EEEEEEEEEEEE"
    "FFFFFFFFFFFF"
    "GGGGGGGGGGGG"
    "HHHHHHHHHHHH"
};

#elif ( SCROLL_STEP == 8 )

// path for 8-pixel scrolling
uint8_t scroll_path[] = {
    "AAAAAA"
    "BBBBBB"
    "CCCCCC"
    "DDDDDD"
    "EEEEEE"
    "FFFFFF"
    "GGGGGG"
    "HHHHHH"
};

#else
    #error "SCROLL_STEP must be defined with value 1, 2, 4 or 8"
#endif
    
void main( void ) {
    uint16_t i;

    init_perfmeter();
    init_screen_address_tables();
    init_tile_map();

    // initial setup and draw
    viewport_x = 32;
    viewport_y = 96;
    scroll_map_set_viewport_xy( viewport_x, viewport_y );
    scroll_map_draw_viewport();

    reset_perfmeter();
    while (1) {
        for ( i=0; i < SCROLL_PATH_SIZE; i++ ) {
            scroll_dir( directions[ scroll_path[ i ] - 'A' ], SCROLL_STEP );
            scroll_map_set_viewport_xy( viewport_x, viewport_y );
            redraw_scroll_area();
            do_perf_accounting();
        }
    }
}
