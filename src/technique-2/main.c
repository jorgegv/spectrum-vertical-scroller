#include <arch/zx/spectrum.h>
#include <intrinsic.h>
#include <string.h>

#include "offscreen.h"

uint8_t ball_tile[ 8 ] = { 0x38, 0x44, 0x92, 0xba, 0x92, 0x44, 0x38, 0x00 };

void main( void ) {
    uint8_t i;

    // initialize real and virtual screen
    zx_border( INK_GREEN );
    zx_colour( INK_WHITE | PAPER_BLACK );
    init_offscreen();

    // draw some tiles in virtual screen
    for ( i = 0; i < SCROLL_COLS; i++ ) {
        offscreen_draw_tile( i, i, ball_tile );
    }

    // do some magic with virtual screen
    i = 0;
    while( 1 ) {
        // sync to retrace
        intrinsic_halt();
        zx_border( INK_RED );

        // draw frame
        asm_offscreen_show_frame_stack();
//        offscreen_show_frame();

        // adjust scrolling window - technique 2

        // check if we need to draw new tiles
        if ( ! ( current_scroll_start_line % SCROLL_EXTRA_LINES ) ) {
//            // draw additional tiles on the extra row
            offscreen_clear_extra_tile_row();
            offscreen_draw_tile( current_extra_tiles_row, i, ball_tile );
            offscreen_draw_tile( current_extra_tiles_row + 1, i, ball_tile );
            if ( ++i == SCROLL_COLS )
                i = 0;

            // prepare extra row for next time it is needed
            if ( ! current_extra_tiles_row ) {
                current_extra_tiles_row = SCROLL_ROWS + SCROLL_EXTRA_ROWS;
            }
            current_extra_tiles_row -= SCROLL_EXTRA_ROWS;
        }

        // next frame, start drawing on the next line upwards
        // check and do rollover
        if ( ! current_scroll_start_line ) {
            current_scroll_start_line = SCROLL_LINES + SCROLL_EXTRA_LINES;
        }
        current_scroll_start_line--;

        zx_border( INK_BLACK );
    }

    // end of program
    while(1);
}
