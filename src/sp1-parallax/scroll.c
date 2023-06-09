#include <string.h>

#include "build.h"

///////////////////////////////////
//
// OFFSCREEN BUFFER DEFINITIONS
//
///////////////////////////////////

// The offscreen buffer. There are SCROLL_PIXELS_MAX top pixel lines on each column which are
// initialized to 0 so that when scrolling down the top non-visible tile row is erased
#define OFFSCREEN_BUFFER_SIZE	( SCROLL_AREA_WIDTH * SCROLL_PIXELS_MAX + SCROLL_AREA_WIDTH * SCROLL_AREA_REAL_HEIGHT * 8 )
uint8_t offscreen[ OFFSCREEN_BUFFER_SIZE ];

// returns address offset into offscreen for a cell at (row,col)
uint16_t cell_address_offset( uint8_t row, uint8_t col ) {
  return ( 8 * ( SCROLL_AREA_REAL_HEIGHT * col + row ) + SCROLL_PIXELS_MAX * ( col + 1 ) );
}

// Tables of start and end addresses for each column; initialized at startup
uint8_t *offscreen_column_start_address[ SCROLL_AREA_WIDTH ];
uint8_t *offscreen_column_end_address[ SCROLL_AREA_WIDTH ];

// initialize address tables
void init_address_tables( void ) {
    uint8_t c;
    for ( c = 0; c < SCROLL_AREA_WIDTH; c++ ) {
      offscreen_column_start_address[ c ] = &offscreen[ cell_address_offset( 0, c ) ];
      offscreen_column_end_address[ c ] = &offscreen[ cell_address_offset( 0, c ) + ( 8 * SCROLL_AREA_REAL_HEIGHT ) - 1 ];
    }
}
/////////////////////////
//
// SCROLLING FUNCTIONS
//
/////////////////////////

// SP1 scroll area
struct sp1_Rect scroll_area = { SCROLL_AREA_POS_ROW, SCROLL_AREA_POS_COL, SCROLL_AREA_WIDTH, SCROLL_AREA_HEIGHT };

// global scroll counter
uint8_t scroll_counter_1;
uint8_t scroll_counter_2;
uint8_t scroll_counter_3;

/*
// function to scroll a column SCROLL_PIXELS_MAX pixels down - loop version
// the number of DEC HL instructions at the top must be SCROLL_PIXELS
void scroll_down_column_loop( uint8_t *data ) __z88dk_fastcall __naked {
__asm
    ld de,hl

REPT SCROLL_PIXELS
    dec hl
ENDR

    ld bc,SCROLL_AREA_REAL_HEIGHT*8
    lddr
    ret
__endasm;
}

// function to scroll a column SCROLL_PIXELS pixels down - stack version
// the number of transfer loops must be exactly SCROLL_AREA_REAL_HEIGHT
// must be called with INTs disabled!
uint16_t saved_sp;
void scroll_down_column_stack( uint8_t *column_end ) __z88dk_fastcall __naked {
__asm

    ;; we will be using HL as the dst ptr and HL' as the src ptr
    inc hl	;; stack is predec/postinc
    push hl	;; HL already contains the final column address (dst address)
    exx
    pop	hl
    ld de,- ( 8 + SCROLL_PIXELS	)
    add hl,de

    ld (_saved_sp),sp	;; save stack pointer

    ;; we enter the loop with alt regs
REPT SCROLL_AREA_REAL_HEIGHT
    ld sp,hl	;; load src address in SP (from HL')
    pop de	;; load 8 bytes from source
    pop bc
    exx
    pop de
    pop bc

    ld sp, hl	;; load dst address in SP (from HL)
    push bc	;; store 8 bytes ro dest
    push de

    ld de,-8	;; update dst for next iter
    add hl,de

    exx		;; back to alt regs
    push bc
    push de

    ld de,-8	;; update src for next iter
    add hl,de
ENDR
    exx		;; back to normal regs
    ld sp, (_saved_sp)	;; restore stack pointer
    ret
__endasm;
}

*/
// function to scroll down a column a given number of pixels - unrolled version
// the number of LDDs must be exactly SCROLL_AREA_REAL_HEIGHT * 8
void scroll_down_column_unrolled( uint8_t *column_end, uint8_t num_pixels ) __smallc __z88dk_callee __naked {
__asm
    pop af      ;; save retaddr
    pop bc      ;; C = num_pixels
    pop hl
    push af     ;; restore retaddr

    ld de,hl

    ld b,c      ;; B = num_pixels
scroll_adjust_loop:
    dec hl
    djnz scroll_adjust_loop

REPT SCROLL_AREA_REAL_HEIGHT*8
    ldd
ENDR

    ret
__endasm;
}

// scroll the whole scroll area one pixel down
void scroll_down_area( void ) {
  uint8_t i;

  // scroll zone 1 (AREA_1)
  for ( i = 2; i < SCROLL_AREA_WIDTH-2; i++ )
    scroll_down_column_unrolled( offscreen_column_end_address[ i ], SCROLL_AREA1_PIXELS );
  scroll_counter_1 += SCROLL_AREA1_PIXELS;

  // scroll zone 2 (AREA_2)
  scroll_down_column_unrolled( offscreen_column_end_address[ 1 ], SCROLL_AREA2_PIXELS );
  scroll_down_column_unrolled( offscreen_column_end_address[ SCROLL_AREA_WIDTH-2 ], SCROLL_AREA2_PIXELS );
  scroll_counter_2 += SCROLL_AREA2_PIXELS;

  // scroll zone 3 (AREA_3)
  scroll_down_column_unrolled( offscreen_column_end_address[ 0 ], SCROLL_AREA3_PIXELS );
  scroll_down_column_unrolled( offscreen_column_end_address[ SCROLL_AREA_WIDTH-1 ], SCROLL_AREA3_PIXELS );
  scroll_counter_3 += SCROLL_AREA3_PIXELS;
}

// initialize scroll areas
void init_scroll_area( void ) {
  memset( offscreen, 0, OFFSCREEN_BUFFER_SIZE );
  scroll_counter_1 = 0;
  scroll_counter_2 = 0;
  scroll_counter_3 = 0;
}

