// zcc +zx hscroll.c -o hscroll -create-app

void scroll_right( void ) __naked {
__asm
	ld hl,16384
	ld a,1
	ld b,32
loop1:
	ld (hl),a
	inc a
	inc hl
	djnz loop1

;; rotamos
inicio:
	ld b,32
	ld hl,16384+31
loop2:
	ld a,(hl)
	dec hl
	ld c,(hl)
	rrc c
	rra
	inc hl
	ld (hl),a
	dec hl
	djnz loop2

	halt
	jp inicio
__endasm;
}

void main( void ) {
    scroll_right();
}

