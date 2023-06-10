// Compilation:
//   Nightly Z88DK:	zcc +zx -vn -m --list --c-code-in-asm -lndos -lsp1-zx -o sp1scroll -create-app sp1scroll.c
//   Release Z88DK:	zcc +zx -vn -m --list --c-code-in-asm -lndos -lsp1    -o sp1scroll -create-app sp1scroll.c

#pragma output STACKPTR=53248                    // place stack at $d000 at startup

#include <spectrum.h>
#include <sprites/sp1.h>
#include <intrinsic.h>
#include <stdint.h>

#include <stdio.h>

// scroll area constants
#define SCROLL_AREA_POS_ROW		0
#define SCROLL_AREA_POS_COL		8
#define SCROLL_AREA_WIDTH		16
#define SCROLL_AREA_HEIGHT		16

// the tiles that are used for drawing the map have this dimensions
// there is a row of this height at the top of the offscreen to draw the
// new tiles that are coming into view
#define SCROLL_AREA_TOP_TILE_HEIGHT	2
#define SCROLL_AREA_TOP_TILE_WIDTH	2
#define SCROLL_AREA_REAL_HEIGHT		( SCROLL_AREA_HEIGHT + SCROLL_AREA_TOP_TILE_HEIGHT )

// the scroll speed, i.e. the number of pixels scrolled down on each round
#define SCROLL_PIXELS			1

// a test diamond tile - must have the dimensions mentioned above
uint8_t tile[ 8 * SCROLL_AREA_TOP_TILE_HEIGHT * SCROLL_AREA_TOP_TILE_WIDTH ] = {
  1,2,4,8,16,32,64,128,
  128,64,32,16,8,4,2,1,
  128,64,32,16,8,4,2,1,
  1,2,4,8,16,32,64,128
};

// The offscreen buffer. There are SCROLL_PIXELS top pixel lines on each column which are
// initialized to 0 so that when scrolling down the top non-visible tile row is erased
#define OFFSCREEN_BUFFER_SIZE			( SCROLL_AREA_WIDTH * SCROLL_PIXELS + SCROLL_AREA_WIDTH * SCROLL_AREA_REAL_HEIGHT * 8 )
uint8_t offscreen[ OFFSCREEN_BUFFER_SIZE ];

// Tables of start and end addresses for each column; initialized at start
uint8_t *offscreen_column_start_address[ SCROLL_AREA_WIDTH ];
uint8_t *offscreen_column_end_address[ SCROLL_AREA_WIDTH ];

// SP1 scroll area
struct sp1_Rect scroll_area = { SCROLL_AREA_POS_ROW, SCROLL_AREA_POS_COL, SCROLL_AREA_WIDTH, SCROLL_AREA_HEIGHT };

uint16_t cell_address_offset( uint8_t row, uint8_t col ) {
  return ( 8 * ( SCROLL_AREA_REAL_HEIGHT * col + row ) + SCROLL_PIXELS * ( col + 1 ) );
}

// initialize address tables
void init_address_tables( void ) {
    uint8_t c;
    for ( c = 0; c < SCROLL_AREA_WIDTH; c++ ) {
      offscreen_column_start_address[ c ] = &offscreen[ cell_address_offset( 0, c ) ];
      offscreen_column_end_address[ c ] = &offscreen[ cell_address_offset( 0, c ) + ( 8 * SCROLL_AREA_REAL_HEIGHT ) - 1 ];
    }
}

// initialize scroll area
void init_scroll_area( void ) {
  uint8_t c,i;
  // set the top pixel line to 0
  for ( c = 0; c < SCROLL_AREA_WIDTH; c++ )
    for ( i = 0; i < SCROLL_PIXELS; i++ )
      offscreen[ c * ( SCROLL_AREA_REAL_HEIGHT * 8 + SCROLL_PIXELS ) + i ] = 0;
}

// initialize SP1 tile map
void init_tile_map( void ) {
    uint8_t r,c;
    // setup the SP1 tile map to point to the cells in the offscreen buffer
    // the top 2 rows are reserved for drawing and not visible
    // the tile array is configured in columns!
    for ( c = 0; c < SCROLL_AREA_WIDTH; c++ )
      for ( r = 0; r < SCROLL_AREA_HEIGHT; r++ )
        sp1_PrintAt( SCROLL_AREA_POS_ROW + r, SCROLL_AREA_POS_COL + c,		// screen position
          ( ( (r+c) % 2) ? PAPER_BLACK : PAPER_RED ) | INK_WHITE | BRIGHT,	// attr
          ( uint16_t ) &offscreen[ cell_address_offset( r + SCROLL_AREA_TOP_TILE_HEIGHT, c ) ] );	// pointer
}

// function to scroll a column SCROLL_PIXELS pixels down - loop version
// the number of DEC HL instructions at the top must be SCROLL_PIXELS
void scroll_down_column_loop( uint8_t *data ) __z88dk_fastcall __naked {
__asm
    ld de,hl
REPT SCROLL_PIXELS
    dec hl
ENDR
    ld bc,SCROLL_AREA_REAL_HEIGHT*8
    lddr
    ret
__endasm;
}

// function to scroll a column SCROLL_PIXELS pixels down - unrolled version
// the number of DEC HL instructions at the top must be SCROLL_PIXELS
// the number of LDDs must be exactly SCROLL_AREA_REAL_HEIGHT * 8
void scroll_down_column_unrolled( uint8_t *data ) __z88dk_fastcall __naked {
__asm
    ld de,hl

REPT SCROLL_PIXELS
    dec hl
ENDR

REPT SCROLL_AREA_REAL_HEIGHT*8
    ldd
ENDR

    ret
__endasm;
}

// scroll the whole scroll area one pixel down
void scroll_down_area( void ) {
  uint8_t i;
  for ( i = 0; i < SCROLL_AREA_WIDTH; i++ )
//    scroll_down_column_loop( offscreen_column_end_address[ i ] );
    scroll_down_column_unrolled( offscreen_column_end_address[ i ] );
}

void draw_tile_on_top( uint8_t *tile, uint8_t col ) {
  uint8_t i,j;
  for ( i = 0; i < SCROLL_AREA_TOP_TILE_HEIGHT * 8; i++ )
    for ( j = 0; j < SCROLL_AREA_TOP_TILE_WIDTH; j++ )
      offscreen_column_start_address[ col + j ][ i ] = tile[ j * SCROLL_AREA_TOP_TILE_HEIGHT * 8 + i ];
}

void main( void ) {
    uint8_t i,c;

    zx_border(INK_BLACK);

    // initializations
    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
      PAPER_BLACK | INK_GREEN, ' ');
    init_address_tables();
    init_scroll_area();
    init_tile_map();

    // main loop
    i = c = 0;
    while (1) {
      // do whatever we want with the background
      zx_border(INK_BLUE);
      scroll_down_area();
      zx_border(INK_BLACK);

      // draw tiles
      if ( ! ( i++ % (SCROLL_AREA_TOP_TILE_HEIGHT * 8 ) ) ) {
        draw_tile_on_top( tile, c * 2 );
        if ( ++c == 8 )
          c = 0;
      }

      // invalidate the whole scroll area
      sp1_Invalidate( &scroll_area );

      // redraw
      sp1_UpdateNow();
    }
}
