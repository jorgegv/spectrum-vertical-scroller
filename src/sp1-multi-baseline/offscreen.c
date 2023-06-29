#include <stdint.h>

#include "build.h"

// the main offscreen double buffer
#define OFFSCREEN_TOTAL_SIZE_BYTES	( SCROLL_AREA_EXTENDED_WIDTH * SCROLL_AREA_EXTENDED_HEIGHT * 8 )
uint8_t offscreen[ OFFSCREEN_TOTAL_SIZE_BYTES ];

// the offscreen[] is again stored in column-major form, for fast vertical scrolling
uint8_t *offscreen_cell_address( uint8_t row, uint8_t col ) {
    return &offscreen[ ( col * SCROLL_AREA_EXTENDED_HEIGHT + row ) * 8 ];
}

///////////////////////////////////////////////
// SCROLLING FUNCTIONS
///////////////////////////////////////////////

// scrolls the offscreen 1 pixel to the right

void offscreen_scroll_right_1pixel( void ) __naked {
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

void offscreen_scroll_right_2pixel( void ) __naked {
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

void offscreen_scroll_right_4pixel( void ) __naked {
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

