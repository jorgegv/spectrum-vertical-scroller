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
	-rm -f *.tap *.o *.bin *.map *.lis main config.h scroll_map_data.*
	if [ -d graphics ]; then $(MAKE) -s -C graphics clean ; fi
	$(MAKE) -s -C sprites clean
	$(MAKE) -s -C tiles clean

run:
	fuse main.tap

tiles:
	$(MAKE) -s -C tiles

sprites:
	$(MAKE) -s -C sprites

build: clean config tiles sprites
	$(MAKE) -s -j8 main.tap

map:
	./create-map.sh

config: map
	./create-config.sh
