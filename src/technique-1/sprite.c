
#include <stdint.h>
#include <stdio.h>
#include <intrinsic.h>
#include <stdlib.h>

#include "offscreen.h"
#include "sprite.h"

#define SPRITE_HEIGHT		2

struct  sprite_cell_s ball_sprite_cells[] = {
    { .lines = { { 0xfc, 0x00 }, { 0xf8, 0x03 }, { 0xe0, 0x0f }, { 0xc0, 0x19 }, { 0x80, 0x31 }, { 0x80, 0x23 }, { 0x00, 0x67 }, { 0x00, 0x7f } } },
    { .lines = { { 0x00, 0x7f }, { 0x00, 0x7f }, { 0x80, 0x3f }, { 0x80, 0x3f }, { 0xc0, 0x1f }, { 0xe0, 0x0f }, { 0xf8, 0x03 }, { 0xfc, 0x00 } } },
    { .lines = { { 0x3f, 0x00 }, { 0x1f, 0xc0 }, { 0x07, 0xf0 }, { 0x03, 0xf8 }, { 0x01, 0xfc }, { 0x01, 0xfc }, { 0x00, 0xfe }, { 0x00, 0xfe } } },
    { .lines = { { 0x00, 0xfe }, { 0x00, 0xfe }, { 0x01, 0xfc }, { 0x01, 0xfc }, { 0x03, 0xf8 }, { 0x07, 0xf0 }, { 0x1f, 0xc0 }, { 0x3f, 0x00 } } },
    { .lines = { { 0xff, 0x00 }, { 0xff, 0x00 }, { 0xff, 0x00 }, { 0xff, 0x00 }, { 0xff, 0x00 }, { 0xff, 0x00 }, { 0xff, 0x00 }, { 0xff, 0x00 } } },
    { .lines = { { 0xff, 0x00 }, { 0xff, 0x00 }, { 0xff, 0x00 }, { 0xff, 0x00 }, { 0xff, 0x00 }, { 0xff, 0x00 }, { 0xff, 0x00 }, { 0xff, 0x00 } } },

    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },
    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },
    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },
    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },
    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },
    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },

    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },
    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },
    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },
    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },
    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },
    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },

    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },
    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },
    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },
    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },
    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },
    { .lines = { { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } } },

};

uint8_t ball_sprite_buffer[48];

struct sprite_graphic_s ball_sprite = {
    .rows		= 2,
    .cols		= 2,
    .save_buffer	= &ball_sprite_buffer[0],
    .cells		= {
    	&ball_sprite_cells[0],
    	&ball_sprite_cells[6],
    	&ball_sprite_cells[12],
    	&ball_sprite_cells[18],
    },
};

void draw_sprite( struct sprite_graphic_s *s, uint8_t x, uint8_t y ) {
    uint8_t col, shift_index, n_rows, n_lines, shift_col;
    uint8_t *addr;

    shift_index = x % PRESHIFT_BITS_MASK;
    shift_col = x >> PRESHIFT_BITS;
    addr = offscreen_line_address[ y ] + shift_col;
    n_rows = s->rows;
    n_lines = n_rows * 8;

    col = s->cols;
    while( col-- ) {
        draw_sprite_column( (uint8_t *)&s->cells[ shift_index ][ col * n_rows ], s->save_buffer, addr + col, n_lines );
    }
}

void draw_sprite_column( uint8_t *sprite_data,
                        uint8_t *sprite_buffer,
                        uint8_t *framebuffer_addr,
                        uint16_t lines ) __smallc  __z88dk_callee __naked {
__asm


;; trashes all regs

	exx
	pop bc				;; retaddr
	exx

	pop bc
	ld a,c
	pop hl				;; HL: framebuffer addr
	pop de				;; DE: sprite buffer addr
	pop bc				;; BC: sprite data addr

	exx
	push bc				;; restore retaddr
	exx

	;; A: number of lines

loop_draw_line:
	;; When entering loop iteration:
	;; A: current loop counter
	;; HL: current framebuffer addr
	;; DE: current sprite buffer addr
	;; BC: current sprite data addr

	push af				;; save the line counter

	;; save the background
	ld a,(hl)
	ex de,hl
	ld (hl),a			;; now:
					;; DE: framebuffer addr
					;; HL: sprite buffer addr
					;; A: original framebuffer data

	;; draw sprite pixels
	push hl				;; save sprite buffer addr

	ld hl,bc			;; HL: sprite data addr
					;; DE: framebuffer addr
					;; BC: sprite data addr
	ld b,(hl)			;; B: mask data
	inc hl
	ld c,(hl)			;; C: pixel data
					;; HL: sprite data addr + 1
					
					;; A already contains original framebuffer data
	and b				;; apply mask to screen data
	or c				;; put the sprite pixels

	ex de,hl			;; HL: framebuffer addr
					;; DE: sprite data addr + 1
	ld (hl),a			;; save to framebuffer

	ld bc,de			;; restore BC: sprite data addr
	pop de				;; restore DE: sprite buffer addr + 1

	;; adjust for next iteration
	add hl,SCROLL_COLS	;; HL: next framebuffer line
	inc bc				;; BC: next sprite data line
					;; it was already +1, so +1 more
	inc de				;; DE: next sprite buffer line

	;; loop
	pop af
	dec a
	jp nz,loop_draw_line
	ret
__endasm;
}

void erase_sprite_column( uint8_t *sprite_buffer,
                        uint8_t *framebuffer_addr,
                        uint16_t lines ) __smallc  __z88dk_callee __naked {
__asm
;; trashes all regs

	pop af				;; retaddr
	pop bc				;; B: number of lines
	pop de				;; DE: framebuffer addr
	pop hl				;; HL: sprite buffer addr
	push af				;; restore retaddr

	;; B: number of lines

loop_erase_line:
	;; When entering loop iteration:
	;; HL: sprite buffer addr
	;; DE: framebuffer addr

	ld a,(hl)
	ex de,hl	;; now: DE: sprite buffer addr, HL: frame buffer addr
	ld (hl),a

	;; loop
	add hl,SCROLL_COLS	;; adjust framebuffer addr
	inc de				;; adjust sprite buffer addr
	ex de,hl			;; put them as needed again

	djnz loop_erase_line
	ret
__endasm;
}


// generates the shifted version of the sprite supplied at data[0]
// ofs can be 1-7 - Valid values depend on PRESHIFT_BITS
void generate_shifted_sprite( struct sprite_graphic_s *s, uint8_t ofs ) {
    uint8_t row,col,line,n_rows,shifted;

    n_rows = s->rows;

    col = s->cols + 1;
    while ( col-- ) {
        row = s->rows;
        while ( row-- ) {
            line = 8;
            while ( line-- ) {
            	// calculate the shifted value
            	shifted = s->cells[ 0 ][ ( col * n_rows ) + row ].lines[ line ].pixels >> ofs;
            	// if we are not on the first column, shift in the lower bits from the column on the left
            	if ( col )
		    shifted |= ( s->cells[ 0 ][ ( ( col - 1 ) * n_rows ) + row ].lines[ line ].pixels & ( 0xFF >> ( 8 - ofs ) ) ) << ( 8 - ofs );
		// if we are on the leftmost column, the new MSBs that enter are already 0, so no need to touch them

		// store shifted value
                s->cells[ ofs / PRESHIFT_BITS ][ ( col * n_rows ) + row ].lines[ line ].pixels = shifted;
                    
            	// calculate the shifted value
		shifted = ~( ( ~s->cells[ 0 ][ ( col * n_rows ) + row ].lines[ line ].mask ) >> ofs );
            	// if we are not on the first column, shift in the lower bits from the column on the left
		if ( col )
		    shifted |= ( s->cells[ 0 ][ ( ( col - 1 ) * n_rows ) + row ].lines[ line ].mask & ( 0xFF >> ( 8 - ofs ) ) ) << ( 8 - ofs );
		else
		    // if we are on the leftmost column, the new MSBs that enter are 0, but they needs to be 1 so flip them
		    shifted |= ( 0xFF >> ( 8 - ofs ) ) << ( 8 - ofs );

		// store shifted value
                s->cells[ ofs / PRESHIFT_BITS ][ ( col * n_rows ) + row ].lines[ line ].mask = shifted;
            }
        }
    }
}

char *byte2bin( uint8_t b, char *buf ) {
	uint8_t i;
	for (i=0; i<8; i++ ) buf[i]='-';
	buf[8]='\0';
	for (i=0; i<8; i++ )
	    if ( b & ( 1 << i ) )
	        buf[7-i]='#';
	return buf;
}

unsigned char buf_pix[32];
unsigned char buf_mask[32];
unsigned char buf_pix2[32];
unsigned char buf_mask2[32];
void dump_sprite( struct sprite_graphic_s *s ) {
	uint8_t i;
	zx_cls();
	puts( "Normal:" );
	for ( i=0; i<8; i++ ) {
		printf("  m:%8s %8s", byte2bin(s->cells[0][0].lines[i].mask,buf_mask), byte2bin(s->cells[0][2].lines[i].mask,buf_mask2) );
		printf("  p:%8s %8s\n", byte2bin(s->cells[0][0].lines[i].pixels,buf_pix), byte2bin(s->cells[0][2].lines[i].pixels,buf_pix2) );
	}
	puts( "Shifted:" );
	for ( i=0; i<8; i++ ) {
		printf("  m:%8s %8s", byte2bin(s->cells[2][0].lines[i].mask,buf_mask), byte2bin(s->cells[2][2].lines[i].mask,buf_mask2) );
		printf("  p:%8s %8s\n", byte2bin(s->cells[2][0].lines[i].pixels,buf_pix), byte2bin(s->cells[2][2].lines[i].pixels,buf_pix2) );
	}
	intrinsic_di();
	intrinsic_halt();
}
