#ifndef _SPRITE_H
#define _SPRITE_H

#include <stdint.h>

#define PRESHIFT_BITS		2
#define PRESHIFTED_IMAGES	( 8 / PRESHIFT_BITS )

struct sprite_cell_line_s {
    uint8_t mask;
    uint8_t pixels;
};

struct sprite_cell_s {
    struct sprite_cell_line_s lines[ 8 ];	// sprite data for a 8x8 cell, mask+pixels
};

// structure for sprite graphics, preshifted to all positions ( ...cells[8] )
struct sprite_graphic_s {
    uint8_t rows;
    uint8_t cols;			// real dimensions of the sprite in cells
    uint8_t *save_buffer;		// must point to an array of rows * (cols + 1) cells
    struct sprite_cell_s *cells[ PRESHIFTED_IMAGES ];	// each must point to an array of rows * (cols + 1) cells
};

// generates the shifted version of the sprite supplied at *cells[0]
// ofs can be 1-7
void generate_shifted_sprite( struct sprite_graphic_s *s, uint8_t ofs );

void draw_sprite_column( uint8_t *sprite_data, uint8_t *sprite_buffer, uint8_t *framebuffer_addr ) __smallc  __z88dk_callee;
void erase_sprite_column( uint8_t *sprite_buffer, uint8_t *framebuffer_addr ) __smallc  __z88dk_callee;

//extern uint8_t ball_sprite[];
//extern struct  sprite_cell_s ball_sprite_data[];
//extern uint8_t ball_sprite_buffer[];
extern struct sprite_graphic_s ball_sprite;

#endif // _SPRITE_H
