#pragma once
#include "DataStructures.hpp"
//#include <wiringPi.h>
// Reference: http://wiringpi.com/reference/

#define LEFTMOTOR
#define RIGHTMOTOR

/* Movement Class must be tested during Functional Testing Phase */

class Movement {
public:
	Movement() {
		//pinMode(LEFTMOTOR, PWM_OUTPUT);
		//pinMode(RIGHTMOTOR, PWM_OUTPUT);

	}

	~Movement() = default;

	void operator()(PWMPair& signal) {
		// PWM values range from 0-1024 (2^10)


		//digitalWrite(LEFTMOTOR, signal.left());
		//digitalWrite(RIGHTMOTOR, signal.right());

	}



private:


};