
#include <stdint.h>
#include <stdio.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <conio.h>

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

uint8_t ball_sprite_buffer[48]; // 8 bytes * 6 cells ( 4 sprite + 2 right blank col)

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
    uint8_t col, shift_index, n_rows, n_lines, shift_col,i;
    uint8_t *addr;

    // calculate which shift table to use
    shift_index = ( x % 8 ) / PRESHIFT_BITS;

    // column to draw on
    shift_col = x / 8;

    // start addr of col
    addr = offscreen_line_address[ y ] + shift_col;
    n_rows = s->rows;
    n_lines = n_rows * 8;

//    gotoxy( 0,21 );
//    printf("x: %-3d y:%-3d\n",x,y);
//    printf("shift_index: %d shift_col: %d addr: 0x%04x lines:%-2d\n",
//    	shift_index, shift_col,addr,n_lines );

    col = s->cols;
    // if shift index != 0 then we need to draw extra right column
    if ( shift_index ) col++;
    for ( i = 0; i < col; i++ ) {
        draw_sprite_column( (uint8_t *)&s->cells[ shift_index ][ i * n_rows ], &s->save_buffer[ i * n_lines ], addr + i, n_lines );
    }
}

void draw_sprite_column( uint8_t *sprite_data,
                        uint8_t *sprite_buffer,
                        uint8_t *framebuffer_addr,
                        uint16_t lines ) __smallc  __z88dk_callee {
    while ( lines-- ) {
    	// save framebuffer content to sprite buffer
    	*sprite_buffer = *framebuffer_addr;

    	// apply mask [0] and put graphics [1] to framebuffer
    	*framebuffer_addr = ( *framebuffer_addr & sprite_data[0] ) | sprite_data[1];

    	// adjust pointers
    	sprite_buffer++;			// next byte for buffer
    	framebuffer_addr += SCROLL_COLS;	// next line
    	sprite_data += 2;			// add 2 for mask and pixels
    }
}

void erase_sprite_column( uint8_t *sprite_buffer,
                        uint8_t *framebuffer_addr,
                        uint16_t lines ) __smallc  __z88dk_callee {
    while ( lines-- ) {
    	// apply mask [0] and put graphics [1] to framebuffer
    	*framebuffer_addr = *sprite_buffer++;
    	framebuffer_addr += SCROLL_COLS;	// next line
    }
}

void erase_sprite( struct sprite_graphic_s *s, uint8_t x, uint8_t y ) {
    uint8_t col, shift_index, n_rows, n_lines, shift_col, i;
    uint8_t *addr;

    // calculate which shift table to use
    shift_index = ( x % 8 ) / PRESHIFT_BITS;

    // column to draw on
    shift_col = x / 8;

    // start addr of col
    addr = offscreen_line_address[ y ] + shift_col;
    n_rows = s->rows;
    n_lines = n_rows * 8;

//    gotoxy( 0,21 );
//    printf("x: %-3d y:%-3d\n",x,y);
//    printf("shift_index: %d shift_col: %d addr: 0x%04x lines:%-2d\n",
//    	shift_index, shift_col,addr,n_lines );

    col = s->cols;
    // if shift index != 0 then we need to draw extra right column
    if ( shift_index ) col++;
    for ( i = 0; i < col; i++ ) {
        erase_sprite_column( &s->save_buffer[ i * n_lines ], addr + i, n_lines );
    }
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
            	// pixel shifting
            	// calculate the shifted value
            	shifted = s->cells[ 0 ][ ( col * n_rows ) + row ].lines[ line ].pixels >> ofs;
            	// if we are not on the first column, shift in the lower bits from the column on the left
            	if ( col )
		    shifted |= ( s->cells[ 0 ][ ( ( col - 1 ) * n_rows ) + row ].lines[ line ].pixels & ( 0xFF >> ( 8 - ofs ) ) ) << ( 8 - ofs );
		// if we are on the leftmost column, the new MSBs that enter are already 0, so no need to touch them
		// store shifted value
                s->cells[ ofs / PRESHIFT_BITS ][ ( col * n_rows ) + row ].lines[ line ].pixels = shifted;
                    
                // mask shifting
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
