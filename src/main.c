#include <arch/zx/spectrum.h>
#include <intrinsic.h>

#include "offscreen.h"

uint8_t ball_tile[ 8 ] = { 0x38, 0x44, 0x92, 0xba, 0x92, 0x44, 0x38, 0x00 };

void main( void ) {

    // initialize real and virtual screen
    zx_border( INK_BLACK );
    zx_colour( INK_WHITE | PAPER_BLACK );
    init_offscreen();

    // draw some tiles in virtual screen
    uint8_t i;
    for ( i = 0; i < SCROLL_COLS; i++ ) {
        offscreen_draw_tile( i, i, ball_tile );
    }

//    offscreen_show_frame();
//    asm_offscreen_show_frame_ldir();
//    asm_offscreen_show_frame_ldi();
//    asm_offscreen_show_frame_stack();
//    while (1);

    // do some magic with virtual screen
    i = 0;
    while( 1 ) {
        // sync to retrace
        intrinsic_halt();
        zx_border( INK_RED );

        // draw frame
//        offscreen_show_frame();
//        asm_offscreen_show_frame_ldir();
//        asm_offscreen_show_frame_ldi();
        asm_offscreen_show_frame_stack();

        // adjust scrolling window
        if ( ! current_scroll_offset_line-- ) {
            current_scroll_offset_line = SCROLL_EXTRA_LINES - 1;
            offscreen_scroll_down();
            offscreen_draw_tile( 0, i, ball_tile );
            offscreen_draw_tile( 1, i, ball_tile );
            if ( ++i == SCROLL_COLS )
                i = 0;
        }

        zx_border( INK_BLACK );
    }

    // end of program
    while(1);
}
