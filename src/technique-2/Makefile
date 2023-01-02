# compilation
ZCC = zcc +zx -vn -m --c-code-in-asm --list
LIBS = -lndos

# files
C_SRC = main.c offscreen.c
C_OBJS = $(C_SRC:.c=.o)
ASM_SRC = asm_offscreen_stack.asm
ASM_OBJS = $(ASM_SRC:.asm=.o)
BIN = scroll.bin
TAP = $(BIN:.bin=.tap)
OBJS = $(C_OBJS) $(ASM_OBJS)

.SILENT:

%.o: %.c
	$(ZCC) -c $< -o $@

%.o: %.asm
	$(ZCC) -c $< -o $@

bin: $(OBJS)
	$(ZCC) -startup=31 $(OBJS) $(LIBS) -o $(BIN) -create-app

clean:
	-rm -f *.bin *.tap *.o *.lis *.map

run:
	fuse $(TAP)

mem:
	z88dk-z80nm *.o | grep -E "(^Object |Section)"

build: clean bin
