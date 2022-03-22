#include "TrashCollector.hpp"
#include <iostream>

#define UPDATETIME 300

TrashCollector::TrashCollector(double distance_threshold) {
	reset();
	lidar.setDistanceThreshold(distance_threshold);
	running = true;
	gpioSetMode(LIGHTPIN, PI_OUTPUT);
	rc.getSwitchState();
	mode.disable();

	gpioWrite(LIGHTPIN, 0);
	gpioDelay(1000000);
	gpioWrite(LIGHTPIN, 1);

}

TrashCollector::~TrashCollector() {
	mode.disable();
	reset();
	//gpioTerminate();
}


void TrashCollector::reset() {
	//mode.disable();
	motors.stop();
	pdCon.reset();

}

void TrashCollector::run() {

	// Setup
	while (!rc.getSwitchState()) { 
		// Stall for half a second until the operator is ready to move robot
		gpioDelay(500000);
	}


	while (running) {
		switch (mode.currMode()) {
		case standby:
			standByMode();
			break;

		case manual:
			manualMode();
			break;

		case test_auto:
			testAutoMode();
			break;

		case automatic:
			automatedMode();
			break;

		case transition_state:
			reset();
			mode.setMode(nextState);

			if (nextState == 3)
				gpioSetTimerFuncEx(0, UPDATETIME, _callbackExt, (void*)this);
			else
				gpioSetTimerFuncEx(0, UPDATETIME, NULL, (void*)this);

			// Flashing Light
			if (mode.currMode() == 1)
				gpioWrite(LIGHTPIN, 0);
			else
				gpioWrite(LIGHTPIN, 1);

			std::cout << "Now in mode "<< (int)nextState << std::endl;
			break;

		default:
			reset();
			running = false;

		}

		// Checks if the mode is being switched
		checkState();

	}

}

void TrashCollector::checkState() {

	if (rc.changedSwitches()) { // Reads RC's button states
		reset();
		nextState = (Mode)rc.getSwitchState();
		mode.setMode(transition_state);
	}
	else if ((mode.currMode() == test_auto || mode.currMode() == automatic)
		&& !lidar.safeDistance()) {
		// Forcefully transition to standby state if an obstacle
		// is in front of the Trash Collector

		mode.setMode(transition_state);
		nextState = (Mode)1;
		reset();
	}

}

// Do nothing
void TrashCollector::standByMode() { }


/***********
	Move motors based on analog stick readings from RC and wait 100ms

*/
void TrashCollector::manualMode() {

	motors(rc.readAnalogStick());
	gpioDelay(100000);

}

/***********

	Goal: Move Trash Collector forward indefinitely until it senses an
			obstacle in front of it.

	SAFETY IMPROVEMENT: Move Trash Collector forward for some time (10 seconds) until
					it senses an obstacle in front of it, or until time runs out

*/
void TrashCollector::testAutoMode() {

    motors(signals(767, 512));
    gpioDelay(100000);


}

/***********
	TODO: Implement roadside line following autonomy

	Goal: Move Trash Collector forward, changing its steering angle based on
			the line it detects between the ashpalt and terrain. Stop if it detects
			an obstable in front of it.

*/
void TrashCollector::automatedMode() {
	// Could use this function to log control system's outputs



}

void TrashCollector::_callbackExt(void* user) {
	TrashCollector* self = (TrashCollector*)user;

	self->_followLine();

}

void TrashCollector::_followLine() {

	// Get inputs to control system
	camera.getRoadCharacteristics(distance, angle);

	// Run control system and output value to motor
	motors(signals(600, (512*2/CV_PI)*angle));
		

}














