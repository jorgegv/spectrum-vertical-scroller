;; transfer frame from offscreen buffer to real screen

EXTERN	_offscreen_extra_line_address	;; table of line initial addresses (src)
EXTERN	_screen_line_address		;; table of line initial addresses (dst)
EXTERN	_current_scroll_offset_line	;; 0..15

PUBLIC	_asm_offscreen_show_frame_ldir
_asm_offscreen_show_frame_ldir:

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

	ld l,a
	ld h,0
	add hl,hl
	ld de,_screen_line_address
	add hl,de
	ld e,(hl)
	inc hl
	ld d,(hl)	;; DE = initial address of dst line

	pop hl

	ld bc,16	;; transfer line
	ldir

	inc a
	cp a,128	;; number of lines to draw
	jr nz, loop1

	ret
