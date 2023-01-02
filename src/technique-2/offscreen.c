#include <stdint.h>
#include <string.h>
#include <arch/zx/spectrum.h>

#include "offscreen.h"

// offscreen linear framebuffer
uint8_t offscreen[ SCROLL_COLS * ( SCROLL_LINES + SCROLL_EXTRA_LINES ) ];

// initial and end+1 screen addresses for each line of the scroll window
uint8_t *screen_line_address[ SCROLL_LINES ];
uint8_t *screen_line_end_address[ SCROLL_LINES ];

// initial and end+1 addresses of each of the lines in the offscreen, plus the first
uint8_t *offscreen_line_address[ SCROLL_LINES + SCROLL_EXTRA_LINES + 1 ];
uint8_t *offscreen_line_end_address[ SCROLL_LINES + SCROLL_EXTRA_LINES + 1 ];

// current line in the offscreen to start drawing the real screen from
uint8_t current_scroll_start_line;

// the row where extra tiles are drawn when new graphics are being made visible
uint8_t current_extra_tiles_row;

// Initializes the LUT for the initial address for each line that will be
// drawn on the real screen in the scroll window, taking into account the
// position of the scroll window: offset by (SCROLL_POS_ROW * 8) and by
// SCROLL_POS_COL where needed.
//
// This is needed just for the real screen, where addresses are weird.  The
// origin framebuffer is linear and addresses are consecutive, so no LUT
// needed there.
void init_real_screen_address_tables( void ) {
    uint8_t i;
    uint8_t *addr;
    for ( i = 0; i < SCROLL_LINES ; i++ ) {
        addr = zx_py2saddr( SCROLL_POS_ROW * 8 + i ) + SCROLL_POS_COL;
        screen_line_address[ i ] = addr;
        screen_line_end_address[ i ] = addr + SCROLL_COLS;
    }
}

void init_offscreen_address_tables( void ) {
    uint8_t i;
    uint8_t *addr;
    for ( i = 0; i < SCROLL_LINES + SCROLL_EXTRA_LINES + 1; i++ ) {
        addr = &offscreen[ i * SCROLL_COLS ];
        offscreen_line_address[ i ] = addr;
        offscreen_line_end_address[ i ] = addr + SCROLL_COLS;
    }
}

void offscreen_show_frame( void ) {
    uint8_t screen_line;
    uint8_t offscreen_line;

    offscreen_line = current_scroll_start_line;
    for ( screen_line = 0; screen_line < SCROLL_LINES; screen_line++ ) {
        memcpy( screen_line_address[ screen_line ], offscreen_line_address[ offscreen_line ], SCROLL_COLS );
        offscreen_line++;
        if ( offscreen_line == SCROLL_LINES + SCROLL_EXTRA_LINES ) {
            offscreen_line = 0;
        }
    }
}

void offscreen_clear_extra_tile_row( void ) {
    memset( &offscreen[ current_extra_tiles_row * 8 * SCROLL_COLS ], 0, SCROLL_EXTRA_LINES * SCROLL_COLS );
}

void offscreen_draw_tile( uint8_t row, uint8_t col, uint8_t *udg ) {
    uint8_t *addr = &offscreen[ row * SCROLL_COLS * 8 + col ];
    uint8_t i = 8;
    while ( i-- ) {
        *addr = *udg++;
        addr += SCROLL_COLS;
    }
}

void init_offscreen( void ) {
    init_real_screen_address_tables();
    init_offscreen_address_tables();
    current_scroll_start_line = SCROLL_EXTRA_LINES;
    current_extra_tiles_row = 0;
}
