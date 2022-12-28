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
    for ( i = 0; i < 8; i++ ) {
        offscreen_draw_tile( i, i, ball_tile );
    }

    // do some magic with virtual screen
    while( 1 ) {
        intrinsic_halt();
        zx_border( INK_RED );
//        offscreen_show_frame();
//        asm_offscreen_show_frame_ldir();
        asm_offscreen_show_frame_ldi();
        zx_border( INK_BLACK );
    }

    // end of program
    while(1);
}
