#!/bin/bash

# This script generates the config.h file with macros #defined as raw
# numbers (i.e.  not with calculations _and_ maybe parentheses). 
# Parentheses are interpreted by the assembler as "content at address XXX"
# and generate incorrect code when used in inline asm e.g.  for instruction
# "ld de,VARIABLE", if VARIABLE is defined as "(TOP + 23)"

#############################################
## Configuration variables
#############################################

# Scroll area dimensions and position, in char cells
SCROLL_AREA_TOP=4
SCROLL_AREA_LEFT=8
SCROLL_AREA_WIDTH=16
SCROLL_AREA_HEIGHT=16

# The visible scroll area has a surrounding invisible area for drawing
# tiles just before they should come into view.  This area is one tile wide
# in all directions.  The SCROLL_AREA_EXTENDED_...  macros below take this into
# account.

SCROLL_MAP_TILE_WIDTH=2
SCROLL_MAP_TILE_HEIGHT=2

#############################################
## End of configuration variables
#############################################

# precalculate some other variables
SCROLL_AREA_BOTTOM=$(( SCROLL_AREA_TOP + SCROLL_AREA_HEIGHT - 1 ))
SCROLL_AREA_RIGHT=$(( SCROLL_AREA_LEFT + SCROLL_AREA_WIDTH - 1 ))
SCROLL_AREA_HEIGHT_LINES=$(( SCROLL_AREA_HEIGHT * 8 ))
SCROLL_AREA_EXTENDED_WIDTH=$(( SCROLL_AREA_WIDTH + 2 * SCROLL_MAP_TILE_WIDTH ))
SCROLL_AREA_EXTENDED_HEIGHT=$(( SCROLL_AREA_HEIGHT + 2 * SCROLL_MAP_TILE_HEIGHT ))
SCROLL_AREA_EXTENDED_HEIGHT_LINES=$(( SCROLL_AREA_EXTENDED_HEIGHT * 8 ))

# generate config.h

cat <<EOF >config.h
/////////////////////////////////////////////////////////////////////////////
//
// ATTENTION!  This file is autogenerated, DO NOT MODIFY!  Edit config.sh
// script instead and re-run 'make config'
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _CONFIG_H
#define _CONFIG_H

// real scroll area sizes, in char cells
#define SCROLL_AREA_TOP				$SCROLL_AREA_TOP
#define SCROLL_AREA_LEFT			$SCROLL_AREA_LEFT
#define SCROLL_AREA_WIDTH			$SCROLL_AREA_WIDTH
#define SCROLL_AREA_HEIGHT			$SCROLL_AREA_HEIGHT
#define SCROLL_AREA_BOTTOM			$SCROLL_AREA_BOTTOM
#define SCROLL_AREA_RIGHT			$SCROLL_AREA_RIGHT

#define SCROLL_AREA_HEIGHT_LINES		$SCROLL_AREA_HEIGHT_LINES

// additional scroll area definitions

// The visible scroll area has a surrounding invisible area for drawing
// tiles just before they should come into view.  This area is one tile wide
// in all directions.  The SCROLL_AREA_EXTENDED_...  macros below take this into
// account.

#define SCROLL_MAP_TILE_WIDTH			$SCROLL_MAP_TILE_WIDTH
#define SCROLL_MAP_TILE_HEIGHT			$SCROLL_MAP_TILE_HEIGHT

#define SCROLL_AREA_EXTENDED_WIDTH		$SCROLL_AREA_EXTENDED_WIDTH
#define SCROLL_AREA_EXTENDED_HEIGHT		$SCROLL_AREA_EXTENDED_HEIGHT
#define SCROLL_AREA_EXTENDED_HEIGHT_LINES	$SCROLL_AREA_EXTENDED_HEIGHT_LINES

// commodity definitions
// #define OFFSCREEN_TOP_LINE_START_OFFSET	0	// we don't need this :-)
#define OFFSCREEN_TOP_LINE_END_OFFSET		$(( ( SCROLL_AREA_EXTENDED_WIDTH - 1 ) * SCROLL_AREA_EXTENDED_HEIGHT_LINES ))
#define OFFSCREEN_BOTTOM_LINE_START_OFFSET	$(( SCROLL_AREA_EXTENDED_HEIGHT_LINES - 1 ))
#define OFFSCREEN_BOTTOM_LINE_END_OFFSET	$(( SCROLL_AREA_EXTENDED_WIDTH * SCROLL_AREA_EXTENDED_HEIGHT_LINES - 1 ))


#endif // _CONFIG_H
EOF