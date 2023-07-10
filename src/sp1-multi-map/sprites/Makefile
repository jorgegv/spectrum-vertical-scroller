.SILENT:

all: ball.c

%.c: %.sprite
	sed 's/^/dw %"/g' $*.sprite > $*.asm
	sed -i 's/$$/"/g' $*.asm
	sed 's/^/dw %"/g' $*.mask > $*_mask.asm
	sed -i 's/$$/"/g' $*_mask.asm
	# compile to binary
	z80asm -b $*.asm
	z80asm -b $*_mask.asm
	# generate bytes in columns for sprite and mask
	od -tx1 -Ax $*.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==0' >$*.col0
	od -tx1 -Ax $*.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==1' >$*.col1
	od -tx1 -Ax $*_mask.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==0' >$*_mask.col0
	od -tx1 -Ax $*_mask.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==1' >$*_mask.col1
	# interleave (mask,graph) as for SP1_DRAW_MASK2, sprite in columns, 7 lines separation between columns
	echo -e "#include <stdint.h>\n\nuint8_t $*_sprite[] = {" > $*.c
	( echo -n "0x"; for i in `seq 1 8`; do echo -e "ff\t00"; done; \
		paste $*_mask.col0 $*.col0 ; \
	for i in `seq 1 8`; do echo -e "ff\t00"; done; \
		paste $*_mask.col1 $*.col1 ; \
	for i in `seq 1 8`; do echo -e "ff\t00"; done ) | tr -s "\n\t" "  "| sed 's/ $$//g' | sed 's/ /, 0x/g' | \
		fold -s -w100 | sed 's/^/    /g' >> $*.c
	echo -e "\n};" >>$*.c

clean:
	-rm *.col* *.bin *.asm *.o *.c *.lis 2>/dev/null
