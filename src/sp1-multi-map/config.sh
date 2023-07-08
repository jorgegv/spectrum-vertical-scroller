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

# the file which includes the scroll map - found in dir maps/
# dimensions are automatically calculated from it
SCROLL_MAP_FILE=small-test.map

#############################################
## End of configuration variables
#############################################

