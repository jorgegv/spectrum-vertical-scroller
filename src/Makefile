# compilation
ZCC = zcc +zx -vn -startup=31 -m --c-code-in-asm --list
LIBS = -lndos

# files
SOURCES = main.c offscreen.c
BIN = scroll.bin

.SILENT:

default:
	$(ZCC) $(SOURCES) $(LIBS) -o $(BIN) -create-app

clean:
	-rm -f *.bin *.tap *.o *.lis *.map
