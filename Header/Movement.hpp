#pragma once
#include "DataStructures.hpp"
#include "PinDefs.hpp"
#include <pigpio.h>


class Movement {
public:
	Movement();

	~Movement();

	void operator()(SignalPair& signal);

	void stop();


private:
	SignalPair prevSignal;
	int handle;
	unsigned int toPulseWidth(unsigned int pwmSig);
	//unsigned int sensitiveMode(unsigned int pwmSig);

};
