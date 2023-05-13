# compiler configuration
ZCC		= zcc +zx
CFLAGS		= -vn -m --list --c-code-in-asm -compiler=sdcc
LDFLAGS		= -lndos -lsp1-zx

# sources
SRCS_C		= $(wildcard *.c)
SRCS_ASM	= $(wildcard *.asm)
OBJS		= $(SRCS_C:.c=.o) $(SRCS_ASM:.asm=.o)

# targets
.SILENT:
.PHONY: clean run graphics build

%.o: %.c
	$(ZCC) $(CFLAGS) -c $*.c

%.o: %.asm
	$(ZCC) $(CFLAGS) -c $*.asm

main.tap: $(OBJS)
	$(ZCC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o main -create-app

clean:
	-rm -f *.tap *.o *.bin *.map *.lis main
	$(MAKE) -s -C graphics clean

run:
	fuse main.tap

graphics:
	$(MAKE) -s -C graphics

build: clean
	$(MAKE) -s -j8 main.tap
