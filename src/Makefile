# compilation
ZCC = zcc +zx -vn -m --c-code-in-asm --list
LIBS = -lndos

# files
SOURCES = main.c offscreen.c
OBJS = $(SOURCES:.c=.o)
BIN = scroll.bin
TAP = $(BIN:.bin=.tap)

.SILENT:

%.o: %.c
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
