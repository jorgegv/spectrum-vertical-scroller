#include <stdint.h>
#include <string.h>
#include <arch/zx/spectrum.h>

#include "offscreen.h"

// offscreen linear framebuffer
uint8_t offscreen[ SCROLL_COLS * ( SCROLL_LINES + SCROLL_EXTRA_LINES ) ];

// initial screen addresses for each line of the scroll window
uint8_t *screen_line_address[ SCROLL_LINES ];
uint8_t *screen_line_end_address[ SCROLL_LINES ];

// address of each of the extra lines in the offscreen, plus the first
uint8_t *offscreen_extra_line_address[ SCROLL_EXTRA_LINES + 1 ];
uint8_t *offscreen_extra_line_end_address[ SCROLL_EXTRA_LINES + 1 ];

// current line in the offscreen to start drawing the real screen from
uint8_t current_scroll_offset_line;

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
    for ( i = 0; i < SCROLL_LINES ; i++ ) {
        screen_line_address[ i ] = zx_py2saddr( SCROLL_POS_ROW * 8 + i ) + SCROLL_POS_COL;
        screen_line_end_address[ i ] = zx_py2saddr( SCROLL_POS_ROW * 8 + i ) + SCROLL_POS_COL + SCROLL_COLS;
    }
}

void init_offscreen_extra_lines_offset_tables( void ) {
    uint8_t i;
    for ( i = 0; i < SCROLL_EXTRA_LINES + 1; i++ ) {
        offscreen_extra_line_address[ i ] = &offscreen[ i * SCROLL_COLS ];
        offscreen_extra_line_end_address[ i ] = &offscreen[ i * SCROLL_COLS ] + SCROLL_COLS;
    }
}

void offscreen_show_frame( void ) {
    uint8_t line;
    uint16_t offscreen_address;

    offscreen_address = offscreen_extra_line_address[ current_scroll_offset_line ];
    for ( line = 0; line < SCROLL_LINES; line++ ) {
        memcpy( screen_line_address[ line ], offscreen_address, SCROLL_COLS );
        offscreen_address += SCROLL_COLS;
    }
}

void offscreen_draw_tile( uint8_t row, uint8_t col, uint8_t *udg ) {
    uint8_t *addr = &offscreen[ row * SCROLL_COLS * 8 + col ];
    uint8_t i = 8;
    while ( i-- ) {
        *addr = *udg++;
        addr += SCROLL_COLS;
    }
}

void offscreen_scroll_down( void ) {
    memmove( offscreen_extra_line_address[ 16 ], offscreen_extra_line_address[ 0 ], SCROLL_SIZE_BYTES );
    memset( offscreen_extra_line_address[ 0 ], 0, SCROLL_EXTRA_LINES_BYTES );
}

void init_offscreen( void ) {
    init_real_screen_address_tables();
    init_offscreen_extra_lines_offset_tables();
    current_scroll_offset_line = 0;
}
