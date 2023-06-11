#include "build.h"

// a test diamond tile - must have the dimensions mentioned above for the top tile row
uint8_t diamond_tile[] = {
    0x01, 0x02, 0x05, 0x0b, 0x16, 0x2d, 0x58, 0xb1, 0xb5, 0x58, 0x2c, 0x16, 0x0b, 0x05, 0x02, 0x01, 
    0x80, 0x40, 0xa0, 0xd0, 0x68, 0x34, 0x1a, 0xad, 0x8d, 0x1a, 0xb4, 0x68, 0xd0, 0xa0, 0x40, 0x80
};

////////////////////////////////////////////
//
// INVALIDATION DEFINITIONS AND FUNCTIONS
//
////////////////////////////////////////////

// a struct for tile positions.  row,col can be negative for tiles that
// are out of view. rect is updated every time the tile is moved down
struct tile_position_s {
  int8_t row,col;
  struct sp1_Rect rect;
};

// Maximum number of tiles on screen.  This depends on the map and/or the
// function that draw new tiles on top.  If a tool for compiling maps is
// ever created, this number can be calculated from the map layout.
//
// For the current map generation function, 1-2-3 tiles are generated per
// row, and there are 8+1 2-cell rows, so we would be safe with about 27 max
// tiles on screen.  Let's define some more to be able to experiment safely.
//
// This number must be 1-byte for the moment, so maximum of 255 visible
// tiles at any instant
#define MAX_NUM_TILES_ON_SCREEN		27
#define TILE_POSITION_QUEUE_SIZE	MAX_NUM_TILES_ON_SCREEN

// the invalidation queue struct, implemented as a ring buffer for efficiency
// head holds the position of the first used slot
// num_tiles holds the number of currently visible tiles
// the indexing on "positions" using "size" is always done modulo
// TILE_POSITION_QUEUE_SIZE, so it it autmoatically handled as a ring buffer
struct tile_position_queue_s {
  uint8_t head,num_tiles;
  struct tile_position_s positions[ TILE_POSITION_QUEUE_SIZE ];
};

// the single instance of the position queue
struct tile_position_queue_s position_queue;

void init_tile_position_queue( void ) {
  position_queue.head = 0;
  position_queue.num_tiles = 0;
}

void add_tile_position_to_queue( int8_t row, int8_t col, uint8_t width, uint8_t height ) {
  uint8_t new_pos = ( position_queue.head + position_queue.num_tiles ) % TILE_POSITION_QUEUE_SIZE;
  position_queue.positions[ new_pos ].row = row;
  position_queue.positions[ new_pos ].col = col;
  position_queue.positions[ new_pos ].rect.row = SCROLL_AREA_POS_ROW + ( row >= 0 ? row : 0 );
  position_queue.positions[ new_pos ].rect.col = SCROLL_AREA_POS_COL + col;
  position_queue.positions[ new_pos ].rect.width = width;
  position_queue.positions[ new_pos ].rect.height = height;
  position_queue.num_tiles++;
}

void move_down_tile_positions( void ) {
  uint8_t i, real_index, num_tiles, head;
  struct tile_position_s *tile_pos;
  if ( position_queue.num_tiles ) {

    // Values of head and num_tiles need to be saved at the start of the
    // loop, they may be modified by the instructions inside the loop and
    // they are used as loop exit conditions
    head = position_queue.head;
    num_tiles = position_queue.num_tiles;

    // Sweep the position records of currently visible tiles
    for ( i = 0; i < num_tiles; i++ ) {
      real_index = ( head + i ) % TILE_POSITION_QUEUE_SIZE;
      tile_pos = &position_queue.positions[ real_index ];

      // Move the position down one row.  If the new position is outside the
      // scrolling window visible any more, drop it from the queue
      if ( ++(tile_pos->row) == SCROLL_AREA_HEIGHT ) {

        // drop current record from queue: decrement the number of elements
        // and increment the head pointer in circular way
        position_queue.num_tiles--;
        if ( ++position_queue.head == TILE_POSITION_QUEUE_SIZE ) {
          position_queue.head = 0;
        }
      } else {

        // if it is not outside of the scrolling, update its invalidation rect too
        tile_pos->rect.row = SCROLL_AREA_POS_ROW + ( tile_pos->row >= 0 ? tile_pos->row : 0 );
        if ( tile_pos->rect.row + tile_pos->rect.height > SCROLL_AREA_POS_ROW + SCROLL_AREA_HEIGHT )
          tile_pos->rect.height = SCROLL_AREA_POS_ROW + SCROLL_AREA_HEIGHT - tile_pos->rect.row;
      }
    }
  }
}

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
  for ( c = 0; c < SCROLL_AREA_WIDTH; c++ )
    for ( r = 0; r < SCROLL_AREA_HEIGHT; r++ )
      sp1_PrintAt( SCROLL_AREA_POS_ROW + r, SCROLL_AREA_POS_COL + c,	// screen position
        PAPER_YELLOW | INK_BLACK,					// attr
        ( uint16_t ) &offscreen[ cell_address_offset( r + SCROLL_AREA_TOP_TILE_HEIGHT, c ) ] );	// pointer
}

// draw a tile on the top non-visible row, at the given column
void draw_tile_on_top_row( uint8_t *tile, uint8_t col ) {

  // transfer the tile pixels
  uint8_t i,j;
  for ( j = 0; j < SCROLL_AREA_TOP_TILE_WIDTH; j++ )
    // transfer bytes column-wise
    for ( i = 0; i < SCROLL_AREA_TOP_TILE_HEIGHT * 8; i++ )
      offscreen_column_start_address[ col + j ][ i ] = tile[ j * SCROLL_AREA_TOP_TILE_HEIGHT * 8 + i ];

  // add a tile position record to the queue
  add_tile_position_to_queue( -SCROLL_AREA_TOP_TILE_HEIGHT, col, SCROLL_AREA_TOP_TILE_WIDTH, SCROLL_AREA_TOP_TILE_HEIGHT+1 );
}

// draws the top row of tiles.  This is the function where the map can be
// explored and be brought into view
void draw_top_row_of_tiles( void ) {
  // uncomment more calls for higher tile density
  draw_tile_on_top_row( diamond_tile, 2 * ( rand() % (SCROLL_AREA_WIDTH / 2) ) );
//  draw_tile_on_top_row( diamond_tile, 2 * ( rand() % (SCROLL_AREA_WIDTH / 2 ) ) );
//  draw_tile_on_top_row( diamond_tile, 2 * ( rand() % (SCROLL_AREA_WIDTH / 2 ) ) );
}

/////////////////////////////
//
// INVALIDATION FUNCTIONS
//
/////////////////////////////

void invalidate_dirty_scrollarea( void ) {
  uint8_t i;
  if ( position_queue.num_tiles ) {
    for ( i = 0; i < position_queue.num_tiles; i++ ) {
      sp1_Invalidate( &position_queue.positions[ ( position_queue.head + i ) % TILE_POSITION_QUEUE_SIZE ].rect );
    }
  }
}

void dump_tile_positions( void ) {
  uint8_t i;
  gotoxy(0,0);
  printf( "head:%d, n:%d     \n",position_queue.head,position_queue.num_tiles );
///*
  for ( i=0; i < TILE_POSITION_QUEUE_SIZE; i++ )
    printf( " %02d: r:%d c:%d    \n   w:%d h:%d   \n",
      i,
      position_queue.positions[ i ].rect.row,
      position_queue.positions[ i ].rect.col,
      position_queue.positions[ i ].rect.width,
      position_queue.positions[ i ].rect.height
    );
//*/
}
