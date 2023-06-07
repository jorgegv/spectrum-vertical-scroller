#include "build.h"

/////////////////////////
//
// HEAP FUNCTIONS
//
/////////////////////////

long heap;

// heap initialization - see SP1 examples
void init_heap( void ) {
    heap = 0L;
    sbrk( ( void * ) 0xC000, 0x1000 );	// 4096 bytes of heap at 0xC000
}

void *u_malloc(uint16_t size) {
    return malloc( size );
}

void u_free(void *addr) {
     free( addr );
}
