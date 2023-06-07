#pragma output CRT_ORG_CODE           = 0x5f00  // org of compile
#pragma output REGISTER_SP            = 0xd1d1  // just below ISR
#pragma output CRT_STACK_SIZE         = 208     // just enough

#pragma output CRT_ENABLE_CLOSE       = 0       // don't bother closing files on exit
#pragma output CRT_ENABLE_EIDI        = 1       // disable interrupts at start
#pragma output CRT_ENABLE_RESTART     = 1       // do not return to basic

#pragma output CLIB_MALLOC_HEAP_SIZE  = 0       // // heap: initialized manually
#pragma output CLIB_STDIO_HEAP_SIZE   = 0       // no stdio heap (no files)

// Memory map:
// $5F00 - $BFFF: code/data/bss
// $C001 - $CFFF: heap (manualy assigned)
// $D100 - $D1D1: stack
// $D1D2 - $FFFF: SP1 data

// For classic:
#include <spectrum.h>
#include <sprites/sp1.h>

// For Newlib:
//#include <arch/spectrum.h>
//#include <games/sp1.h>

#include <intrinsic.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include "config.h"

// program extern and function declarations

// global data
extern struct sp1_Rect scroll_area;
extern uint8_t scroll_counter_1;
extern uint8_t scroll_counter_2;
extern uint8_t scroll_counter_3;
extern uint8_t offscreen[];
extern uint8_t *offscreen_column_start_address[];

// initialization functions
void init_heap( void );
void init_address_tables( void );
void init_scroll_area( void );
void init_tile_map( void );
void init_sprites( void );
void init_column_invalidation_ranges( void );

// utility functions
void draw_top_row_of_tiles( void );
void scroll_down_area( void );
void invalidate_dirty_scrollarea( void );
void move_sprites( void );
void move_down_column_invalidation_ranges( void );
uint16_t cell_address_offset( uint8_t row, uint8_t col );
void dump_invalidations( void );
