#include <stdint.h>

#include "build.h"

void tile_draw_offscreen_cell( uint8_t *cell_data, uint8_t row, uint8_t col ) {
    uint8_t i;
    uint8_t *src = cell_data;
    uint8_t *dst = offscreen_cell_address( row, col );
    for ( i = 0; i < 8; i++ ) *dst++ = *src++;
}

void tile_draw_offscreen( uint8_t row, uint8_t col, struct tile_s *t ) {
    uint8_t r,c;
    uint8_t *data = t->data;
    for ( c = 0; c < t->width; c++ ) {
        for ( r = 0; r < t->height; r++ ) {
            tile_draw_offscreen_cell( data, row + r, col + c );
            data += 8;
        }
    }
}
