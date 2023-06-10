#include <stdio.h>

#include "build.h"

#define FRAMES_PER_SECOND 50

uint8_t main_loop_counter;

uint32_t last_frame_counter;

void reset_perfmeter( void ) {
    main_loop_counter = 0;
    last_frame_counter = global_frame_counter;
}

void do_perf_accounting( void ) {
    // count a new loop iteration
    main_loop_counter++;

    // if one second has passed, show FPS marker and reset perfmeter
    if ( global_frame_counter - last_frame_counter >= FRAMES_PER_SECOND ) {
        gotoxy( 0, 23 );
        printf( "FPS: %d", main_loop_counter );
        reset_perfmeter();
    }
}
