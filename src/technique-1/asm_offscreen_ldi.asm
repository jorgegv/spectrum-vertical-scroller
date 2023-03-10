;; transfer frame from offscreen buffer to real screen

;; these constants must match those in offscreen.h
SCROLL_LINES	equ 128
SCROLL_COLS	equ 16

EXTERN	_offscreen_extra_line_address	;; table of line initial addresses (src)
EXTERN	_screen_line_address		;; table of line initial addresses (dst)
EXTERN	_current_scroll_offset_line	;; 0..15

PUBLIC	_asm_offscreen_show_frame_ldi
_asm_offscreen_show_frame_ldi:

	;; DE = offscreen_extra_line_address[ current_scroll_offset_line ]
	ld a,(_current_scroll_offset_line)
	add a,a
	ld l,a
	ld h,0		;; HL = offset into LUT
	ld de,_offscreen_extra_line_address
	add hl,de	;; HL = address of the LUT element
	ld c,(hl)
	inc hl
	ld b,(hl)
	ld hl,bc	;; HL = initial address of first src line

	ld a,0		;; line loop counter

loop1:
	push hl

	;; get initial address of destination line from LUT
	ld l,a
	ld h,0
	add hl,hl
	ld de,_screen_line_address
	add hl,de
	ld e,(hl)
	inc hl
	ld d,(hl)	;; DE = initial address of dst line

	pop hl

	ldi		;; transfer line
	ldi		;; number of LDIs here must be exactly SCROLL_COLS
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi

	inc a
	cp a,SCROLL_LINES	;; number of lines to draw
	jr nz, loop1

	ret
