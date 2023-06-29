#include <stdint.h>

#include "build.h"

void tile_draw_offscreen_1x1( uint8_t *tile, uint8_t row, uint8_t col ) {
    uint8_t i;
    uint8_t *addr;
    addr = offscreen_cell_address( row, col );
    for ( i = 0; i < 8; i++ )
        *addr++ = tile[ i ];
}

void tile_draw_offscreen_2x2( uint8_t *tile, uint8_t row, uint8_t col ) {
    tile_draw_offscreen_1x1( tile, row, col );
    tile_draw_offscreen_1x1( tile + 8, row + 1, col );
    tile_draw_offscreen_1x1( tile + 16, row, col + 1 );
    tile_draw_offscreen_1x1( tile + 24, row + 1, col + 1 );
}

