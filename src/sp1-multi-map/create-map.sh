#!/bin/bash

source "$( dirname "$0" )/config.sh"

INPUT="$( dirname "$0" )/maps/$SCROLL_MAP_FILE"

( cat <<EOF1
// Scroll Map data
// Automatically generated from file $INPUT

#include <stdint.h>

#include "build.h"

uint8_t scroll_map_data[] =
EOF1

WIDTH=0
LINE_NUMBER=0
while read -r line; do
	LINE_NUMBER=$(( LINE_NUMBER + 1 ))

	# we set the width to that of the first line
	if [ "$WIDTH" == 0 ]; then
		WIDTH=$( echo -n "$line" | wc -c )
	else
	# else check if it is the same as before
		NEW_WIDTH="$( echo -n "$line" | wc -c )"
		if [ "$NEW_WIDTH" != "$WIDTH" ]; then
			echo "Line $LINE_NUMBER has $NEW_WIDTH characters, but should have $WIDTH"
			exit 1
		fi
	fi

	printf '    "%s"\n' "$line"
done < "$INPUT"

cat <<EOF2
; // end of Scroll Map data

// Scroll Map definition
struct scroll_map_s scroll_map = {
    SCROLL_MAP_WIDTH,
    SCROLL_MAP_HEIGHT,
    &scroll_map_data[0],
    { 0, 0 },           // initial viewport position
};

EOF2
) > scroll_map_data.c

( cat <<EOF3
# Scroll Map data
# Automatically generated from file $INPUT

SCROLL_MAP_WIDTH=$(( $( head -1 "$INPUT" | wc -c ) - 1 ))
SCROLL_MAP_HEIGHT=$( grep -Evc '^$' "$INPUT" )

EOF3
) > scroll_map_data.sh
