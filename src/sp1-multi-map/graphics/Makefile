.SILENT:

all: ball diamond.c clubs.c spades.c hearts.c wallh.c wallv.c cornerbl.c cornerbr.c cornertl.c cornertr.c

%.c: %.asm
	# compile to binary
	z80asm -b $*.asm
	# generate bytes in columns for sprite and mask
	od -tx1 -Ax -v $*.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==0' >$*.col0
	od -tx1 -Ax -v $*.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==1' >$*.col1
	# interleave (mask,graph) as for SP1_DRAW_MASK2, sprite in columns, 7 lines separation between columns
	echo -e "#include <stdint.h>\n\nuint8_t $*_tile[] = {" > $*.c
	( echo -n "0x"; cat $*.col0 $*.col1 ) | tr -s "\n\t" "  "| sed 's/ $$//g' | sed 's/ /, 0x/g' | \
		fold -s -w100 | sed 's/^/    /g' >> $*.c
	echo -e "\n};" >>$*.c

ball:
	# compile to binary
	z80asm -b ball.asm
	z80asm -b ballmask.asm
	# generate bytes in columns for sprite and mask
	od -tx1 -Ax ball.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==0' >ball.col0
	od -tx1 -Ax ball.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==1' >ball.col1
	od -tx1 -Ax ballmask.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==0' >ballmask.col0
	od -tx1 -Ax ballmask.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==1' >ballmask.col1
	# interleave (mask,graph) as for SP1_DRAW_MASK2, sprite in columns, 7 lines separation between columns
	echo -e "#include <stdint.h>\n\nuint8_t ball_sprite[] = {" > ball.c
	( echo -n "0x"; for i in `seq 1 8`; do echo -e "ff\t00"; done; \
		paste ballmask.col0 ball.col0 ; \
	for i in `seq 1 8`; do echo -e "ff\t00"; done; \
		paste ballmask.col1 ball.col1 ; \
	for i in `seq 1 8`; do echo -e "ff\t00"; done ) | tr -s "\n\t" "  "| sed 's/ $$//g' | sed 's/ /, 0x/g' | \
		fold -s -w100 | sed 's/^/    /g' >> ball.c
	echo -e "\n};" >>ball.c

clean:
	-rm *.col* *.bin *.o *.c 2>/dev/null
