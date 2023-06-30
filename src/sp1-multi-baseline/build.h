#ifndef _BUILD_H
#define _BUILD_H

#pragma output CRT_ORG_CODE           = 0x5f00  // org of compile
#pragma output REGISTER_SP            = 0xd1d1  // just below ISR
#pragma output CRT_STACK_SIZE         = 208     // just enough

#pragma output CRT_ENABLE_CLOSE       = 0       // don't bother closing files on exit
#pragma output CRT_ENABLE_EIDI        = 1       // disable interrupts at start
#pragma output CRT_ENABLE_RESTART     = 1       // do not return to basic

#pragma output CLIB_MALLOC_HEAP_SIZE  = 0       // // heap: initialized manually
#pragma output CLIB_STDIO_HEAP_SIZE   = 0       // no stdio heap (no files)

// Memory map:
// $5F00 - $BFFF: code/data/bss
// $C001 - $CFFF: heap (manualy assigned)
// $D100 - $D1D1: stack
// $D1D2 - $FFFF: SP1 data

// For classic:
#include <spectrum.h>
#include <sprites/sp1.h>

// For Newlib:
//#include <arch/spectrum.h>
//#include <games/sp1.h>

#include <stdio.h>
#include <conio.h>
#include <intrinsic.h>

#include "config.h"
#include "perf.h"
#include "offscreen.h"
#include "screen.h"
#include "tile.h"
#include "graphics.h"

#endif // _BUILD_H
