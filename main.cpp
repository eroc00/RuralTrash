#include "Header/OpController.hpp"
#include "Header/Receiver.hpp"
#include "Header/Movement.hpp"
#include "Header/LateralController.hpp"
#include "Header/HeadwayTracker.hpp"
#include <cstdlib>


//#include <WiringPi> http://wiringpi.com/reference/ [WIRING PI IS DEPRECATED]
//#include <pigpio.h> https://abyz.me.uk/rpi/pigpio/cif.html

int main() {
	
	if (gpioInitialise() >= 0){ // Attempt to initialize Library	
	
	
		OpController setting;
		Receiver rcSignal; // manual input
		LateralController calculator;
		Movement navSignal; // output to motor

		// Perhaps take picture by using this command as a unix interface
		std::system("");

		// Put switch statement into loop
		switch (setting.currMode()) {
		case off:

<<<<<<< HEAD
			break;
=======
	// Perhaps take picture by using this command as a unix interface
	std::system("");

	// Put switch statement into loop
	switch (setting.currMode()) {
	case standby:

		break;

	case manual:
		// This mode should run much quicker than control system
		navSignal(rcSignal.read());
		break;

	case test_auto:
		// TODO: Receive a signal from RC to activate automation for X number of seconds.
		//			after X number of seconds, switch to "standby mode"
		
		// TODO: Read calculated parameters from edge image
		navSignal(calculator(0, 0));
		break;
>>>>>>> de34dd3c74e8d3bbc2b1ba45c5df7a957c6e4cd2

		case manual:
			// This mode should run much quicker than control system
			navSignal(rcSignal.read());
			break;

		case test_auto:
			// TODO: Receive a signal from RC to activate automation for X number of seconds
			
			// TODO: Read calculated parameters from edge image
			navSignal(calculator(0, 0));
			break;

		case automatic:
			// TODO: Calculate angular correction from edge image
			navSignal(calculator(0, 0));
			break;


		}



		// Terminate library functionalities
		gpioTerminate();
	}

}


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
