#include "build.h"

///////////////////////////////////////
//
// TILE AND INVALIDATION DEFINITIONS
//
///////////////////////////////////////

#define INVAL_NO_RANGE (-128)

struct column_invalidation_range_s {
  // offset from the scroll area top
  // values can be negative, -SCROLL_AREA_TOP_TILE_HEIGHT-1 to (SCROLL_AREA_HEIGHT-1)
  // INVAL_NO_RANGE is -128 (0x80)
  int8_t start_row;
  int8_t end_row;
};

struct column_invalidation_range_s column_invalidations[ SCROLL_AREA_WIDTH ];

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

uint8_t area2_tile[] = {
    0x18, 0x24, 0x42, 0x81, 0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81, 0x81, 0x42, 0x24, 0x18, 
};

uint8_t area3_tile[] = {
    0x18, 0x24, 0x24, 0x42, 0x42, 0x99, 0xa5, 0xa5, 0xa5, 0xa5, 0x99, 0x42, 0x42, 0x24, 0x24, 0x18, 
};

/////////////////////////////////////
//
// TILE FUNCTIONS
//
/////////////////////////////////////

// initialize SP1 tile map
void init_tile_map( void ) {
    uint8_t r,c;
    // setup the SP1 tile map to point to the cells in the offscreen buffer
    // the top 2 rows are reserved for drawing and not visible
    // the tile array is configured in columns!
    for ( r = 0; r < SCROLL_AREA_HEIGHT; r++ ) {

      // Scroll zone 1 (AREA_1)
      for ( c = 2; c < SCROLL_AREA_WIDTH - 2; c++ )
        sp1_PrintAt( SCROLL_AREA_POS_ROW + r, SCROLL_AREA_POS_COL + c,		// screen position
          PAPER_BLACK | INK_WHITE,	// attr
          ( uint16_t ) &offscreen[ cell_address_offset( r + SCROLL_AREA_TOP_TILE_HEIGHT, c ) ] );	// pointer

      // scroll zone 2 (AREA_2)
      sp1_PrintAt( SCROLL_AREA_POS_ROW + r, SCROLL_AREA_POS_COL + SCROLL_AREA2_LEFT_COL,		// screen position
        PAPER_BLUE | INK_CYAN | BRIGHT,	// attr
         ( uint16_t ) &offscreen[ cell_address_offset( r + SCROLL_AREA_TOP_TILE_HEIGHT, SCROLL_AREA2_LEFT_COL ) ] );	// pointer
      sp1_PrintAt( SCROLL_AREA_POS_ROW + r, SCROLL_AREA_POS_COL + SCROLL_AREA2_RIGHT_COL,		// screen position
        PAPER_BLUE | INK_CYAN | BRIGHT,	// attr
         ( uint16_t ) &offscreen[ cell_address_offset( r + SCROLL_AREA_TOP_TILE_HEIGHT, SCROLL_AREA2_RIGHT_COL ) ] );	// pointer

      // scroll zone 3 (AREA_3)
      sp1_PrintAt( SCROLL_AREA_POS_ROW + r, SCROLL_AREA_POS_COL + SCROLL_AREA3_LEFT_COL,		// screen position
        PAPER_BLUE | INK_YELLOW | BRIGHT,	// attr
         ( uint16_t ) &offscreen[ cell_address_offset( r + SCROLL_AREA_TOP_TILE_HEIGHT, SCROLL_AREA3_LEFT_COL ) ] );	// pointer
      sp1_PrintAt( SCROLL_AREA_POS_ROW + r, SCROLL_AREA_POS_COL + SCROLL_AREA3_RIGHT_COL,		// screen position
        PAPER_BLUE | INK_YELLOW | BRIGHT,	// attr
         ( uint16_t ) &offscreen[ cell_address_offset( r + SCROLL_AREA_TOP_TILE_HEIGHT, SCROLL_AREA3_RIGHT_COL ) ] );	// pointer
    }
}

// draw a tile on the top non-visible row, at the given column
void draw_tile_on_top_row( uint8_t *tile, uint8_t col ) {
  uint8_t i,j;

  for ( j = 0; j < SCROLL_AREA_TOP_TILE_WIDTH; j++ ) {

    // transfer the column pixels
    for ( i = 0; i < SCROLL_AREA_TOP_TILE_HEIGHT * 8; i++ )
      offscreen_column_start_address[ col + j ][ i ] = tile[ j * SCROLL_AREA_TOP_TILE_HEIGHT * 8 + i ];

    // modify invalidation ranges for the column
    column_invalidations[ col + j ].start_row = -SCROLL_AREA_TOP_TILE_HEIGHT;

    // if the bottom row is INVAL_NO_RANGE, set it to the top row
    // if the bottom is something else, keep it
    if ( column_invalidations[ col + j ].end_row == INVAL_NO_RANGE )
      column_invalidations[ col + j ].end_row = 0;
  }
}

void draw_half_tile_on_top_row( uint8_t *tile, uint8_t col ) {
  uint8_t i;

  // transfer the column pixels
  for ( i = 0; i < SCROLL_AREA_TOP_TILE_HEIGHT * 8; i++ )
    offscreen_column_start_address[ col ][ i ] = tile[ i ];

  // modify invalidation ranges for the column
  column_invalidations[ col ].start_row = -SCROLL_AREA_TOP_TILE_HEIGHT;

  // if the bottom row is INVAL_NO_RANGE, set it to the top row
  // if the bottom is something else, keep it
  if ( column_invalidations[ col ].end_row == INVAL_NO_RANGE )
    column_invalidations[ col ].end_row = 0;
}

// draws the top row of tiles.  This is the function where the map can be
// explored and be brought into view
void draw_top_row_of_tiles( void ) {

  static uint8_t tile_counter = 0;

  // main scroll zone (AREA_1)
  if ( ! ( scroll_counter_1 % (SCROLL_AREA_TOP_TILE_HEIGHT * 8 ) ) ) {
    draw_tile_on_top_row( diamond_tile, 2 + tile_counter * 2 );
    if ( ++tile_counter == 6 )
      tile_counter = 0;
  }

  // scroll zone 2 (AREA_2)
  if ( ! ( scroll_counter_2 % (SCROLL_AREA_TOP_TILE_HEIGHT * 8 ) ) ) {
    draw_half_tile_on_top_row( area2_tile, SCROLL_AREA2_LEFT_COL );
    draw_half_tile_on_top_row( area2_tile, SCROLL_AREA2_RIGHT_COL );
  }

  // scroll zone 3 (AREA_3)
  if ( ! ( scroll_counter_3 % (SCROLL_AREA_TOP_TILE_HEIGHT * 8 ) ) ) {
    draw_half_tile_on_top_row( area3_tile, SCROLL_AREA3_LEFT_COL );
    draw_half_tile_on_top_row( area3_tile, SCROLL_AREA3_RIGHT_COL );
  }

}

/////////////////////////////
//
// INVALIDATION FUNCTIONS
//
/////////////////////////////

void init_column_invalidation_ranges( void ) {
  uint8_t i;
  for ( i = 0; i < SCROLL_AREA_WIDTH; i++ ) {
    column_invalidations[ i ].start_row = INVAL_NO_RANGE;
    column_invalidations[ i ].end_row = INVAL_NO_RANGE;
  }
}

struct sp1_Rect dirty;
void invalidate_dirty_scrollarea( void ) {
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

// refactored code from function below
void move_down_single_column_invalidation( uint8_t col ) {
      // If end_row is not the bottom row, increment it, else leave it alone
    if ( ( column_invalidations[ col ].end_row >= 0 ) && ( column_invalidations[ col ].end_row < ( SCROLL_AREA_HEIGHT - 1 ) ) )
      column_invalidations[ col ].end_row++;

    // if start_row is out of scroll area, reset both start_row and end_row
    if ( column_invalidations[ col ].start_row != INVAL_NO_RANGE ) {
      if ( ++column_invalidations[ col ].start_row >= SCROLL_AREA_HEIGHT ) {
        column_invalidations[ col ].start_row = INVAL_NO_RANGE;
        column_invalidations[ col ].end_row = INVAL_NO_RANGE;
      }
    }
}

void move_down_column_invalidation_ranges( void ) {
  uint8_t i;

  // main scroll zone (AREA_1)
  if ( ! ( scroll_counter_1 % 8 ) )
    for ( i = 2; i < SCROLL_AREA_WIDTH-2; i++ )
      move_down_single_column_invalidation( i );


  // scroll zone 2 (AREA_2)
  if ( ! ( scroll_counter_2 % 8 ) ) {
    move_down_single_column_invalidation( 1 );
    move_down_single_column_invalidation( SCROLL_AREA_WIDTH-2 );
  }

  // scroll zone 3 (AREA_3)
  if ( ! ( scroll_counter_3 % 8 ) ) {
    move_down_single_column_invalidation( 0 );
    move_down_single_column_invalidation( SCROLL_AREA_WIDTH-1 );
  }


}

void dump_invalidations( void ) {
  uint8_t i;
  gotoxy(0,0);
  for ( i = 0; i < SCROLL_AREA_HEIGHT; i++ )
    printf( "%02d:S:%d E:%d      \n", i,column_invalidations[i].start_row,column_invalidations[i].end_row);
}

