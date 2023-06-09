#include "build.h"

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
    if ( s->pos_x > SCROLL_AREA1_MAX_X - SPRITE_WIDTH_PX + 1 ) {
      // bounce right
      s->pos_x = SCROLL_AREA1_MAX_X - SPRITE_WIDTH_PX + 1;
      s->dx = -s->dx;
    }
    if ( s->pos_x < SCROLL_AREA1_MIN_X ) {
      // bounce left
      s->pos_x = SCROLL_AREA1_MIN_X;
      s->dx = -s->dx;
    }

    // update and check Y
    s->pos_y += s->dy;
    if ( s->pos_y > SCROLL_AREA1_MAX_Y - SPRITE_HEIGHT_PX + 1 ) {
      // bounce bottom
      s->pos_y = SCROLL_AREA1_MAX_Y - SPRITE_HEIGHT_PX + 1;
      s->dy = -s->dy;
    }
    if ( s->pos_y < SCROLL_AREA1_MIN_Y ) {
      // bounce up
      s->pos_y = SCROLL_AREA1_MIN_Y;
      s->dy = -s->dy;
    }

    // move sprite to new position
    sp1_MoveSprPix( s->sprite, &scroll_area, SPRITE_START_ADDRESS, s->pos_x, s->pos_y );
  }
}

