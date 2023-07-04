.SILENT:

all: ball diamond clubs spades hearts

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

diamond:
	# compile to binary
	z80asm -b diamond.asm
	# generate bytes in columns for sprite and mask
	od -tx1 -Ax diamond.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==0' >diamond.col0
	od -tx1 -Ax diamond.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==1' >diamond.col1
	# interleave (mask,graph) as for SP1_DRAW_MASK2, sprite in columns, 7 lines separation between columns
	echo -e "#include <stdint.h>\n\nuint8_t diamond_tile[] = {" > diamond.c
	( echo -n "0x"; cat diamond.col0 diamond.col1 ) | tr -s "\n\t" "  "| sed 's/ $$//g' | sed 's/ /, 0x/g' | \
		fold -s -w100 | sed 's/^/    /g' >> diamond.c
	echo -e "\n};" >>diamond.c

clubs:
	# compile to binary
	z80asm -b clubs.asm
	# generate bytes in columns for sprite and mask
	od -tx1 -Ax clubs.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==0' >clubs.col0
	od -tx1 -Ax clubs.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==1' >clubs.col1
	# interleave (mask,graph) as for SP1_DRAW_MASK2, sprite in columns, 7 lines separation between columns
	echo -e "#include <stdint.h>\n\nuint8_t clubs_tile[] = {" > clubs.c
	( echo -n "0x"; cat clubs.col0 clubs.col1 ) | tr -s "\n\t" "  "| sed 's/ $$//g' | sed 's/ /, 0x/g' | \
		fold -s -w100 | sed 's/^/    /g' >> clubs.c
	echo -e "\n};" >>clubs.c

spades:
	# compile to binary
	z80asm -b spades.asm
	# generate bytes in columns for sprite and mask
	od -tx1 -Ax spades.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==0' >spades.col0
	od -tx1 -Ax spades.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==1' >spades.col1
	# interleave (mask,graph) as for SP1_DRAW_MASK2, sprite in columns, 7 lines separation between columns
	echo -e "#include <stdint.h>\n\nuint8_t spades_tile[] = {" > spades.c
	( echo -n "0x"; cat spades.col0 spades.col1 ) | tr -s "\n\t" "  "| sed 's/ $$//g' | sed 's/ /, 0x/g' | \
		fold -s -w100 | sed 's/^/    /g' >> spades.c
	echo -e "\n};" >>spades.c

hearts:
	# compile to binary
	z80asm -b hearts.asm
	# generate bytes in columns for sprite and mask
	od -tx1 -Ax hearts.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==0' >hearts.col0
	od -tx1 -Ax hearts.bin|cut -c8-|tr -s " " "\n" |awk 'NR%2==1' >hearts.col1
	# interleave (mask,graph) as for SP1_DRAW_MASK2, sprite in columns, 7 lines separation between columns
	echo -e "#include <stdint.h>\n\nuint8_t hearts_tile[] = {" > hearts.c
	( echo -n "0x"; cat hearts.col0 hearts.col1 ) | tr -s "\n\t" "  "| sed 's/ $$//g' | sed 's/ /, 0x/g' | \
		fold -s -w100 | sed 's/^/    /g' >> hearts.c
	echo -e "\n};" >>hearts.c

clean:
	-rm *.col* *.bin *.o *.c 2>/dev/null
