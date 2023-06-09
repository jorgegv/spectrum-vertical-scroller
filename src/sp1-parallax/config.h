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

#define SCROLL_AREA1_POS_X		( ( SCROLL_AREA_POS_COL + 2 ) * 8 )
#define SCROLL_AREA1_POS_Y		( SCROLL_AREA_POS_ROW * 8 )
#define SCROLL_AREA1_WIDTH_PX		( ( SCROLL_AREA_WIDTH - 4 ) * 8 )
#define SCROLL_AREA1_HEIGHT_PX		( SCROLL_AREA_HEIGHT * 8 )

#define SCROLL_AREA1_MIN_X		SCROLL_AREA1_POS_X
#define SCROLL_AREA1_MAX_X		( SCROLL_AREA1_POS_X + SCROLL_AREA1_WIDTH_PX - 1 )
#define SCROLL_AREA1_MIN_Y		SCROLL_AREA1_POS_Y
#define SCROLL_AREA1_MAX_Y		( SCROLL_AREA1_POS_Y + SCROLL_AREA1_HEIGHT_PX - 1 )

// the tiles that are used for drawing the map have this dimensions
// there is a row of this height at the top of the offscreen to draw the
// new tiles that are coming into view
#define SCROLL_AREA_TOP_TILE_HEIGHT	2
#define SCROLL_AREA_TOP_TILE_WIDTH	2
#define SCROLL_AREA_REAL_HEIGHT		( SCROLL_AREA_HEIGHT + SCROLL_AREA_TOP_TILE_HEIGHT )

// the scroll speed, i.e. the number of pixels scrolled down on each round
#define SCROLL_PIXELS_AREA_1		1
#define SCROLL_PIXELS_AREA_2		2
#define SCROLL_PIXELS_AREA_3		4
#define SCROLL_PIXELS_MAX		4

