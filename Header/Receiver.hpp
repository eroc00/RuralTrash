#pragma 
#include <pigpio.h>
#include "DataStructures.hpp"

// Analog Read: http://wiringpi.com/reference/core-functions/
// 
// Likely to be PWM signals

#define SIGNAL1 18
#define SIGNAL2 19

// More testing to be done during Functional Tests

class Receiver {
public:

	Receiver() {
		gpioSetMode(SIGNAL1, PI_ALT5);
		gpioSetMode(SIGNAL2, PI_ALT5);

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
