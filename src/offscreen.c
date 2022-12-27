#include <stdint.h>
#include <string.h>
#include <arch/zx/spectrum.h>

#include "offscreen.h"

// offscreen linear framebuffer
uint8_t offscreen[ SCROLL_COLS * ( SCROLL_LINES + SCROLL_EXTRA_LINES ) ];

// initial screen addresses for each line of the scroll window
uint8_t *screen_line_address[ SCROLL_LINES ];

// Initializes the LUT for the initial address for each line that will be
// drawn on the real screen in the scroll window, taking into account the
// position of the scroll window: offset by (SCROLL_POS_ROW * 8) and by
// SCROLL_POS_COL where needed.
//
// This is needed just for the real screen, where addresses are weird.  The
// origin framebuffer is linear and addresses are consecutive, so no LUT
// needed there.
void init_real_screen_address_table( void ) {
    uint8_t i;
    for ( i = 0; i < SCROLL_LINES ; i++ ) {
        screen_line_address[ i ] = zx_py2saddr( SCROLL_POS_ROW * 8 + i ) + SCROLL_POS_COL;
    }
}

void init_offscreen( void ) {
    init_real_screen_address_table();
}

void offscreen_show_frame( void ) {
    uint8_t line;
    uint16_t offscreen_address;
    offscreen_address = (uint16_t) &offscreen[ 0 ];
    for ( line = 0; line < SCROLL_LINES; line++ ) {
        memcpy( screen_line_address[ line ], (void *) offscreen_address, SCROLL_COLS );
        offscreen_address += SCROLL_COLS;
    }
}
