#include "build.h"

// some tile definitions for the map

// blank tile
uint8_t blank_tile_2x2_data[ 32 ] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
struct tile_s blank_tile_2x2 = { 2, 2, blank_tile_2x2_data };

// other tiles
struct tile_s diamond	= { 2, 2, diamond_tile };
struct tile_s clubs	= { 2, 2, clubs_tile };
struct tile_s spades	= { 2, 2, spades_tile };
struct tile_s hearts	= { 2, 2, hearts_tile };
struct tile_s wallh	= { 2, 2, wallh_tile };
struct tile_s cornerbl	= { 2, 2, cornerbl_tile };
struct tile_s wallv	= { 2, 2, wallv_tile };
struct tile_s cornerbr	= { 2, 2, cornerbr_tile };
struct tile_s cornertl	= { 2, 2, cornertl_tile };
struct tile_s cornertr	= { 2, 2, cornertr_tile };
struct tile_s walltt	= { 2, 2, walltt_tile };
struct tile_s walltb	= { 2, 2, walltb_tile };
struct tile_s walltl	= { 2, 2, walltl_tile };
struct tile_s walltr	= { 2, 2, walltr_tile };
struct tile_s wallhre	= { 2, 2, wallhre_tile };
struct tile_s wallhle	= { 2, 2, wallhle_tile };
struct tile_s wallvte	= { 2, 2, wallvte_tile };
struct tile_s wallvbe	= { 2, 2, wallvbe_tile };
struct tile_s sand	= { 2, 2, sand_tile };
struct tile_s flower	= { 2, 2, flower_tile };

struct tile_s *tile_bank[] = {
    &diamond,	// A
    &clubs,	// B
    &spades,	// C
    &hearts,	// D
    &wallh,	// E
    &cornerbl,	// F
    &wallv,	// G
    &cornerbr,	// H
    &cornertl,	// I
    &cornertr,	// J
    &walltt,	// K
    &walltb,	// L
    &walltl,	// M
    &walltr,	// N
    &wallhre,	// O
    &wallhle,	// P
    &wallvte,	// Q
    &wallvbe,	// R
    &sand,	// S
    &flower,	// T
};

void init_scroll_map( void ) {
}

void scroll_map_set_viewport_xy( uint16_t x, uint16_t y ) {
    scroll_map.viewport_pos.x = x;
    scroll_map.viewport_pos.y = y;
}

struct tile_s *scroll_map_get_tile_at( uint8_t row, uint8_t col ) {
    static uint8_t tile_code;

    tile_code = scroll_map.data[ row * SCROLL_MAP_WIDTH + col ];
    switch ( tile_code ) {
        case ' ':
            return &blank_tile_2x2;
            break;
        default:
            return tile_bank[ tile_code - 'A' ];
            break;
    }
}

void scroll_map_draw_hidden_top_row( void ) {
    static uint8_t i,offset,map_offset;
    static uint16_t row,col,width;

    // scroll map coordinates
    row = scroll_map.viewport_pos.y / SCROLL_MAP_TILE_HEIGHT_PIX - 1;
    col = scroll_map.viewport_pos.x / SCROLL_MAP_TILE_WIDTH_PIX;
    width = SCROLL_AREA_EXTENDED_WIDTH / SCROLL_MAP_TILE_WIDTH;

    // offscreen cell offset
    offset = 0;

    // map cell offset
    map_offset = 1;

    // adjust offsets and number of tiles to draw
    if ( scroll_map.viewport_pos.x < SCROLL_MAP_TILE_WIDTH_PIX ) {
        offset = SCROLL_MAP_TILE_WIDTH;
        map_offset = 0;
        width--;
    }
    if ( scroll_map.viewport_pos.x >= SCROLL_MAP_WIDTH_PIX - SCROLL_AREA_WIDTH_PIX - SCROLL_MAP_TILE_WIDTH_PIX ) {
        width--;
    }    

//    gotoxy( 0,20 );
//    printf( "TOP: r=%-2d c=%-2d w=%-2d o=%-2d", row, col, width, offset );
    for ( i = 0; i < width; i++ )
        tile_draw_offscreen(	// offscreen cell coordinates
            0,							// row = 0
            SCROLL_MAP_TILE_WIDTH * i + offset,			// col = loop
            scroll_map_get_tile_at( row, col + i - map_offset)	// use proper tile from map
        );
}

void scroll_map_draw_hidden_bottom_row( void ) {
    static uint8_t i,offset,map_offset;
    static uint16_t row,col,width;

    // scroll map coordinates
    row = scroll_map.viewport_pos.y / SCROLL_MAP_TILE_HEIGHT_PIX + SCROLL_AREA_HEIGHT / SCROLL_MAP_TILE_HEIGHT;
    col = scroll_map.viewport_pos.x / SCROLL_MAP_TILE_WIDTH_PIX;
    width = SCROLL_AREA_EXTENDED_WIDTH / SCROLL_MAP_TILE_WIDTH;

    // offscreen cell offset
    offset = 0;

    // map cell offset
    map_offset = 1;

    // adjust offsets and number of tiles to draw
    if ( scroll_map.viewport_pos.x < SCROLL_MAP_TILE_WIDTH_PIX ) {
        offset = SCROLL_MAP_TILE_WIDTH;
        map_offset = 0;
        width--;
    }
    if ( scroll_map.viewport_pos.x >= SCROLL_MAP_WIDTH_PIX - SCROLL_AREA_WIDTH_PIX - SCROLL_MAP_TILE_WIDTH_PIX ) {
        width--;
    }    

//    gotoxy( 0,21 );
//    printf( "BTM: r=%-2d c=%-2d w=%-2d o=%-2d", row, col, width, offset );
    for ( i = 0; i < width; i++ )
        tile_draw_offscreen(	// offscreen cell coordinates
            SCROLL_AREA_EXTENDED_HEIGHT - SCROLL_MAP_TILE_HEIGHT,	// row = bottom - 1 tile height
            SCROLL_MAP_TILE_WIDTH * i + offset,				// col = loop
            scroll_map_get_tile_at( row, col + i - map_offset)		// use proper tile from map
        );
}

void scroll_map_draw_hidden_left_col( void ) {
    static uint8_t i,offset,map_offset;
    static uint16_t row,col,height;

    // scroll map coordinates
    row = scroll_map.viewport_pos.y / SCROLL_MAP_TILE_HEIGHT_PIX;
    col = scroll_map.viewport_pos.x / SCROLL_MAP_TILE_WIDTH_PIX - 1;
    height = SCROLL_AREA_EXTENDED_HEIGHT / SCROLL_MAP_TILE_HEIGHT;

    // offscreen cell offset
    offset = 0;

    // map cell offset
    map_offset = 1;

    // adjust offsets and number of tiles to draw
    if ( scroll_map.viewport_pos.y < SCROLL_MAP_TILE_HEIGHT_PIX ) {
        offset = SCROLL_MAP_TILE_HEIGHT;
        map_offset = 0;
        height--;
    }
    if ( scroll_map.viewport_pos.y >= SCROLL_MAP_HEIGHT_PIX - SCROLL_AREA_HEIGHT_PIX - SCROLL_MAP_TILE_HEIGHT_PIX ) {
        height--;
    }    

//    gotoxy( 0,22 );
//    printf( "LFT: r=%-2d c=%-2d h=%-2d o=%-2d", row, col, height, offset );
    for ( i = 0; i < height ; i++ )
        tile_draw_offscreen(	// offscreen cell coordinates
            SCROLL_MAP_TILE_HEIGHT * i + offset,		// row = loop
            0,							// col = 0
            scroll_map_get_tile_at( row + i - map_offset, col )	// use proper tile from map
        );
}

void scroll_map_draw_hidden_right_col( void ) {
    static uint8_t i,offset,map_offset;
    static uint16_t row,col,height;

    // scroll map coordinates
    row = scroll_map.viewport_pos.y / SCROLL_MAP_TILE_HEIGHT_PIX;
    col = scroll_map.viewport_pos.x / SCROLL_MAP_TILE_WIDTH_PIX + SCROLL_AREA_WIDTH / SCROLL_MAP_TILE_WIDTH;
    height = SCROLL_AREA_EXTENDED_HEIGHT / SCROLL_MAP_TILE_HEIGHT;

    // offscreen cell offset
    offset = 0;

    // map cell offset
    map_offset = 1;

    // adjust offsets and number of tiles to draw
    if ( scroll_map.viewport_pos.y < SCROLL_MAP_TILE_HEIGHT_PIX ) {
        offset = SCROLL_MAP_TILE_HEIGHT;
        map_offset = 0;
        height--;
    }
    if ( scroll_map.viewport_pos.y >= SCROLL_MAP_HEIGHT_PIX - SCROLL_AREA_HEIGHT_PIX - SCROLL_MAP_TILE_HEIGHT_PIX ) {
        height--;
    }

//    gotoxy( 0,23 );
//    printf( "RGT: r=%-2d c=%-2d h=%-2d o=%-2d", row, col, height, offset );
    for ( i = 0; i < height; i++ )
        tile_draw_offscreen(	// offscreen cell coordinates
            SCROLL_MAP_TILE_HEIGHT * i + offset,			// row = loop
            SCROLL_AREA_EXTENDED_WIDTH - SCROLL_MAP_TILE_WIDTH,		// col = right - 1 tile width
            scroll_map_get_tile_at( row + i - map_offset, col )		// use proper tile from map
        );
}

// When calling this function, viewport coordinates must be a multiple of
// SCROLL_MAP_TILE_HEIGHT_PIX and SCROLL_MAP_TILE_WIDTH_PIX (for the moment)

void scroll_map_draw_viewport( void ) {
    static uint8_t i,j;
    static uint16_t row,col;

    // scroll map coordinates
    row = scroll_map.viewport_pos.y / SCROLL_MAP_TILE_HEIGHT_PIX;
    col = scroll_map.viewport_pos.x / SCROLL_MAP_TILE_WIDTH_PIX;

    // draw viewport
    for ( j = 0; j < SCROLL_AREA_WIDTH / SCROLL_MAP_TILE_WIDTH; j++ )
        for ( i = 0; i < SCROLL_AREA_HEIGHT / SCROLL_MAP_TILE_HEIGHT; i++ )
            tile_draw_offscreen(	// offscreen cell coordinates
                ( i + 1 ) * SCROLL_MAP_TILE_HEIGHT,
                ( j + 1 ) * SCROLL_MAP_TILE_WIDTH,
                scroll_map_get_tile_at( row + i, col + j )
            );

    // draw hidden areas surrounding the viewport, if the current position
    // is not on the corners
    if ( row > 0 )
        scroll_map_draw_hidden_top_row();
    if ( row <= SCROLL_MAP_HEIGHT / SCROLL_MAP_TILE_HEIGHT - SCROLL_AREA_HEIGHT / SCROLL_MAP_TILE_HEIGHT )
        scroll_map_draw_hidden_bottom_row();
    if ( col > 0 )
        scroll_map_draw_hidden_left_col();
    if ( col <= SCROLL_MAP_WIDTH / SCROLL_MAP_TILE_WIDTH - SCROLL_AREA_WIDTH / SCROLL_MAP_TILE_WIDTH )
        scroll_map_draw_hidden_right_col();
}

void scroll_map_scroll_viewport( uint8_t dir, uint8_t num_pix ) {
//    gotoxy( 0, 19 );
//    printf( "VX:%-3d, VY:%-3d", scroll_map.viewport_pos.x, scroll_map.viewport_pos.y );

    // first we have to check if we need to redraw borders (all of them)
    // only then we can do the scroll (all that are requested)
    // if we do not do it in this order, scroll won't work correctly

    // Also, when redrawing the borders, we need to scroll them right (for
    // top and bottom) or down (for left and right) because of the
    // fractional coordinates: e.g.  for scroll right: if pos_x is multiple
    // of TILE_WIDTH (->forces redraw of left border), then we need to
    // scroll down the left border that has just been drawn by ( pos_y %
    // TILE_HEIGHT) pixels, so that the just redrawn border is pixel aligned
    // with what's inside the viewport

    // first, check if borders need to be redrawn
    if ( dir & DIR_UP ) {
        if ( scroll_map.viewport_pos.y >= num_pix ) {
            if ( ! ( scroll_map.viewport_pos.y % SCROLL_MAP_TILE_HEIGHT_PIX ) )
                scroll_map_draw_hidden_top_row();
        }
    }
    if ( dir & DIR_DOWN ) {
        if ( scroll_map.viewport_pos.y <= SCROLL_MAP_HEIGHT_PIX - SCROLL_AREA_HEIGHT_PIX - num_pix ) {
            if ( ! ( scroll_map.viewport_pos.y % SCROLL_MAP_TILE_HEIGHT_PIX ) )
                scroll_map_draw_hidden_bottom_row();
        }
    }
    if ( dir & DIR_LEFT ) {
        if ( scroll_map.viewport_pos.x >= num_pix ) {
            if ( ! ( scroll_map.viewport_pos.x % SCROLL_MAP_TILE_WIDTH_PIX ) )
                scroll_map_draw_hidden_left_col();
        }
    }
    if ( dir & DIR_RIGHT ) {
        if ( scroll_map.viewport_pos.x <= SCROLL_MAP_WIDTH_PIX - SCROLL_AREA_WIDTH_PIX - num_pix ) {
            if ( ! ( scroll_map.viewport_pos.x % SCROLL_MAP_TILE_WIDTH_PIX ) )
                scroll_map_draw_hidden_right_col();
        }
    }

    // after, do scroll if needed and update viewport coords
    if ( dir & DIR_UP ) {
        // FIX: scroll right top hidden row by viewport_pos.x % SCROLL_MAP_TILE_WIDTH_PIX
        if ( scroll_map.viewport_pos.y >= num_pix ) {
            offscreen_scroll_down_pixels( num_pix );
            scroll_map.viewport_pos.y -= num_pix;
        }
    }
    if ( dir & DIR_DOWN ) {
        // FIX: scroll right bottom hidden row by viewport_pos.x % SCROLL_MAP_TILE_WIDTH_PIX
        if ( scroll_map.viewport_pos.y <= SCROLL_MAP_HEIGHT_PIX - SCROLL_AREA_HEIGHT_PIX - num_pix ) {
            offscreen_scroll_up_pixels( num_pix );
            scroll_map.viewport_pos.y += num_pix;
        }
    }
    if ( dir & DIR_LEFT ) {
        // FIX: scroll down left hidden column by viewport_pos.x % SCROLL_MAP_TILE_WIDTH_PIX
        if ( scroll_map.viewport_pos.x >= num_pix ) {
            offscreen_scroll_right_pixels( num_pix );
            scroll_map.viewport_pos.x -= num_pix;
        }
    }
    if ( dir & DIR_RIGHT ) {
        // FIX: scroll down right hidden column by viewport_pos.x % SCROLL_MAP_TILE_WIDTH_PIX
        if ( scroll_map.viewport_pos.x <= SCROLL_MAP_WIDTH_PIX - SCROLL_AREA_WIDTH_PIX - num_pix ) {
            offscreen_scroll_left_pixels( num_pix );
            scroll_map.viewport_pos.x += num_pix;
        }
    }

}
