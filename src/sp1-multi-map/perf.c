#include <intrinsic.h>
#include <arch/z80.h>
#include <spectrum.h>
#include <stdio.h>
#include <conio.h>

#include "perf.h"

// interrupt setup for performance accounting

uint32_t global_frame_counter;

void my_isr( void ) __interrupt {
    global_frame_counter++;
}

void init_interrupts( void ) {
    intrinsic_di();
    zx_im2_init((void *) 0xD000, 0xD1);
    z80_bpoke(0xD1D1, 0xC3);
    z80_wpoke(0xD1D2, (uint16_t) my_isr);
    global_frame_counter = 0;
    intrinsic_ei();
}

// performance accounting functions

uint8_t main_loop_counter;
uint32_t last_frame_counter;

void init_perfmeter( void ) {
    init_interrupts();
    reset_perfmeter();
}

void reset_perfmeter( void ) {
    main_loop_counter = 0;
    last_frame_counter = global_frame_counter;
}

void do_perf_accounting( void ) {
    // count a new loop iteration
    main_loop_counter++;

    // if one second has passed, show FPS marker and reset perfmeter
    if ( global_frame_counter - last_frame_counter >= FRAMES_PER_SECOND ) {
        gotoxy( 40, 23 );
        printf( "FPS: %-3d", main_loop_counter );
        reset_perfmeter();
    }
}

