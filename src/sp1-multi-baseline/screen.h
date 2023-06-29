#ifndef _SCREEN_H
#define _SCREEN_H

extern uint8_t *scroll_area_screen_line_address[];
extern uint8_t *scroll_area_screen_line_end_address[];

void init_screen_address_tables( void );
void init_tile_map( void );
void invalidate_scroll_area( void );
void redraw_scroll_area( void );

#endif // _SCREEN_H
