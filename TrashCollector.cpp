#include "TrashCollector.hpp"


TrashCollector::TrashCollector(double distance_threshold) {
	reset();
	lidar.setDistanceThreshold(distance_threshold);
	running = true;
	
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
			break;

		case default:
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
		nextState = rc.getSwitchState();
		mode.setMode(transition_state);
	}
	else if ((mode.currMode() == test_auto || mode.currMode() == automatic)
		&& !lidar.safeDistance()) {
		// Forcefully transition to standby state if an obstacle
		// is in front of the Trash Collector
		mode.disable();
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
	delay(100000);

}

/***********
	TODO: Implement straight-line autonomy

	Goal: Move Trash Collector forward indefinitely until it senses an 
			obstacle in front of it.

	SAFETY IMPROVEMENT: Move Trash Collector forward for some time (10 seconds) until 
					it senses an obstacle in front of it, or until time runs out 

*/
void TrashCollector::testAutoMode() {
	





}

/***********
	TODO: Implement roadside line following autonomy

	Goal: Move Trash Collector forward, changing its steering angle based on 
			the line it detects between the ashpalt and terrain. Stop if it detects
			an obstable in front of it.

*/
void TrashCollector::automatedMode() {




}






















