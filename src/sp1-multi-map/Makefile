# compiler configuration
ZCC		= zcc +zx
CFLAGS		= -vn -m --list --c-code-in-asm -compiler=sdcc
LDFLAGS		= -lndos -lsp1-zx -lmath32

# sources
SRCS_C		= $(wildcard *.c)
SRCS_ASM	= $(wildcard *.asm)
OBJS		= $(SRCS_C:.c=.o) $(SRCS_ASM:.asm=.o)

# targets
.SILENT:
.PHONY: clean run graphics build config map

%.o: %.c
	$(ZCC) $(CFLAGS) -c $*.c

%.o: %.asm
	$(ZCC) $(CFLAGS) -c $*.asm

main.tap: $(OBJS)
	$(ZCC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o main -create-app

clean:
	-rm -f *.tap *.o *.bin *.map *.lis main config.h scroll_map_data.*
	if [ -d graphics ]; then $(MAKE) -s -C graphics clean ; fi

run:
	fuse main.tap

graphics:
	$(MAKE) -s -C graphics
	cp graphics/*.c .
	$(MAKE) -s -C graphics clean

build: clean config graphics
	$(MAKE) -s -j8 main.tap

map:
	./create-map.sh

config: map
	./create-config.sh
