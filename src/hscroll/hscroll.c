// zcc +zx -lndos -o hscroll -create-app hscroll.c

#include <stdio.h>
#include <conio.h>
#include <intrinsic.h>

void scroll_right_1pixel( void ) __naked {
__asm

start:
	ld hl,16384	;; first screen line
	ld de,32	;; increment for next line

	ld a,192	;; number of lines

line:
	ld b,32
	or a
loop_line:
	rr (hl)
	inc hl
	djnz loop_line

	dec a
	jp nz, line
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
        scroll_right_1pixel();
    	zx_border( INK_WHITE );
    }
}
