// zcc +zx -vn -lndos -lsp1-zx -o sp1scroll -create-app sp1scroll.c

#include <spectrum.h>
#include <sprites/sp1.h>
#include <intrinsic.h>
#include <stdint.h>

//char tile[8] = { 1,2,4,8,16,32,64,128 };
char tile[8] = { 255,0,0,0,0,0,0,0 };

struct sp1_Rect scroll_area = { 0, 8, 16, 24 };

void rotate_tile( void ) {
  int i;
  uint8_t tmp;
  tmp = tile[7];
  for ( i = 6; i >= 0; i-- )
    tile[i+1] = tile[i];
  tile[0] = tmp;
}

void main( void ) {
    uint8_t r,c;
    zx_border(INK_BLACK);
    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
      PAPER_BLACK | INK_GREEN, ' ');
    for ( r = scroll_area.row; r < scroll_area.row+scroll_area.height; r++ )
      for ( c= scroll_area.col; c < scroll_area.col+scroll_area.width; c++ )
        sp1_PrintAt( r, c, PAPER_BLACK | INK_YELLOW, ( void * ) &tile );

    while (1) {
      sp1_Invalidate( &scroll_area );
      rotate_tile();

      intrinsic_halt();
//      zx_border( INK_CYAN );
      sp1_UpdateNow();
//      zx_border( INK_BLACK );
      intrinsic_halt();
    }
}
