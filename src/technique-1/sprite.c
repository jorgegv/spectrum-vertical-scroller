#include <stdint.h>

#define SPRITE_HEIGHT		2
#define FRAMEBUFFER_WIDTH	16

uint8_t ball_sprite[] = {
    0xfc, 0x00, 0xf8, 0x03, 0xe0, 0x0f, 0xc0, 0x19, 0x80, 0x31, 0x80, 0x23, 0x00, 0x67, 0x00, 0x7f, 
    0x00, 0x7f, 0x00, 0x7f, 0x80, 0x3f, 0x80, 0x3f, 0xc0, 0x1f, 0xe0, 0x0f, 0xf8, 0x03, 0xfc, 0x00, 
    0x3f, 0x00, 0x1f, 0xc0, 0x07, 0xf0, 0x03, 0xf8, 0x01, 0xfc, 0x01, 0xfc, 0x00, 0xfe, 0x00, 0xfe, 
    0x00, 0xfe, 0x00, 0xfe, 0x01, 0xfc, 0x01, 0xfc, 0x03, 0xf8, 0x07, 0xf0, 0x1f, 0xc0, 0x3f, 0x00, 
};

uint8_t ball_sprite_buffer[32];

void draw_sprite_column( uint8_t *sprite_data,
                        uint8_t *sprite_buffer,
                        uint8_t *framebuffer_addr ) __smallc  __z88dk_callee __naked {
__asm


;; trashes all regs

	pop af				;; retaddr
	pop hl				;; HL: framebuffer addr
	pop de				;; DE: sprite buffer addr
	pop bc				;; BC: sprite data addr
	push af				;; restore retaddr

	;; initialize loop
	ld a, SPRITE_HEIGHT * 8

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
	add hl,FRAMEBUFFER_WIDTH	;; HL: next framebuffer line
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
                        uint8_t *framebuffer_addr ) __smallc  __z88dk_callee __naked {
__asm
;; trashes all regs
	pop bc				;; retaddr
	pop de				;; DE: framebuffer addr
	pop hl				;; HL: sprite buffer addr
	push bc				;; restore retaddr

	;; initialize loop
	ld b, SPRITE_HEIGHT * 8

loop_erase_line:
	;; When entering loop iteration:
	;; HL: sprite buffer addr
	;; DE: framebuffer addr

	ld a,(hl)
	ex de,hl	;; now: DE: sprite buffer addr, HL: frame buffer addr
	ld (hl),a

	;; loop
	add hl,FRAMEBUFFER_WIDTH	;; adjust framebuffer addr
	inc de				;; adjust sprite buffer addr
	ex de,hl			;; put them as needed again
	djnz loop_erase_line
	ret
__endasm;
}

