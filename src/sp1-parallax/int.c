#include <intrinsic.h>
#include <arch/z80.h>
#include <spectrum.h>

#include "build.h"

uint32_t global_frame_counter;

void my_isr( void ) __interrupt {
    global_frame_counter++;
}

void init_interrupts( void ) {
    intrinsic_di();
    zx_im2_init((void *) 0xD000, 0xD1);
    z80_bpoke(0xD1D1, 0xC3);
    z80_wpoke(0xD1D2, (uint16_t) my_isr);
    intrinsic_ei();
    global_frame_counter = 0;
}
