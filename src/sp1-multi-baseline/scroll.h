#ifndef _SCROLL_H
#define _SCROLL_H

extern uint8_t *scroll_area_screen_line_address[];
extern uint8_t *scroll_area_screen_line_end_address[];

void init_screen_address_tables( void );
void init_sp1_tile_map( void );

void scroll_right_1pixel( void );
void scroll_right_2pixel( void );
void scroll_right_4pixel( void );

#endif // _SCROLL_H
