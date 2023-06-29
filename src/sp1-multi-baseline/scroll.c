#include <arch/zx/spectrum.h>
#include <sprites/sp1.h>

#include "build.h"

uint8_t *scroll_area_screen_line_address[ SCROLL_AREA_HEIGHT * 8 ];
uint8_t *scroll_area_screen_line_end_address[ SCROLL_AREA_HEIGHT * 8 ];

void init_screen_address_tables( void ) {
    uint8_t i, *addr;
    for ( i = 0; i < SCROLL_AREA_HEIGHT_LINES; i++ ) {
    	addr = zx_py2saddr( SCROLL_AREA_TOP * 8 + i ) + SCROLL_AREA_LEFT;
    	scroll_area_screen_line_address[ i ] = addr;
    	scroll_area_screen_line_end_address[ i ] = addr + SCROLL_AREA_WIDTH;
    }
}

void init_sp1_tile_map( void ) {
    uint8_t r,c;
    // setup the SP1 tile map to point to the cells in the offscreen buffer
    // the top rows are reserved for drawing and not visible
    // the tile array is configured in columns!
    for ( c = 0; c < SCROLL_AREA_WIDTH; c++ )
      for ( r = 0; r < SCROLL_AREA_HEIGHT; r++ )
        sp1_PrintAt( SCROLL_AREA_TOP + r, SCROLL_AREA_LEFT + c,          // screen position
          ( ( (r+c) % 2) ? PAPER_BLACK : PAPER_RED ) | INK_WHITE | BRIGHT,      // attr
          ( uint16_t ) offscreen_cell_address( SCROLL_MAP_TILE_HEIGHT + r, c ) );       // pointer
}

///////////////////////////////////////////////
// SCROLLING FUNCTIONS
///////////////////////////////////////////////

// scrolls the offscreen 1 pixel to the right

void scroll_right_1pixel( void ) __naked {
__asm

	ld hl,_offscreen			;; first screen line
	ld a,SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; number of lines

line_1px:
	ld b,SCROLL_AREA_EXTENDED_WIDTH

	or a	;; CF = 0
loop_line_1px:
	rr (hl)
	inc hl
	djnz loop_line_1px

	dec a
	jp nz, line_1px
	ret

__endasm;
}

void scroll_right_2pixel( void ) __naked {
__asm

	ld hl,_offscreen + SCROLL_AREA_EXTENDED_WIDTH - 1	;; first screen line
	ld a,SCROLL_AREA_EXTENDED_HEIGHT_LINES			;; number of lines

line_2px:
	ld b,SCROLL_AREA_EXTENDED_WIDTH

	ex af,af	;; save line counter

loop_line_2px:
	;; process the byte at (HL)

	ld de,hl	;; save pointer in DE for later

	ld c,(hl)	;; C = current byte
	dec hl
	ld a,(hl)	;; A = byte to its left

	;; repeat the following block twice for 2 pixel scroll; 3 times for 3 pixel, etc.
	rr a		;; take lowest bit of previous byte into CF
	rr c		;; and store it into highest bit of current byte, rotating

	rr a		;; repeat as needed
	rr c

	ld (de),c	;; store processed byte
			;; HL is already decremented for next iteration

	djnz loop_line_2px

	ld de,SCROLL_AREA_EXTENDED_WIDTH * 2 - 1	;; add for end of next line
	add hl,de

	ex af,af	;; continue with line counter
	dec a
	jp nz, line_2px
	ret

__endasm;
}

void scroll_right_4pixel( void ) __naked {
__asm

	ld hl,_offscreen				;; first screen line
	ld c,SCROLL_AREA_EXTENDED_HEIGHT_LINES		;; number of lines

line_4px:
	ld b,SCROLL_AREA_EXTENDED_WIDTH
	xor a
loop_line_4px:
	rrd		;; rrd (hl)
	inc hl
	djnz loop_line_4px

	dec c
	jp nz, line_4px
	ret

__endasm;
}

