;; transfer frame from offscreen buffer to real screen

;; these constants must match those in offscreen.h
SCROLL_LINES	equ 128
SCROLL_COLS	equ 16

EXTERN	_offscreen_extra_line_address	;; table of line initial addresses (src)
EXTERN	_screen_line_end_address		;; table of line initial addresses (dst)
EXTERN	_current_scroll_offset_line	;; 0..15

;; temp vars
counter:
	db 0
src_addr:
	dw 0
dst_addr:
	dw 0

PUBLIC	_asm_offscreen_show_frame_stack
_asm_offscreen_show_frame_stack:

	;; interrupts must be disabled during this routine
	di

	;; use SMC to save SP to the position at the end of this routine
	ld (restore_sp - 2),sp

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
	ld (src_addr),bc	;; save initial address of first src line

	xor a		;; set line loop counter to 0

loop1:
	;; get initial address of destination line from LUT
	ld (counter),a
	ld l,a
	ld h,0
	add hl,hl
	ld de,_screen_line_end_address
	add hl,de
	ld e,(hl)
	inc hl
	ld d,(hl)	;; DE = initial address of dst line
	ld (dst_addr),de

	;; src and dst addresses are ready
	;; read 16 bytes into regs
	ld sp,(src_addr)
	pop af
	pop bc
	pop de
	pop hl
	exx
	pop af
	pop bc
	pop de
	pop hl

	;; write 16 bytes to memory
	ld sp,(dst_addr)
	push hl
	push de
	push bc
	push af
	exx
	push hl
	push de
	push bc
	push af

	;; adjust src address
	ld hl,(src_addr)
	ld de,SCROLL_COLS
	add hl,de
	ld (src_addr),HL

	;; inc counter and check
	ld hl,counter
	inc (hl)
	ld a,(counter)
	cp a,SCROLL_LINES	;; number of lines to draw
	jr nz, loop1

	ld sp,0		;; SMC: the 0 is modified when saving SP at the top
restore_sp:
	ei
	ret
