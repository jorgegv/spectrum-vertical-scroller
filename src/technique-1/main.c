#include <arch/zx/spectrum.h>
#include <intrinsic.h>

#include "offscreen.h"
#include "sprite.h"

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

//    offscreen_show_frame();
//    asm_offscreen_show_frame_ldir();
//    asm_offscreen_show_frame_ldi();
//    asm_offscreen_show_frame_stack();
//    while (1);

    for ( i = 1; i < 4; i++ ) {
        generate_shifted_sprite( &ball_sprite, i * 2 );
    }

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
            // draw the sprite
            draw_sprite_column( (uint8_t *)( ball_sprite.cells[0] ), &ball_sprite.save_buffer[0], &offscreen[0] );
            draw_sprite_column( (uint8_t *)( ball_sprite.cells[0] + 2 ), &ball_sprite.save_buffer[16], &offscreen[1] );
            draw_sprite_column( (uint8_t *)( ball_sprite.cells[0] + 4 ), &ball_sprite.save_buffer[32], &offscreen[2] );

            if ( ++i == SCROLL_COLS )
                i = 0;
        }

        zx_border( INK_BLACK );
    }

    // end of program
    while(1);
}
