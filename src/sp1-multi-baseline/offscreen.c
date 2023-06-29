#include <stdint.h>

#include "build.h"

#define OFFSCREEN_TOTAL_SIZE_BYTES	( SCROLL_AREA_EXTENDED_WIDTH * SCROLL_AREA_EXTENDED_HEIGHT * 8 )
uint8_t offscreen[ OFFSCREEN_TOTAL_SIZE_BYTES ];

// the offscreen[] is again stored in column-major form, for fast vertical scrolling
uint8_t *offscreen_cell_address( uint8_t row, uint8_t col ) {
    return &offscreen[ ( col * SCROLL_AREA_EXTENDED_HEIGHT + row ) * 8 ];
}

