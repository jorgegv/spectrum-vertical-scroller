// Recent Z88DK: zcc +zx -vn -lndos -lsp1-zx -o sp1scroll -create-app sp1scroll.c
// Last official Z88DK: zcc +zx -vn -lndos -lsp1 -o sp1scroll -create-app sp1scroll.c

#pragma output STACKPTR=53248                    // place stack at $d000 at startup

#include <spectrum.h>
#include <sprites/sp1.h>
#include <intrinsic.h>
#include <stdint.h>

#include "perf.h"

//char tile[8] = { 1,2,4,8,16,32,64,128 };
char tile[8] = { 255,0,0,0,0,0,0,0 };

#define SCROLL_AREA_POS_ROW	0
#define SCROLL_AREA_POS_COL	8
#define SCROLL_AREA_WIDTH	16
#define SCROLL_AREA_HEIGHT	16

struct sp1_Rect scroll_area = { SCROLL_AREA_POS_ROW, SCROLL_AREA_POS_COL, SCROLL_AREA_WIDTH, SCROLL_AREA_HEIGHT };

void scroll_tile( void ) {
  int i;
  uint8_t tmp;

  // save bottom pixels
  tmp = tile[7];

  // just scroll all values from top to down
  for ( i = 6; i >= 0; i-- )
    tile[i+1] = tile[i];

  // bottom pixels come to top
  tile[0] = tmp;
}

void main( void ) {
    uint8_t r,c;

    init_interrupts();

    // initialization
    zx_border(INK_BLACK);
    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
      PAPER_BLACK | INK_GREEN, ' ');
    for ( r = SCROLL_AREA_POS_ROW; r < SCROLL_AREA_POS_ROW + SCROLL_AREA_HEIGHT; r++ )
      for ( c= SCROLL_AREA_POS_COL; c < SCROLL_AREA_POS_COL + SCROLL_AREA_WIDTH; c++ )
        sp1_PrintAt( r, c, PAPER_BLACK | INK_YELLOW, (uint16_t) &tile );

    reset_perfmeter();
    while (1) {
      // do whatever we want with the background
      scroll_tile();

      // invalidate the whole scroll area
      sp1_Invalidate( &scroll_area );

//      zx_border( INK_CYAN );
      sp1_UpdateNow();
//      zx_border( INK_BLACK );
      do_perf_accounting();
    }
}
