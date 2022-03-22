#include "TrashCollector.hpp"
#include <cstdlib>
#include <iostream>

#define DIST_THR 2.1


int main() {
	
	if (gpioInitialise() < 0){ // Attempt to initialize Library	
		std::cout << "Failed to initialize pigpio";
		return -1;
				
	}

	TrashCollector tc(DIST_THR);
	tc.run();

	gpioTerminate();
	return 0;
}
