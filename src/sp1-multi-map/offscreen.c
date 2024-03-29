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

// internal function prototypes
void offscreen_scroll_right_1px( void );
void offscreen_scroll_right_2px( void );
void offscreen_scroll_right_4px( void );
void offscreen_scroll_right_8px( void );
void offscreen_scroll_right_Npx( uint16_t num_pix ) __z88dk_callee;
void offscreen_scroll_left_1px( void );
void offscreen_scroll_left_2px( void );
void offscreen_scroll_left_4px( void );
void offscreen_scroll_left_8px( void );
void offscreen_scroll_left_Npx( uint16_t num_pix ) __z88dk_callee;
void offscreen_scroll_up_Npx( uint16_t num_pix ) __z88dk_callee;
void offscreen_scroll_down_Npx( uint16_t num_pix ) __z88dk_callee;

///////////////////////////
// Driver functions
///////////////////////////

void offscreen_scroll_right_pixels( uint16_t num_pix ) {
    if ( num_pix & 0x01 )
    	offscreen_scroll_right_1px();
    if ( num_pix & 0x02 )
    	offscreen_scroll_right_Npx( 2 );
    if ( num_pix & 0x04 )
    	offscreen_scroll_right_4px();
    if ( num_pix & 0x08 )
    	offscreen_scroll_right_8px();
}

void offscreen_scroll_left_pixels( uint16_t num_pix ) {
    if ( num_pix & 0x01 )
    	offscreen_scroll_left_1px();
    if ( num_pix & 0x02 )
    	offscreen_scroll_left_Npx( 2 );
    if ( num_pix & 0x04 )
    	offscreen_scroll_left_4px();
    if ( num_pix & 0x08 )
    	offscreen_scroll_left_8px();
}

void offscreen_scroll_up_pixels( uint16_t num_pix ) {
    offscreen_scroll_up_Npx( num_pix );
}

void offscreen_scroll_down_pixels( uint16_t num_pix ) {
    offscreen_scroll_down_Npx( num_pix );
}

//////////////////
// SCROLL RIGHT //
//////////////////

// scroll the offscreen 1 pixel to the right
// this technique is done left to right
void offscreen_scroll_right_1px( void ) __naked {
__asm

	ld hl,_offscreen			;; first screen line
	ld a,SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; number of lines
	ld de,SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; needed for quick sum

line_1px:
	ld bc,hl				;; save line start address

	or a					;; C = 0

REPT SCROLL_AREA_EXTENDED_WIDTH
	rr (hl)					;; bring in C flag as MSB, then C = LSB
	ex af,af
	add hl,de
	ex af,af
ENDR

	ld hl,bc				;; restore start address
	inc hl					;; ...one line down

	dec a					;; iterate next line
	jp nz, line_1px

	ret

__endasm;
}

// scroll the offscreen 2 pixels to the right
// this technique is done right to left
// this routine is slower than running 2 calls to offscreen_scroll_right_1px() in sequence (!)
// it also introduces some garbage on the left, but it assumed that the left edge is redrawn
// periodically, and before the garbage enters the visible area
void offscreen_scroll_right_2px( void ) __naked {
__asm

	ld hl,_offscreen + OFFSCREEN_TOP_LINE_END_OFFSET	;; end of first screen line
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

// scroll the offscreen 4 pixels to the right
// this technique uses rrd instruction to shift 4 bits through the
// accumulator in a single instruction
void offscreen_scroll_right_4px( void ) __naked {
__asm

	ld hl,_offscreen			;; first screen line
	ld a,SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; number of lines
	ld de,SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; save for quick sum

line_4px:
	ld bc,hl				;; save start address

	ex af,af				;; save line counter
	xor a					;; A = 0

REPT SCROLL_AREA_EXTENDED_WIDTH
	rrd		;; rrd (hl)
	add hl,de				;; next column
ENDR

	ld hl,bc				;; restore start address
	inc hl					;; ...one line down

	ex af,af				;; restore line counter
	dec a
	jp nz,line_4px				;; iterate next line
	ret

__endasm;
}

// scroll the offscreen 8 pixels (1 column) to the right
// this technique is done right to left, bottom to top
// we scroll a full column on each iteration
// it also introduces some garbage on the left, but it assumed that the left edge is redrawn
// periodically, and before the garbage enters the visible area
void offscreen_scroll_right_8px( void ) __naked {
__asm

	;; HL = src, DE = dst

        ld de,_offscreen + OFFSCREEN_BOTTOM_LINE_END_OFFSET	;; bottom of last col
        ld hl,_offscreen + OFFSCREEN_BOTTOM_LINE_END_OFFSET - SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; bottom of previous col

        ld a, SCROLL_AREA_EXTENDED_WIDTH		;; number of columns

column_8px:
        push hl						;; save next col top addr

REPT SCROLL_AREA_EXTENDED_HEIGHT_LINES
	ldd						;; mods BC,DE,HL
ENDR

	;; now HL points to the top of the current column
	pop de						;; DE = next col top addr (saved above)

	dec a						;; iterate column counter
	jp nz, column_8px

	ret
	
__endasm;
}


// scroll the offscreen N pixels to the right
// works the same as for 1px scroll, but repeating N times on each line
void offscreen_scroll_right_Npx( uint16_t num_pix ) __naked __z88dk_callee {
__asm

	pop hl					;; save retaddr
	pop bc					;; C = num_pix (param)
	push hl					;; push retaddr again

	ld hl,_offscreen			;; first screen line
	ld a,SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; number of lines
	ld de,SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; needed for quick sum

pre_n_line_1px:
	push bc					;; save C (num_pix) for later

n_line_1px:
	push hl					;; save line start address

	or a					;; CF = 0

REPT SCROLL_AREA_EXTENDED_WIDTH
	rr (hl)					;; bring in CF flag as MSB, then CF = LSB
	ex af,af
	add hl,de
	ex af,af
ENDR

	pop hl					;; restore start address

	dec c					;; iterate number of scrolled pixels
	jp nz, n_line_1px

	pop bc					;; if finished with this line, restore C = num_pix

	inc hl					;; ...one line down
	dec a					;; iterate next line
	jp nz, pre_n_line_1px

	ret

__endasm;
}

//////////////////
// SCROLL LEFT  //
//////////////////

// scroll the offscreen 1 pixel to the left
// this technique is done right to left
void offscreen_scroll_left_1px( void ) __naked {
__asm

	ld hl,_offscreen + OFFSCREEN_TOP_LINE_END_OFFSET	;; top of last col
	ld a,SCROLL_AREA_EXTENDED_HEIGHT_LINES			;; number of lines
	ld de,-SCROLL_AREA_EXTENDED_HEIGHT_LINES		;; needed for quick sum

line_1px_left:
	ld bc,hl				;; save line start address

	or a					;; C = 0

REPT SCROLL_AREA_EXTENDED_WIDTH
        rl (hl)                                 ;; bring in C flag as MSB, then C = LSB
        ex af,af
        add hl,de
        ex af,af
ENDR

	ld hl,bc				;; restore start address
	inc hl					;; ...one line down

	dec a					;; iterate next line
	jp nz, line_1px_left

	ret

__endasm;
}

// scroll the offscreen 2 pixels to the left
// this technique is done left to right
// this routine is slower than running 2 calls to offscreen_scroll_left_1px() in sequence (!)
// it also introduces some garbage on the right, but it assumed that the right edge is redrawn
// periodically, and before the garbage enters the visible area
void offscreen_scroll_left_2px( void ) __naked {
__asm

	ld hl,_offscreen					;; start of first screen line
	ld a,SCROLL_AREA_EXTENDED_HEIGHT_LINES			;; number of lines
	ld de,SCROLL_AREA_EXTENDED_HEIGHT_LINES			;; bytes to add for next column
								;; needed for quick sum

line_2px_left:
	push af					;; save line counter
	push de					;; save quick sum

	ld b,SCROLL_AREA_EXTENDED_WIDTH

loop_line_2px_left:

	;; process byte at (HL)

	ld c,(hl)				;; C = current byte
	push hl					;; save ptr to current byte for later
	add hl,de				;; HL = ptr to right byte
	ld a,(hl)				;; A = right byte

	;; repeat the following block twice for 2 px scroll; 3 times for 3 px, etc.
	rl a					;; take MSB of next byte into CF
	rl c					;; and store it into LSB of current byte, rotating

	rl a					;; repeat as needed
	rl c

	ex (sp),hl				;; HL = ptr to current byte again
						;; and save ptr to right byte

	ld (hl),c				;; store processed byte
	pop hl					;; HL = ptr to left byte

	djnz loop_line_2px_left

	ld de,1 - SCROLL_AREA_EXTENDED_WIDTH * SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; add for start of next line
	add hl,de

	pop de					;; restore quick sum
	pop af					;; restore line counter

	dec a					;; iterate next line
	jp nz, line_2px_left
	ret

__endasm;
}

// scroll the offscreen 4 pixels to the left
// this technique uses rld instruction to shift 4 bits through the
// accumulator in a single instruction
void offscreen_scroll_left_4px( void ) __naked {
__asm

	ld hl,_offscreen + OFFSCREEN_TOP_LINE_END_OFFSET	;; top of last col
	ld a,SCROLL_AREA_EXTENDED_HEIGHT_LINES			;; number of lines
	ld de,-SCROLL_AREA_EXTENDED_HEIGHT_LINES		;; save for quick sum

line_4px_left:
	ld bc,hl				;; save start address

	ex af,af				;; save line counter
	xor a					;; A = 0

REPT SCROLL_AREA_EXTENDED_WIDTH
	rld		;; rld (hl)
	add hl,de				;; next column
ENDR

	ld hl,bc				;; restore start address
	inc hl					;; ...one line down

	ex af,af				;; restore line counter
	dec a
	jp nz,line_4px_left			;; iterate next line
	ret

__endasm;
}

// scroll the offscreen 8 pixels (1 column) to the left
// this technique is done left to right
// we scroll a full column on each iteration
// it also introduces some garbage on the right, but it assumed that the right edge is redrawn
// periodically, and before the garbage enters the visible area
void offscreen_scroll_left_8px( void ) __naked {
__asm

	;; HL = src, DE = dst

        ld de,_offscreen					;; top of first col
        ld hl,_offscreen + SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; top of next col

        ld a, SCROLL_AREA_EXTENDED_WIDTH		;; number of columns

column_8px_left:
        push hl						;; save next col top addr

REPT SCROLL_AREA_EXTENDED_HEIGHT_LINES
	ldi						;; mods BC,DE,HL
ENDR

	;; now HL points to the top of the current column
	pop de						;; DE = next col top addr (saved above)

	dec a						;; iterate column counter
	jp nz, column_8px_left

	ret
	
__endasm;
}

// scroll the offscreen N pixels to the left
// works the same as for 1px scroll, but repeating N times on each line
void offscreen_scroll_left_Npx( uint16_t num_pix ) __naked __z88dk_callee {
__asm

	pop hl							;; save retaddr
	pop bc							;; C = num_pix (param)
	push hl							;; push retaddr again

	ld hl,_offscreen + OFFSCREEN_TOP_LINE_END_OFFSET	;; first screen line
	ld a,SCROLL_AREA_EXTENDED_HEIGHT_LINES			;; number of lines
	ld de,-SCROLL_AREA_EXTENDED_HEIGHT_LINES		;; needed for quick sum

pre_n_line_1px_left:
	push bc						;; save C (num_pix) for later

n_line_1px_left:
	push hl						;; save line start address

	or a						;; CF = 0

REPT SCROLL_AREA_EXTENDED_WIDTH
	rl (hl)						;; bring in CF flag as MSB, then CF = LSB
	ex af,af
	add hl,de
	ex af,af
ENDR

	pop hl						;; restore start address

	dec c						;; iterate number of scrolled pixels
	jp nz, n_line_1px_left

	pop bc						;; if finished with this line, restore C = num_pix

	inc hl						;; ...one line down
	dec a						;; iterate next line
	jp nz, pre_n_line_1px_left

	ret

__endasm;
}

/////////////////////
// SCROLL UP/DOWN  //
/////////////////////

// scroll the offscreen N pixels down
// works bottom to up, right to left
void offscreen_scroll_down_Npx( uint16_t num_pix ) __naked __z88dk_callee {
__asm
    pop hl      ;; save retaddr
    pop bc      ;; BC = num_pix
    push hl     ;; restore retaddr

    ld hl,_offscreen + OFFSCREEN_BOTTOM_LINE_END_OFFSET	;; HL = bottom line right (dst)
    ld de,hl						;; save dst addr

    ld b,0xff						;; BC = negative offset, num_pix lines above
    ld a,c						;; we can do this because C is always <= 8
    neg
    ld c,a
    add hl,bc    					;; HL = num_pix lines before (src)

    ld a,SCROLL_AREA_EXTENDED_WIDTH			;; column iterator

loop_down_column:

REPT SCROLL_AREA_EXTENDED_HEIGHT_LINES
    ldd							;; transfer bytes
ENDR

    ;; with this setup, HL and DE are already set for prev column

    dec a						;; iterate column loop
    jp nz, loop_down_column

    ret
__endasm;
}

// scroll the offscreen N pixels up
// works up to bottom, left to right
void offscreen_scroll_up_Npx( uint16_t num_pix ) __naked __z88dk_callee {
__asm
    pop hl      ;; save retaddr
    pop bc      ;; BC = num_pix
    push hl     ;; restore retaddr

    ld hl,_offscreen					;; HL = top line left (dst)
    ld de,hl						;; save dst addr
    add hl,bc    					;; HL = num_pix lines below (src)

    ld a,SCROLL_AREA_EXTENDED_WIDTH			;; column iterator

loop_up_column:

REPT SCROLL_AREA_EXTENDED_HEIGHT_LINES
    ldi							;; transfer bytes
ENDR

    ;; with this setup, HL and DE are already set for next column

    dec a						;; iterate column loop
    jp nz, loop_up_column

    ret
__endasm;
}

//////////////////////////////////
// Specialized scroll functions
//////////////////////////////////

// scrolls right a horizontal row of tiles N pixels (used to scroll the hidden borders)
void offscreen_scroll_left_Npx_tile_row( void *start, uint16_t num_pix ) __naked __z88dk_callee __smallc {
__asm

	pop de					;; save retaddr
	pop bc					;; C = num_pix (param)
	pop hl					;; HL = start addr of first line
	push de					;; push retaddr again

	ld de,( SCROLL_AREA_EXTENDED_WIDTH - 1 ) * SCROLL_AREA_EXTENDED_HEIGHT_PIX
	add hl,de					;; right-most byte of the top line
	ld a,SCROLL_MAP_TILE_HEIGHT_PIX			;; number of lines
	ld de,-SCROLL_AREA_EXTENDED_HEIGHT_LINES	;; needed for quick sum

pre_n_line_1px_tr:
	push bc					;; save C (num_pix) for later

n_line_1px_tr:
	push hl					;; save line start address

	or a					;; CF = 0

REPT SCROLL_AREA_EXTENDED_WIDTH
	rl (hl)					;; bring in CF flag as MSB, then CF = LSB
	ex af,af				;; save CF
	add hl,de
	ex af,af				;; restore CF
ENDR

	pop hl					;; restore start address

	dec c					;; iterate number of scrolled pixels
	jp nz, n_line_1px_tr

	pop bc					;; if finished with this line, restore C = num_pix

	inc hl					;; ...one line down

	dec a					;; iterate next line
	jp nz, pre_n_line_1px_tr

	ret

__endasm;
}

void offscreen_scroll_left_8px_tile_row( void *start, uint16_t num_pix ) __naked __smallc __z88dk_callee {
__asm

	pop hl		;; save ret addr
	pop bc		;; C = num_pix
	pop de		;; DE = start addr
	push hl		;; push ret addr again

	;; HL = src, DE = dst

	push de
	ld hl,de
        ld de,SCROLL_AREA_EXTENDED_HEIGHT_LINES
        add hl,de
        pop de

        ld a, SCROLL_AREA_EXTENDED_WIDTH	;; number of columns

column_8px_left_tr:
        push hl					;; save next col top addr
        push hl

REPT SCROLL_MAP_TILE_HEIGHT_PIX
	ldi					;; mods BC,DE,HL
ENDR

	pop hl					;; HL = next col top addr
	ld de, SCROLL_AREA_EXTENDED_HEIGHT_LINES
	add hl,de				;; skip to next-next col
	pop de					;; DE = next col addr
	
	dec a						;; iterate column counter
	jp nz, column_8px_left_tr

	ret
	
__endasm;
}

void offscreen_scroll_left_tile_row( void *start, uint16_t num_pix ) __smallc __z88dk_callee {
    while ( num_pix > 8 ) {
    	offscreen_scroll_left_8px_tile_row( start, num_pix );
    	num_pix -= 8;
    }
    offscreen_scroll_left_Npx_tile_row( start, num_pix );
}

// scrolls down a vertical column of tiles N pixels (used to scroll the hidden borders)
void offscreen_scroll_up_tile_col( void *start, uint16_t num_pix ) __naked __smallc __z88dk_callee {
__asm
	
    pop de      ;; save retaddr
    pop bc      ;; C = num_pix
    pop hl	;; HL = start address
    push de     ;; restore retaddr

    ld de,hl						;; save dst addr
    add hl,bc    					;; HL = num_pix lines before (src)

    ld a,SCROLL_MAP_TILE_WIDTH				;; column iterator

loop_down_column_tc:

REPT SCROLL_AREA_EXTENDED_HEIGHT_LINES
    ldi							;; transfer bytes
ENDR

    ;; with this setup, HL and DE are already set for prev column

    dec a						;; iterate column loop
    jp nz, loop_down_column_tc

    ret
__endasm;
}
