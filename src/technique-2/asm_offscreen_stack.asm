;; transfer frame from offscreen buffer to real screen

;; these constants must match those in offscreen.h
SCROLL_LINES	equ 128
SCROLL_COLS	equ 16

EXTERN	_offscreen_extra_line_address	;; table of line initial addresses (src)
EXTERN	_screen_line_end_address	;; table of line initial addresses (dst)
EXTERN	_current_scroll_start_line	;; 0..15

;; temp vars
counter:
	db 0

PUBLIC	_asm_offscreen_show_frame_stack
_asm_offscreen_show_frame_stack:

	;; interrupts must be disabled during this routine
	di

	;; use SMC to save SP to the position at the end of this routine
	ld (restore_sp - 2),sp

	;; DE = offscreen_extra_line_address[ current_scroll_start_line ]
	ld a,(_current_scroll_start_line)
	add a,a
	ld l,a
	ld h,0		;; HL = offset into LUT
	ld de,_offscreen_extra_line_address
	add hl,de	;; HL = address of the LUT element
	ld c,(hl)
	inc hl
	ld b,(hl)
	ld (switch_sp_2 - 2),bc	;; save initial address of first src line

	;; set line loop counter to 0
	xor a
	ld (counter),a

	;; set initial ptr to dst line address
	ld hl,_screen_line_end_address
	ld (switch_sp_1 - 2),hl

loop1:
	;; src and dst addresses are ready
	;; read 16 bytes from src into regs
	ld sp,$ffff		;; SMC: $ffff is used as a variable and modified at the top
switch_sp_2:

	pop af
	pop bc
	pop de
	pop hl
	exx
	ex af,af'
	pop af
	pop bc
	pop de
	pop hl

	;; save current src for next line
	ld (switch_sp_2 - 2),sp

	ld sp,($0)	;; SMC - this value will be the temporary storage
			;; for ptr to dst address
switch_sp_1:

	push hl
	push de
	push bc
	push af
	exx
	ex af,af'
	push hl
	push de
	push bc
	push af

	;; adjust dst address ptr for next iteration: load next address from
	;; LUT
	ld hl,(switch_sp_1 - 2)
	inc hl
	inc hl
	ld (switch_sp_1 - 2),hl

	;; inc counter and check
	ld hl,counter
	inc (hl)
	ld a,(counter)
	cp a,SCROLL_LINES	;; number of lines to draw
	jr nz, loop1

	ld sp,$ffff		;; SMC: $ffff is used as a variable and modified at the top
restore_sp:

	ei
	ret
