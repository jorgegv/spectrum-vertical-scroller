#include "build.h"

void main( void ) {
    init_screen_address_tables();
    init_sp1_tile_map();
    init_perfmeter();

    reset_perfmeter();
    while (1) {
    	do_perf_accounting();
    }
}
