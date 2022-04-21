#include "TrashCollector.hpp"
#include <iostream>

#define UPDATETIME 300

#define FREQ 1

TrashCollector::TrashCollector(double distance_threshold) {
	reset();
	lidar.setDistanceThreshold(distance_threshold);
	running = true;
	autonomyActive = false;
	gpioSetTimerFuncEx(0, UPDATETIME, _callbackExt, (void*)this);
	gpioSetMode(LIGHTPIN, PI_OUTPUT);
	rc.getSwitchState();
	mode.disable();
	camera.open();
	camera.getRoadCharacteristics(distance, angle);

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
	prevAngle = CV_PI / 2;
	newAngle = 0;

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

			mode.setMode(nextState);
			reset();

			if (nextState == 3)
				autonomyActive = true;
			else
				autonomyActive = false;

			// Flashing Light
			if (((int)mode.currMode() & 2) == 2)
				gpioWrite(LIGHTPIN, 0);
			else
				gpioWrite(LIGHTPIN, 1);

			std::cout << "Now in mode "<< (int)mode.currMode() << std::endl;
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

    motors(signals(750, 512));
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
	gpioDelay(100000);


}

void TrashCollector::_callbackExt(void* user) {
	
		TrashCollector* self = (TrashCollector*)user;

		self->_followLine();

	
}

void TrashCollector::_followLine() {
	if (autonomyActive){

	// Get inputs to control system
	camera.getRoadCharacteristics(distance, angle);

	// Filter turn angle
	newAngle = (FREQ*UPDATETIME*0.001*stanleyLC(distance, angle) + prevAngle)/(1.0 + FREQ*UPDATETIME*0.001);
	prevAngle = newAngle;

	// Run control system and output value to motor
		motors(signals(695, (512*2/CV_PI)*newAngle ));
		std::cout << "Distance: " << distance << " | Angle: " << angle << std::endl;
	}

}

double TrashCollector::stanleyLC(const int& distanceErr, const double& headwayTilt) {
	return (headwayTilt + atan(distanceErr*0.0023) + CV_PI)/2;

}












