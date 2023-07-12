#ifndef _SCREEN_H
#define _SCREEN_H

extern uint8_t *scroll_area_screen_line_address[];
extern uint8_t *scroll_area_screen_line_end_address[];

void init_screen( void );
void redraw_scroll_area( void );
void enable_hidden_band_view( void );
void disable_hidden_band_view( void );

#define DEFAULT_ATTR			( PAPER_WHITE | INK_BLACK )
#define HIDDEN_BAND_VISIBLE_ATTR	( PAPER_WHITE | INK_BLACK | BRIGHT )
#define HIDDEN_BAND_INVISIBLE_ATTR	( PAPER_WHITE | INK_WHITE )
#define SCROLL_AREA_ATTR		( PAPER_YELLOW | INK_BLACK )

#endif // _SCREEN_H
