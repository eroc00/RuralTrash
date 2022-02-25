#include "TrashCollector.hpp"
#include <cstdlib>
#include <iostream>

#define DIST_THR 1


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


/***************
	Trying out multi-threaded implementation.IGNORE FOR NOW
*/
void headwayMonitor(OpController* setting) {
	HeadwayTracker lidar;
	while (true) {
		switch (setting->currMode()) {
		case test_auto:
		case automatic:
			if (!lidar.safeDistance())
				setting->disable();
			break;

		default:
			// Do nothing
			break;

		}

		// Wait for half the time it takes between control system sample time


	}


}


void failSafeControlSignals(OpController* setting, Receiver* rcSignal) {







}
