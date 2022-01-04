#pragma once
#include "DataStructures.hpp"

#define SETDISTANCE 700 // pixels
#define ANGLETOLERANCE 3 // degrees
#define MAXSPEED 5.0 // meters per second
#define SAMPLETIME 0.1 // seconds
#define PI 3.14159265358979323846 // pi

class LateralController {
public: 
	LateralController() { reset(); }

	~LateralController() = default;


	// outputs PWM signal to turn signal a certain number of degrees
	PWMPair& operator()(unsigned int distance, int angle) {

		// calculate angle required to turn

		// convert angle to wheel speeds

		// convert wheel speeds to required PWM signals

		return out;
	}

	void reset() {
		// TODO: set all private data to their initial values
		
		previousOutput = 0;
		
	}

private:
	//double currAngle; // should just be 0
	//unsigned int setDistance; // distance from road
	long previousOutput; // previous angle for feedback loop

	PWMPair out;


};

void angleToMotorSignal(int& angle, PWMPair& signals) {

	if (angle > ANGLETOLERANCE) { // if positive

	}
	else if (angle < -ANGLETOLERANCE) { // if negative

	}
	else {

	}




}

unsigned int speedToPWMSignal(double speed) {

	// Band Pass filter to read speeds in the range 0 <= speed <= MAXSPEED
	if (speed > MAXSPEED)
		speed = MAXSPEED;
	else if (speed < 0)
		speed = 0;

	return (speed / MAXSPEED) * 1024;
}
