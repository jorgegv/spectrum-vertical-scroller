// zcc +zx -lndos -m -o hscroll -create-app hscroll.c

#include <stdio.h>
#include <conio.h>
#include <intrinsic.h>

void scroll_right_1pixel( void ) __naked {
__asm

start1:
	ld hl,16384	;; first screen line
	ld a,192	;; number of lines

line1:
	ld b,32
	or a
loop_line1:
	rr (hl)
	inc hl
	djnz loop_line1

	dec a
	jp nz, line1
	ret

__endasm;
}

void scroll_right_2pixel( void ) __naked {
__asm

start2:
	ld hl,16384+31	;; first screen line
	ld a,192	;; number of lines

line2:
	ld b,31

	ex af,af	;; save line counter

	;; process the byte at (HL)
loop_line2:
	ld de,hl	;; save pointer in DE for later

	ld c,(hl)	;; C = current byte
	dec hl
	ld a,(hl)	;; A = byte to its left

	;; repeat the following block twice for 2 pixel scroll; 3 times for 3 pixel, etc.
	rr a		;; take lowest bit of previous byte into CF
	rr c		;; and store it into highest bit of current byte, rotating

	rr a		;; repeat as needed
	rr c

	ld (de),c	;; store processed byte
			;; HL is already decremented for next iteration

	djnz loop_line2

	ld a,(hl)	;; the leftmost byte is special

	;; same number of repetitions as the block above
	or a		;; CF = 0
	rr a		

	or a		;; CF = 0
	rr a

	ld (hl),a

	ld de,63	;; add 32+31 for end of next line
	add hl,de

	ex af,af	;; continue with line counter
	dec a
	jp nz, line2
	ret

__endasm;
}

void scroll_right_4pixel( void ) __naked {
__asm

start4:
	ld hl,16384	;; first screen line
	ld c,192	;; number of lines

line4:
	ld b,32
	xor a
loop_line4:
	rrd		;; rrd (hl)
	inc hl
	djnz loop_line4

	dec c
	jp nz, line4
	ret

__endasm;
}

void main( void ) {
    int i,j;
    for ( i=0; i < 24; i++ ) {
    	gotoxy( i, i );
    	for ( j=0; j < i/2; j++ )
            printf( "AA" );
    }
    	
    while (1) {
//    	intrinsic_halt();
    	zx_border( INK_RED );
//        scroll_right_1pixel();
        scroll_right_2pixel();
//	scroll_right_4pixel();
    	zx_border( INK_WHITE );
    }
}
