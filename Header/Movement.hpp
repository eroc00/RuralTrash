#pragma once
#include "DataStructures.hpp"
#include "PinDefs.hpp"
#include <pigpio.h>

#define FREQUENCY 2000 // Hz
#define PWM_RANGE 1023

/* Movement Class must be tested during Functional Testing Phase */

class Movement {
public:
	Movement();

	~Movement() = default;

	void operator()(PWMPair& signal);


private:


};
