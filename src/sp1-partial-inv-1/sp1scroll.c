// Compilation:
//   Nightly Z88DK:	zcc +zx -vn -m --list --c-code-in-asm -compiler=sdcc -lndos -lsp1-zx -o sp1scroll -create-app sp1scroll.c
//   Release Z88DK:	zcc +zx -vn -m --list --c-code-in-asm -compiler=sdcc -lndos -lsp1    -o sp1scroll -create-app sp1scroll.c

// For classic:
#include <spectrum.h>
#include <sprites/sp1.h>

// For Newlib:
//#include <arch/spectrum.h>
//#include <games/sp1.h>

//#pragma output STACKPTR=53248                    // place stack at $d000 at startup
#pragma output CRT_ORG_CODE           = 0x5f00  // org of compile
#pragma output REGISTER_SP            = 0xd1d1  // just below ISR
#pragma output CRT_STACK_SIZE         = 208     // just enough

#pragma output CRT_ENABLE_CLOSE       = 0       // don't bother closing files on exit
#pragma output CRT_ENABLE_EIDI        = 1       // disable interrupts at start
#pragma output CRT_ENABLE_RESTART     = 1       // do not return to basic

#pragma output CLIB_MALLOC_HEAP_SIZE  = 0       // // heap: initialized manually
#pragma output CLIB_STDIO_HEAP_SIZE   = 0       // no stdio heap (no files)

#include <intrinsic.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

///////////////////////////////////////
//
// SCROLL AREA DEFINITIONS
//
///////////////////////////////////////

// scroll area constants
#define SCROLL_AREA_POS_ROW		0
#define SCROLL_AREA_POS_COL		12
#define SCROLL_AREA_WIDTH		16
#define SCROLL_AREA_HEIGHT		16

#define SCROLL_AREA_POS_X		( SCROLL_AREA_POS_COL * 8 )
#define SCROLL_AREA_POS_Y		( SCROLL_AREA_POS_ROW * 8 )
#define SCROLL_AREA_WIDTH_PX		( SCROLL_AREA_WIDTH * 8 )
#define SCROLL_AREA_HEIGHT_PX		( SCROLL_AREA_HEIGHT * 8 )

#define SCROLL_AREA_MIN_X		SCROLL_AREA_POS_X
#define SCROLL_AREA_MAX_X		( SCROLL_AREA_POS_X + SCROLL_AREA_WIDTH_PX - 1 )
#define SCROLL_AREA_MIN_Y		SCROLL_AREA_POS_Y
#define SCROLL_AREA_MAX_Y		( SCROLL_AREA_POS_Y + SCROLL_AREA_HEIGHT_PX - 1 )

// the tiles that are used for drawing the map have this dimensions
// there is a row of this height at the top of the offscreen to draw the
// new tiles that are coming into view
#define SCROLL_AREA_TOP_TILE_HEIGHT	2
#define SCROLL_AREA_TOP_TILE_WIDTH	2
#define SCROLL_AREA_REAL_HEIGHT		( SCROLL_AREA_HEIGHT + SCROLL_AREA_TOP_TILE_HEIGHT )

// the scroll speed, i.e. the number of pixels scrolled down on each round
#define SCROLL_PIXELS			1

// SP1 scroll area
struct sp1_Rect scroll_area = { SCROLL_AREA_POS_ROW, SCROLL_AREA_POS_COL, SCROLL_AREA_WIDTH, SCROLL_AREA_HEIGHT };

///////////////////////////////////
//
// TILE DEFINITIONS
//
///////////////////////////////////

// a test diamond tile - must have the dimensions mentioned above for the top tile row
uint8_t tile[ 8 * SCROLL_AREA_TOP_TILE_HEIGHT * SCROLL_AREA_TOP_TILE_WIDTH ] = {
  1,2,4,8,16,32,64,128,
  128,64,32,16,8,4,2,1,
  128,64,32,16,8,4,2,1,
  1,2,4,8,16,32,64,128
};

uint8_t diamond_tile[] = {
    0x01, 0x02, 0x05, 0x0b, 0x16, 0x2d, 0x58, 0xb1, 0xb5, 0x58, 0x2c, 0x16, 0x0b, 0x05, 0x02, 0x01, 
    0x80, 0x40, 0xa0, 0xd0, 0x68, 0x34, 0x1a, 0xad, 0x8d, 0x1a, 0xb4, 0x68, 0xd0, 0xa0, 0x40, 0x80
};

///////////////////////////////////
//
// SPRITE DEFINITIONS
//
///////////////////////////////////

#define SPRITE_WIDTH		2
#define SPRITE_HEIGHT		2
#define SPRITE_WIDTH_PX		( SPRITE_WIDTH * 8 )
#define SPRITE_HEIGHT_PX	( SPRITE_HEIGHT * 8 )
uint8_t ball_sprite[] = {
    0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 
    0xfc, 0x00, 0xf8, 0x03, 0xe0, 0x0f, 0xc0, 0x19, 0x80, 0x31, 0x80, 0x23, 0x00, 0x67, 0x00, 0x7f, 
    0x00, 0x7f, 0x00, 0x7f, 0x80, 0x3f, 0x80, 0x3f, 0xc0, 0x1f, 0xe0, 0x0f, 0xf8, 0x03, 0xfc, 0x00, 
    0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 
    0x3f, 0x00, 0x1f, 0xc0, 0x07, 0xf0, 0x03, 0xf8, 0x01, 0xfc, 0x01, 0xfc, 0x00, 0xfe, 0x00, 0xfe, 
    0x00, 0xfe, 0x00, 0xfe, 0x01, 0xfc, 0x01, 0xfc, 0x03, 0xf8, 0x07, 0xf0, 0x1f, 0xc0, 0x3f, 0x00, 
    0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00
};

#define SPRITE_START_ADDRESS	( &ball_sprite[ 16 ] )

struct sprite_s {
    int16_t pos_x, pos_y;
    int8_t dx, dy;
    uint8_t z_plane;
    struct sp1_ss *sprite;
};

// number of sprites to use in the demo
// up to 16 can be tested just changing this value (see the array below),
// but you can add more to the following array if you need more (unlikely!)
#define NUM_SPRITES	6

// sprite data
// coordinates are absolute in pixels
struct sprite_s sprites[] = {
    { .pos_x = SCROLL_AREA_MIN_X + 10, .pos_y = SCROLL_AREA_MIN_Y + 70, .dx =  1, .dy =  2, .z_plane = 10, .sprite = NULL  },
    { .pos_x = SCROLL_AREA_MIN_X + 20, .pos_y = SCROLL_AREA_MIN_Y + 40, .dx = -1, .dy =  2, .z_plane = 11, .sprite = NULL  },
    { .pos_x = SCROLL_AREA_MIN_X + 40, .pos_y = SCROLL_AREA_MIN_Y + 20, .dx =  1, .dy = -2, .z_plane = 12, .sprite = NULL  },
    { .pos_x = SCROLL_AREA_MIN_X + 70, .pos_y = SCROLL_AREA_MIN_Y + 10, .dx = -1, .dy = -2, .z_plane = 13, .sprite = NULL  },
    { .pos_x = SCROLL_AREA_MIN_X + 16, .pos_y = SCROLL_AREA_MIN_Y + 82, .dx =  2, .dy =  1, .z_plane = 14, .sprite = NULL  },
    { .pos_x = SCROLL_AREA_MIN_X + 28, .pos_y = SCROLL_AREA_MIN_Y + 50, .dx = -1, .dy =  2, .z_plane = 15, .sprite = NULL  },
    { .pos_x = SCROLL_AREA_MIN_X + 50, .pos_y = SCROLL_AREA_MIN_Y + 28, .dx =  1, .dy = -2, .z_plane = 16, .sprite = NULL  },
    { .pos_x = SCROLL_AREA_MIN_X + 82, .pos_y = SCROLL_AREA_MIN_Y + 16, .dx = -1, .dy = -2, .z_plane = 17, .sprite = NULL  },
    { .pos_x = SCROLL_AREA_MIN_X +  5, .pos_y = SCROLL_AREA_MIN_Y + 20, .dx =  2, .dy =  1, .z_plane = 18, .sprite = NULL  },
    { .pos_x = SCROLL_AREA_MIN_X + 10, .pos_y = SCROLL_AREA_MIN_Y + 15, .dx = -1, .dy =  2, .z_plane = 19, .sprite = NULL  },
    { .pos_x = SCROLL_AREA_MIN_X + 15, .pos_y = SCROLL_AREA_MIN_Y + 10, .dx =  2, .dy = -2, .z_plane = 20, .sprite = NULL  },
    { .pos_x = SCROLL_AREA_MIN_X + 20, .pos_y = SCROLL_AREA_MIN_Y +  5, .dx = -1, .dy = -2, .z_plane = 21, .sprite = NULL  },
    { .pos_x = SCROLL_AREA_MIN_X + 25, .pos_y = SCROLL_AREA_MIN_Y + 40, .dx =  2, .dy =  1, .z_plane = 22, .sprite = NULL  },
    { .pos_x = SCROLL_AREA_MIN_X + 30, .pos_y = SCROLL_AREA_MIN_Y + 35, .dx = -1, .dy =  2, .z_plane = 23, .sprite = NULL  },
    { .pos_x = SCROLL_AREA_MIN_X + 35, .pos_y = SCROLL_AREA_MIN_Y + 30, .dx =  1, .dy = -1, .z_plane = 24, .sprite = NULL  },
    { .pos_x = SCROLL_AREA_MIN_X + 40, .pos_y = SCROLL_AREA_MIN_Y + 25, .dx = -1, .dy = -2, .z_plane = 25, .sprite = NULL  },
};

///////////////////////////////////
//
// OFFSCREEN BUFFER DEFINITIONS
//
///////////////////////////////////

// The offscreen buffer. There are SCROLL_PIXELS top pixel lines on each column which are
// initialized to 0 so that when scrolling down the top non-visible tile row is erased
#define OFFSCREEN_BUFFER_SIZE	( SCROLL_AREA_WIDTH * SCROLL_PIXELS + SCROLL_AREA_WIDTH * SCROLL_AREA_REAL_HEIGHT * 8 )
uint8_t offscreen[ OFFSCREEN_BUFFER_SIZE ];

// Tables of start and end addresses for each column; initialized at start
uint8_t *offscreen_column_start_address[ SCROLL_AREA_WIDTH ];
uint8_t *offscreen_column_end_address[ SCROLL_AREA_WIDTH ];

uint16_t cell_address_offset( uint8_t row, uint8_t col ) {
  return ( 8 * ( SCROLL_AREA_REAL_HEIGHT * col + row ) + SCROLL_PIXELS * ( col + 1 ) );
}

/////////////////////////
//
// HEAP FUNCTIONS
//
/////////////////////////

long heap;

// heap initialization - see SP1 examples
void init_heap( void ) {
    heap = 0L;
    sbrk( ( void * ) 42000, 8000);
}

void *u_malloc(uint16_t size) {
    return malloc( size );
}

void u_free(void *addr) {
     free( addr );
}

/////////////////////////////
//
// INVALIDATION FUNCTIONS
//
/////////////////////////////

#define NO_RANGE (-128)

struct column_invalidation_range_s {
  // offset from the scroll area top
  // values can be negative, -SCROLL_AREA_TOP_TILE_HEIGHT-1 to (SCROLL_AREA_HEIGHT-1)
  // NO_RANGE is -128 (0x80)
  int8_t start_row;
  int8_t end_row;
};

struct column_invalidation_range_s column_invalidations[ SCROLL_AREA_WIDTH ];

void init_column_invalidation_ranges( void ) {
  uint8_t i;
  for ( i = 0; i < SCROLL_AREA_WIDTH; i++ ) {
    column_invalidations[ i ].start_row = NO_RANGE;
    column_invalidations[ i ].end_row = NO_RANGE;
  }
}

struct sp1_Rect dirty;
void invalidate_dirty_scrollarea( void ) {
//  sp1_Invalidate( &scroll_area );
  uint8_t i, real_start_row;
  for ( i = 0; i < SCROLL_AREA_WIDTH; i++ )
    // if the invalidation area is inside the scroll area...
    if ( column_invalidations[ i ].end_row >= 0 ) {
      real_start_row = ( column_invalidations[ i ].start_row >= 0 ? column_invalidations[ i ].start_row : 0 );
      dirty.row = real_start_row;
      dirty.col = SCROLL_AREA_POS_COL + i;
      dirty.width = 1;	// width: 1 cell
      dirty.height = column_invalidations[ i ].end_row - real_start_row + 1;
      sp1_Invalidate( &dirty );
    }
}

void move_down_column_invalidation_ranges( void ) {
  uint8_t i;
  for ( i = 0; i < SCROLL_AREA_WIDTH; i++ ) {
    // If end_row is not the bottom row, increment it, else leave it alone
    if ( ( column_invalidations[ i ].end_row >= 0 ) && ( column_invalidations[ i ].end_row < ( SCROLL_AREA_HEIGHT - 1 ) ) )
      column_invalidations[ i ].end_row++;
    // if start_row is out of scroll area, reset both start_row and end_row
    if ( ++(column_invalidations[ i ].start_row) >= SCROLL_AREA_HEIGHT ) {
      column_invalidations[ i ].start_row = NO_RANGE;
      column_invalidations[ i ].end_row = NO_RANGE;
    }
  }
}

/////////////////////////
//
// SCROLLING FUNCTIONS
//
/////////////////////////

// global scroll counter
uint8_t scroll_counter;

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
void scroll_down_column_unrolled( uint8_t *column_end ) __z88dk_fastcall __naked {
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

// function to scroll a column SCROLL_PIXELS pixels down - stack version
// the number of transfer loops must be exactly SCROLL_AREA_REAL_HEIGHT
// must be called with INTs disabled!
uint16_t saved_sp;
void scroll_down_column_stack( uint8_t *column_end ) __z88dk_fastcall __naked {
__asm

    ;; we will be using HL as the dst ptr and HL' as the src ptr
    inc hl	;; stack is predec/postinc
    push hl	;; HL already contains the final column address (dst address)
    exx
    pop	hl
    ld de,- ( 8 + SCROLL_PIXELS	)
    add hl,de

    ld (_saved_sp),sp	;; save stack pointer

    ;; we enter the loop with alt regs
REPT SCROLL_AREA_REAL_HEIGHT
    ld sp,hl	;; load src address in SP (from HL')
    pop de	;; load 8 bytes from source
    pop bc
    exx
    pop de
    pop bc

    ld sp, hl	;; load dst address in SP (from HL)
    push bc	;; store 8 bytes ro dest
    push de

    ld de,-8	;; update dst for next iter
    add hl,de

    exx		;; back to alt regs
    push bc
    push de

    ld de,-8	;; update src for next iter
    add hl,de
ENDR
    exx		;; back to normal regs
    ld sp, (_saved_sp)	;; restore stack pointer
    ret
__endasm;
}

// scroll the whole scroll area one pixel down
void scroll_down_area( void ) {
  uint8_t i;
  intrinsic_di();
  for ( i = 0; i < SCROLL_AREA_WIDTH; i++ )
//    scroll_down_column_loop( offscreen_column_end_address[ i ] );	// ~2697 T-states
    scroll_down_column_unrolled( offscreen_column_end_address[ i ] );	// ~2062 T-states
//    scroll_down_column_stack( offscreen_column_end_address[ i ] );	// ~2336 T-states
  intrinsic_ei();
  // adjust scroll counter
  scroll_counter++;
}

//////////////////
//
// TILE DRAWING
//
//////////////////

// draw a tile on the top non-visible row, at the given column
void draw_tile_on_top_row( uint8_t *tile, uint8_t col ) {
  uint8_t i,j;

  for ( j = 0; j < SCROLL_AREA_TOP_TILE_WIDTH; j++ ) {

    // transfer the column pixels
    for ( i = 0; i < SCROLL_AREA_TOP_TILE_HEIGHT * 8; i++ )
      offscreen_column_start_address[ col + j ][ i ] = tile[ j * SCROLL_AREA_TOP_TILE_HEIGHT * 8 + i ];

    // modify invalidation ranges for the column
    column_invalidations[ col + j ].start_row = -SCROLL_AREA_TOP_TILE_HEIGHT -1;
    if ( column_invalidations[ col + j ].end_row == NO_RANGE )
      column_invalidations[ col + j ].end_row = 0;
  }
}

// draws the top row of tiles.  This is the function where the map can be
// explored and be brought into view
void draw_top_row_of_tiles( void ) {

  static uint8_t tile_counter = 0;

  draw_tile_on_top_row( diamond_tile, tile_counter * 2 );
  if ( ++tile_counter == 8 )
    tile_counter = 0;
}

/////////////////////////
//
// UTILITY FUNCTIONS
//
/////////////////////////

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
  // initialize scroll counter
  scroll_counter = 0;
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
          PAPER_YELLOW | INK_BLACK,	// attr
          ( uint16_t ) &offscreen[ cell_address_offset( r + SCROLL_AREA_TOP_TILE_HEIGHT, c ) ] );	// pointer
}

// initialize SP1 sprites
void init_sprites( void ) {
    uint8_t i,c;
    struct sp1_ss *s;
    for ( i = 0; i < NUM_SPRITES; i++ ) {
      // create sprite and first column
      s = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE,
        SPRITE_HEIGHT + 1,       // number of rows including the blank bottom one
        0,              // left column graphic offset
        0               // z-plane
        );
      // add columns
      for ( c = 1; c <= SPRITE_WIDTH - 1; c++ )
        sp1_AddColSpr(s,
            SP1_DRAW_MASK2,             // drawing function
            0,                          // sprite type
            ( SPRITE_HEIGHT + 1 ) * 16 * c,      // nth column graphic offset; 16 is because type is 2BYTE (mask+graphic)
            sprites[ i ].z_plane                           // z-plane
        );
      // add final empty column
      sp1_AddColSpr(s, SP1_DRAW_MASK2RB, 0, 0, 0);
      // save
      sprites[ i ].sprite = s;
    }
}

// move the sprites with a simple bouncing algorithm
void move_sprites( void ) {
  uint8_t i;
  struct sprite_s *s;
  for ( i = 0; i < NUM_SPRITES; i++ ) {
    s = &sprites[ i ];

    // update and check X
    s->pos_x += s->dx;
    if ( s->pos_x > SCROLL_AREA_MAX_X - SPRITE_WIDTH_PX + 1 ) {
      // bounce right
      s->pos_x = SCROLL_AREA_MAX_X - SPRITE_WIDTH_PX + 1;
      s->dx = -s->dx;
    }
    if ( s->pos_x < SCROLL_AREA_MIN_X ) {
      // bounce left
      s->pos_x = SCROLL_AREA_MIN_X;
      s->dx = -s->dx;
    }

    // update and check Y
    s->pos_y += s->dy;
    if ( s->pos_y > SCROLL_AREA_MAX_Y - SPRITE_HEIGHT_PX + 1 ) {
      // bounce bottom
      s->pos_y = SCROLL_AREA_MAX_Y - SPRITE_HEIGHT_PX + 1;
      s->dy = -s->dy;
    }
    if ( s->pos_y < SCROLL_AREA_MIN_Y ) {
      // bounce up
      s->pos_y = SCROLL_AREA_MIN_Y;
      s->dy = -s->dy;
    }

    // move sprite to new position
    sp1_MoveSprPix( s->sprite, &scroll_area, SPRITE_START_ADDRESS, s->pos_x, s->pos_y );
  }
}

///////////////////
// main function
///////////////////

void dump_invalidations( void ) {
  uint8_t i;
  gotoxy(0,0);
  for ( i = 0; i < SCROLL_AREA_HEIGHT; i++ )
    printf( "%02d:S:%d E:%d      \n", i,column_invalidations[i].start_row,column_invalidations[i].end_row);
}

void main( void ) {

    zx_border(INK_BLACK);

    // initializations
    init_heap();
    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
      PAPER_BLACK | INK_GREEN, ' ');
    init_address_tables();
    init_scroll_area();
    init_tile_map();
    init_sprites();
    init_column_invalidation_ranges();

    // make sure the scroll area is cleared at start
    sp1_Invalidate( &scroll_area );

    // main loop
    while (1) {

//      dump_invalidations();

      // draw tiles if top row has been completely scrolled down
      // this updates the invalidation ranges for all affected columns
      if ( ! ( scroll_counter % (SCROLL_AREA_TOP_TILE_HEIGHT * 8 ) ) )
        draw_top_row_of_tiles();

      // move down the column invalidation ranges if needed
      if ( ( scroll_counter % 8 ) == 7 )
        move_down_column_invalidation_ranges();

      // do whatever we want with the background
      // this increments scroll_counter!
      scroll_down_area();

      // invalidate dirty areas in scroll area
      invalidate_dirty_scrollarea();

      // move the sprites
      move_sprites();

      // now sync with vert retrace and redraw
      intrinsic_halt();
      sp1_UpdateNow();
    }
}
