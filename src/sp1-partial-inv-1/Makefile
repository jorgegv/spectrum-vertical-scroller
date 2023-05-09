.SILENT:
.PHONY: build clean run

build:
	zcc +zx -vn -m --list --c-code-in-asm -compiler=sdcc -lndos -lsp1-zx -o sp1scroll -create-app sp1scroll.c

clean:
	-rm -f *.tap *.o *.bin *.map *.lis sp1scroll
	$(MAKE) -C graphics clean

run:
	fuse sp1scroll.tap

graphics:
	$(MAKE) -C graphics
