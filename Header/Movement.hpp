#pragma once
#include "DataStructures.hpp"
#include "PinDefs.hpp"
#include <pigpio.h>

//#define BAUDRATE 19200 // Hz
//#define PWM_RANGE 1023

/* Movement Class must be tested during Functional Testing Phase */

class Movement {
public:
	Movement();

	~Movement();

	void operator()(PWMPair& signal);


private:
	PWMPair prevSignal;
	int handle;
	unsigned int toPulseWidth(unsigned int pwmSig);
	unsigned int sensitiveMode(unsigned int pwmSig);

};
