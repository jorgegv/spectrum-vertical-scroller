#ifndef _SCROLL_MAP_H
#define _SCROLL_MAP_H

#include <stdint.h>

void init_scroll_map( void );
void scroll_map_set_viewport( uint8_t row, uint8_t col );
struct tile_s *scroll_map_get_tile_at( uint8_t row, uint8_t col );

#endif // _SCROLL_MAP_H
