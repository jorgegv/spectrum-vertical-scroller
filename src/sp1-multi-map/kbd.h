#ifndef _KBD_H
#define _KBD_H

#include <stdint.h>
#include <input.h>

extern struct in_UDK kbd;

#define KBD_IN_DEBUG	0x80
#define KBD_IN_SPEED	0x40

void init_kbd( void );
uint8_t kbd_read( void );

#endif // _KBD_H
