#include <stdint.h>
#include <input.h>

#include "build.h"

struct in_UDK kbd;

void init_kbd( void ) {
    kbd.up	= in_LookupKey('q');
    kbd.down	= in_LookupKey('a');
    kbd.left	= in_LookupKey('o');
    kbd.right	= in_LookupKey('p');
    kbd.fire	= in_LookupKey(' ');
}

uint8_t kbd_read( void ) {
    return in_JoyKeyboard( &kbd );
}
