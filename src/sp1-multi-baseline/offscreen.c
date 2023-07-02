#include <stdint.h>

#include "build.h"

// the main offscreen double buffer
#define OFFSCREEN_TOTAL_SIZE_BYTES	( SCROLL_AREA_EXTENDED_WIDTH * SCROLL_AREA_EXTENDED_HEIGHT_LINES )
uint8_t offscreen[ OFFSCREEN_TOTAL_SIZE_BYTES ];

// the offscreen[] is again stored in column-major form, for fast vertical scrolling
uint8_t *offscreen_cell_address( uint8_t row, uint8_t col ) {
    return &offscreen[ ( col * SCROLL_AREA_EXTENDED_HEIGHT + row ) * 8 ];
}

///////////////////////////////////////////////
// SCROLLING FUNCTIONS
///////////////////////////////////////////////

// scrolls the offscreen 1 pixel to the right
// this technique is done left to right
void offscreen_scroll_right_1px( void ) __naked {
__asm

	ld hl,_offscreen			;; first screen line
	ld a,SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; number of lines
	ld de,SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; needed for quick sum

line_1px:
	push hl					;; save line start address

	or a					;; C = 0
	ld b,SCROLL_AREA_EXTENDED_WIDTH		;; initialize line counter

loop_line_1px:
	rr (hl)					;; bring in C flag as MSB, then C = LSB
	jp nc, next_loop_no_carry		;; duplicate loop closing depending on C

next_loop_with_carry:
	add hl,de				;; modifies C flag
	scf					;; C = 1
	djnz loop_line_1px
	jp end_line_1px

next_loop_no_carry:
	add hl,de				;; modifies C flag
	or a					;; C = 0
	djnz loop_line_1px

end_line_1px:
	pop hl					;; restore start address
	inc hl					;; ...one line down

	dec a					;; iterate next line
	jp nz, line_1px

	ret

__endasm;
}

// scrolls the offscreen 2 pixel to the right
// this technique is done right to left
// this routine is slower than running 2 calls to offscreen_scroll_right_1px() in sequence (!)
void offscreen_scroll_right_2px( void ) __naked {
__asm

	ld hl,_offscreen + ( SCROLL_AREA_EXTENDED_WIDTH - 1 ) * SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; end of first screen line
	ld a,SCROLL_AREA_EXTENDED_HEIGHT_LINES			;; number of lines
	ld de,-SCROLL_AREA_EXTENDED_HEIGHT_LINES		;; bytes to substract for previous column
								;; needed for quick sum

line_2px:
	push af					;; save line counter
	push de					;; save quick sum

	ld b,SCROLL_AREA_EXTENDED_WIDTH

loop_line_2px:

	;; process byte at (HL)

	ld c,(hl)				;; C = current byte
	push hl					;; save ptr to current byte for later
	add hl,de				;; HL = ptr to left byte
	ld a,(hl)				;; A = left byte

	;; repeat the following block twice for 2 px scroll; 3 times for 3 px, etc.
	rr a					;; take lowest bit of previous byte into CF
	rr c					;; and store it into highest bit of current byte, rotating

	rr a					;; repeat as needed
	rr c

	ex (sp),hl				;; HL = ptr to current byte again
						;; and save ptr to left byte

	ld (hl),c				;; store processed byte
	pop hl					;; HL = ptr to left byte

	djnz loop_line_2px

	ld de, SCROLL_AREA_EXTENDED_WIDTH * SCROLL_AREA_EXTENDED_HEIGHT_LINES + 1	;; add for end of next line
	add hl,de

	pop de					;; restore quick sum
	pop af					;; restore line counter

	dec a					;; iterate next line
	jp nz, line_2px
	ret

__endasm;
}

void offscreen_scroll_right_4px( void ) __naked {
__asm

	ld hl,_offscreen			;; first screen line
	ld c,SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; number of lines
	ld de,SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; save for quick sum

line_4px:
	push hl					;; save start address

	ld b,SCROLL_AREA_EXTENDED_WIDTH
	xor a					;; A = 0

loop_line_4px:
	rrd		;; rrd (hl)
	add hl,de				;; next column
	djnz loop_line_4px

	pop hl					;; restore start address
	inc hl					;; ...one line down

	dec c					;; iterate next line
	jp nz, line_4px
	ret

__endasm;
}

