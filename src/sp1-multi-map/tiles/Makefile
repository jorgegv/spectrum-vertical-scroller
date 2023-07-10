.SILENT:

C_TARGETS = diamond.c clubs.c spades.c hearts.c
C_TARGETS += wallh.c wallv.c cornerbl.c cornerbr.c cornertl.c cornertr.c
C_TARGETS += walltt.c walltb.c walltl.c walltr.c
C_TARGETS += wallhre.c wallhle.c wallvte.c wallvbe.c sand.c flower.c

all: $(C_TARGETS)

%.c: %.tile
	# compile to binary
	sed 's/^/dw %"/g' $*.tile > $*.asm
	sed -i 's/$$/"/g' $*.asm
	z80asm -b $*.asm
	# generate bytes in columns for sprite and mask
	od -tx1 -Ax -v $*.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==0' >$*.col0
	od -tx1 -Ax -v $*.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==1' >$*.col1
	# interleave (mask,graph) as for SP1_DRAW_MASK2, sprite in columns, 7 lines separation between columns
	echo -e "#include <stdint.h>\n\nuint8_t $*_tile[] = {" > $*.c
	( echo -n "0x"; cat $*.col0 $*.col1 ) | tr -s "\n\t" "  "| sed 's/ $$//g' | sed 's/ /, 0x/g' | \
		fold -s -w100 | sed 's/^/    /g' >> $*.c
	echo -e "\n};" >>$*.c

clean:
	-rm *.col* *.bin *.asm *.o *.c *.lis 2>/dev/null
