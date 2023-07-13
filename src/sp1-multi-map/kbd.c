#include <stdint.h>
#include <input.h>

#include "build.h"

struct in_UDK kbd_config;
uint16_t scan_debug;
uint16_t scan_speed;

void init_kbd( void ) {
    kbd_config.up	= in_LookupKey('q');
    kbd_config.down	= in_LookupKey('a');
    kbd_config.left	= in_LookupKey('o');
    kbd_config.right	= in_LookupKey('p');
    kbd_config.fire	= in_LookupKey(' ');
    scan_debug	= in_LookupKey('d');
    scan_speed	= in_LookupKey('s');
}

uint8_t kbd_read( void ) {
    uint8_t ret;

    // read keyboard movements and fire (bits 0-4)
    ret = in_JoyKeyboard( &kbd_config );

    // set bit 7 if 'debug' toggle was pressed
    ret |= ( in_KeyPressed( scan_debug ) ? KBD_IN_DEBUG : 0 );

    // set bit 6 if 'speed' toggle was pressed
    ret |= ( in_KeyPressed( scan_speed ) ? KBD_IN_SPEED : 0 );

    return ret;
}
