#ifndef _CONFIG_H
#define _CONFIG_H

// real scroll area sizes, in char cells
#define SCROLL_AREA_TOP			4
#define SCROLL_AREA_LEFT		8
#define SCROLL_AREA_WIDTH		16
#define SCROLL_AREA_HEIGHT		16
#define SCROLL_AREA_BOTTOM		( SCROLL_AREA_TOP + SCROLL_AREA_HEIGHT - 1 )
#define SCROLL_AREA_RIGHT		( SCROLL_AREA_LEFT + SCROLL_AREA_WIDTH - 1 )
#define SCROLL_AREA_HEIGHT_LINES	( SCROLL_AREA_HEIGHT * 8 )

// additional scroll area definitions

// The visible scroll area has a surrounding invisible area for drawing
// tiles just before they should come into view.  This area is one tile wide
// in all directions.  The SCROLL_AREA_EXTENDED_...  macros take this into
// account.

#define SCROLL_MAP_TILE_WIDTH		2
#define SCROLL_MAP_TILE_HEIGHT		2

#define SCROLL_AREA_EXTENDED_WIDTH	( SCROLL_AREA_WIDTH + 2 * SCROLL_MAP_TILE_WIDTH )
#define SCROLL_AREA_EXTENDED_HEIGHT	( SCROLL_AREA_HEIGHT + 2 * SCROLL_MAP_TILE_HEIGHT )

#endif // _CONFIG_H
