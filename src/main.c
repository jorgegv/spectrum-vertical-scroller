#include <arch/zx/spectrum.h>

#include "offscreen.h"

void main( void ) {
    zx_border( INK_BLACK );
    zx_colour( INK_WHITE | PAPER_BLACK );
    init_offscreen();

    // end of program
    while(1);
}
