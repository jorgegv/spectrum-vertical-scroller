#ifndef _TILE_H
#define _TILE_H

#include <stdint.h>

struct tile_s {
    uint8_t width;
    uint8_t height;
    uint8_t *data;
};

void tile_draw_offscreen( uint8_t row, uint8_t col, struct tile_s *t );
void tile_draw_offscreen_cell( uint8_t *cell_data, uint8_t row, uint8_t col );

#endif // _TILE_H
