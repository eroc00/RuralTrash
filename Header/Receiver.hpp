#pragma once
#include "DataStructures.hpp"

// Analog Read: http://wiringpi.com/reference/core-functions/

#define SIGNAL1
#define SIGNAL2

// More testing to be done during Functional Tests

class Receiver {
public:

	Receiver() {
		//pinMode(SIGNAL1, INPUT);
		//pinMode(SIGNAL2, INPUT);

	}

	~Receiver() = default;

	PWMPair& read() {
#ifdef UNIT_TESTING
		return signal(512, 512);
#else
		//return signal(analogRead(SIGNAL1), analogRead(SIGNAL2));
#endif
	}


private:
	PWMPair signal;

};