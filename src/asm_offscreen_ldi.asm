;; transfer frame from offscreen buffer to real screen

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
	ld b,(hl)	;; BC = initial address of first src line

	ld a,0		;; line loop counter

	push bc		;; save src address

loop1:
	ld l,a
	ld h,0
	add hl,hl
	ld de,_screen_line_address
	add hl,de
	ld e,(hl)
	inc hl
	ld d,(hl)	;; DE = initial address of dst line

	pop hl		;; HL = initial address of src line

	ldi		;; transfer line
	ldi		;; number of LDIs here must match the scroll window width in cells
	ldi
	ldi
	ldi
	ldi
	ldi
	ldi

	push hl		;; HL is ready for next iteration, save it

	inc a
	cp a,64		;; number of lines to draw
	jr nz, loop1

	pop hl		;; clean leftovers

	ld a,(_current_scroll_offset_line)
	dec a
	cp $ff
	jr nz,update1
	ld a,15		;; number of extra lines
update1:
	ld (_current_scroll_offset_line),a

	ret
