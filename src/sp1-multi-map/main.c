#include <stdint.h>
#include <stdlib.h>

#include "build.h"

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

#define SCROLL_STEP		1

#if ( SCROLL_STEP == 1 )

// path for 1-pixel scrolling
uint8_t scroll_path[] = {
/*
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
    "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"
    "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
*/
/*
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
*/
/*
    "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"
    "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"
    "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"
    "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"
*/
/*
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
*/
/*
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
*/

/*
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
    "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"
    "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"
    "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
*/
/*
    "ACACACACACACACACACACACACACACACACACAC"
*/
/*
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
*/
/*
    "CCCCCCCCCCCCCCCC"
    "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
    "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"
    "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"
    "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
*/
    // walls map walk
    "AAAAAAAAAAAAAAA"
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
    "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"
    "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"
    "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
};

#elif ( SCROLL_STEP == 2 )

// path for 2-pixel scrolling
uint8_t scroll_path[] = {
    // walls map walk
    "AAAAAAAAAAAAAAAA"
    "AAAAAAAAAAAAAAAA"
    "BBBBBBBBBBBBBBBB"
    "CCCCCCCCCCCCCCCC"
    "CCCCCCCCCCCCCCCC"
    "DDDDDDDDDDDDDDDD"
    "EEEEEEEEEEEEEEEE"
    "EEEEEEEEEEEEEEEE"
    "FFFFFFFFFFFFFFFF"
    "GGGGGGGGGGGGGGGG"
    "GGGGGGGGGGGGGGGG"
    "HHHHHHHHHHHHHHHH"
};

#elif ( SCROLL_STEP == 4 )

// path for 4-pixel scrolling
uint8_t scroll_path[] = {
    // walls map walk
    "AAAAAAAA"
    "AAAAAAAA"
    "BBBBBBBB"
    "CCCCCCCC"
    "CCCCCCCC"
    "DDDDDDDD"
    "EEEEEEEE"
    "EEEEEEEE"
    "FFFFFFFF"
    "GGGGGGGG"
    "GGGGGGGG"
    "HHHHHHHH"
};

#elif ( SCROLL_STEP == 8 )

// path for 8-pixel scrolling
uint8_t scroll_path[] = {
    // walls map walk
    "AAAA"
    "AAAA"
    "BBBB"
    "CCCC"
    "CCCC"
    "DDDD"
    "EEEE"
    "EEEE"
    "FFFF"
    "GGGG"
    "GGGG"
    "HHHH"
};

#else
    #error "SCROLL_STEP must be defined with value 1, 2, 4 or 8"
#endif
    
uint8_t scroll_step = SCROLL_STEP;
uint8_t kbd_state = 0;

void update_status_panel( void ) {
    gotoxy( 52, 2 ); puts( "VIEWPORT:" );
    gotoxy( 52, 3 ); printf( "   X: %-3d", scroll_map.viewport_pos.x);
    gotoxy( 52, 4 ); printf( "   Y: %-3d", scroll_map.viewport_pos.y );
    gotoxy( 52, 6 ); printf( "STEP: %dpx ", scroll_step );
    gotoxy( 52, 8 ); printf( " KBD: 0x%02X", kbd_state );
}

void clear_status_panel( void ) {
    uint8_t i;
    for (i=2; i <= 8; i++ ) {
        gotoxy( 52, i ); puts("            ");
    }
}

void main( void ) {
    static uint8_t *p,dir;

    init_perfmeter();
    init_screen();
    init_kbd();

    // horizontal checks
//    scroll_map_set_viewport_xy( 0, 0 );
//    scroll_map_set_viewport_xy( 0, 16 );
//    scroll_map_set_viewport_xy( 0, 32 );
//    scroll_map_set_viewport_xy( 0, 240 );
//    scroll_map_set_viewport_xy( 0, 256 );

    // vertical checks
//    scroll_map_set_viewport_xy( 0, 0 );
//    scroll_map_set_viewport_xy( 16, 0 );
//    scroll_map_set_viewport_xy( 32, 0 );
//    scroll_map_set_viewport_xy( 624, 0 );
//    scroll_map_set_viewport_xy( 640, 0 );

    // initial setup and draw
//    scroll_map_set_viewport_xy( 0, 48 );
    scroll_map_set_viewport_xy( 0, 32 );
    scroll_map_draw_viewport();
    redraw_scroll_area();
    if ( debug_enabled )
        update_status_panel();

//    gotoxy( 0,22 ); printf("VX:%3d VY:%3d",scroll_map.viewport_pos.x,scroll_map.viewport_pos.y );

    reset_perfmeter();
    while (1) {

#if 1
        // either this loop: read kbd and scroll according to QAOP
        kbd_state = kbd_read();

        if ( kbd_state ) {
            // check debug toggle
            if ( kbd_state & KBD_IN_DEBUG ) {
                debug_enabled = ! debug_enabled;
                if ( debug_enabled ) {
                    enable_hidden_band_view();
                    update_status_panel();
                } else {
                    disable_hidden_band_view();
                    clear_status_panel();
                }
                msleep( 100 );
            }

            // check speed toggle
            if ( kbd_state & KBD_IN_SPEED ) {
                scroll_step *= 2;
                if ( scroll_step > 8 )
                    scroll_step = 1;
                update_status_panel();
                msleep( 100 );
            }

            // check movements
            dir = 0;
            dir |= ( kbd_state & in_UP ? DIR_UP : 0 );
            dir |= ( kbd_state & in_DOWN ? DIR_DOWN : 0 );
            dir |= ( kbd_state & in_LEFT ? DIR_LEFT : 0 );
            dir |= ( kbd_state & in_RIGHT ? DIR_RIGHT : 0 );
            if ( dir ) {
                scroll_map_scroll_viewport( dir, scroll_step );
                redraw_scroll_area();
                if ( debug_enabled )
                    update_status_panel();
            }
        }

        do_perf_accounting();
#else
        // or this other: read 'path' and run the walk encoded there
        p = scroll_path;
        while ( dir = *p++ ) {	// walk path
            scroll_map_scroll_viewport( directions[ dir - 'A'], SCROLL_STEP );
            redraw_scroll_area();
            do_perf_accounting();
        }
#endif
    } // loop forever
}
