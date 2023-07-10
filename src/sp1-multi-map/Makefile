# compiler configuration
ZCC		= zcc +zx
CFLAGS		= -vn -m --list --c-code-in-asm -compiler=sdcc
LDFLAGS		= -lndos -lsp1-zx -lmath32

# sources
SRCS_C		= $(wildcard *.c) $(wildcard tiles/*.c) $(wildcard sprites/*.c)
SRCS_ASM	= $(wildcard *.asm)
OBJS		= $(SRCS_C:.c=.o) $(SRCS_ASM:.asm=.o)

# targets
.SILENT:
.PHONY: clean run tiles sprites build config map

%.o: %.c
	$(ZCC) $(CFLAGS) -c $*.c

%.o: %.asm
	$(ZCC) $(CFLAGS) -c $*.asm

main.tap: $(OBJS)
	$(ZCC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o main -create-app

clean:
	-rm -f *.tap *.o *.bin *.map *.lis main config.h graphics.h scroll_map_data.*
	if [ -d graphics ]; then $(MAKE) -s -C graphics clean ; fi
	$(MAKE) -s -C sprites clean
	$(MAKE) -s -C tiles clean

run:
	fuse main.tap

tiles:
	$(MAKE) -s -C tiles

sprites:
	$(MAKE) -s -C sprites

graphics.h:
	echo -e "#ifndef _GRAPHICS_H\n#define _GRAPHICS_H\n\n#include <stdint.h>\n" > graphics.h
	echo -e "// Sprite data" >> graphics.h
	find sprites -name "*.sprite" | xargs -l basename | while read -r file; do \
		printf "extern uint8_t %s_sprite[];\n" $$( basename $$file .sprite ); \
		done >> graphics.h
	echo -e "\n// Tile data" >>graphics.h
	find tiles -name "*.tile" | xargs -l basename | while read -r file; do \
		printf "extern uint8_t %s_tile[];\n" $$( basename $$file .tile ); \
		done >> graphics.h
	echo -e "\n#endif // _GRAPHICS_H" >>graphics.h

build: clean config tiles sprites graphics.h
	$(MAKE) -s -j8 main.tap

map:
	./create-map.sh

config: map
	./create-config.sh
