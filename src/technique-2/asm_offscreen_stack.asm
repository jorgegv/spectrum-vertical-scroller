;; transfer frame from offscreen buffer to real screen

;; these constants must match those in offscreen.h
SCROLL_LINES		equ 128
SCROLL_EXTRA_LINES	equ 16
SCROLL_COLS		equ 16

EXTERN	_offscreen_line_address		;; table of line initial addresses (src)
EXTERN	_screen_line_end_address	;; table of line end addresses (dst)
EXTERN	_current_scroll_start_line
EXTERN	_offscreen			;; virtual screen

;; temp vars
counter:
	db 0

PUBLIC	_asm_offscreen_show_frame_stack
_asm_offscreen_show_frame_stack:

	;; interrupts must be disabled during this routine
	di

	;; use SMC to save SP to the position at the end of this routine
	ld (restore_sp - 2),sp

	;; BC = offscreen_line_address[ current_scroll_start_line ]
	ld a,(_current_scroll_start_line)
	ld l,a
	ld h,0
	add hl,hl	;; HL = offset into LUT
	ld de,_offscreen_line_address
	add hl,de	;; HL = address of the LUT element
	ld c,(hl)
	inc hl
	ld b,(hl)
	ld (switch_sp_2 - 2),bc	;; save initial address of first src line

	;; set line loop counter to 0
	xor a
	ld (counter),a

	;; set initial ptr to first dst line address
	;; right to left!
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

	ld sp,($ffff)	;; SMC - $ffff value will be the temporary storage
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

	;; if next line is out of the offscreen, then reset src to the
	;; beginning of the offscreen:
	;; DE = addr for next line (we saved it before)
	ld de,(switch_sp_2 - 2)
	;; HL = addr of end of offscreen + 1
	ld hl,_offscreen + ( ( SCROLL_LINES + SCROLL_EXTRA_LINES ) * SCROLL_COLS )
	;; check if DE >= HL
	or a		;; reset CF
	sbc hl,de
	jr z,reset_1	;; reset if HL equals DE
	jr nc,skip_1	;; skip reset if HL is greater than DE
reset_1:
	ld hl,_offscreen
	ld (switch_sp_2 - 2),hl

skip_1:
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
