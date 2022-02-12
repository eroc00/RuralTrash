#pragma 
#include <pigpio.h>
#include "DataStructures.hpp"
#include "PinDefs.hpp"

// Analog Read: http://wiringpi.com/reference/core-functions/
// 
// Likely to be PWM signals

// More testing to be done during Functional Tests

class Receiver {
public:

	Receiver() {
		gpioSetMode(CH1, PI_ALT5);
		gpioSetMode(CH2, PI_ALT5);

	}

	~Receiver() = default;

	PWMPair& read() {

	}
	
	// EVERYTHING ON RECEIVER SENDS DATA IN PULSEWIDTHS
	
	void _callbackExt(int gpio, int level, uint32_t tick){
	HeadwayTracker* self = (HeadwayTracker *) user;
	
	self->_measure(gpio, level, tick);
	
}

	void HeadwayTracker::_measure(int gpio, int level, uint32_t tick){
		
		if (level == 1){ // if it starts measuring distance
				pulseWidth = tick;
				gpioSetWatchdog(gpio, TIMEOUT);
				/measuring = true;
			
			}
			
			else if (level == 0){ // if it finishes measuring distance
				pulseWidth = tick - pulseWidth;
				if (pulseWidth > 0)
					measurement = pulseWidth / 1000.0; // meters
				//measuring = false;
			}
			
			else{ // if distance is too far for LiDAR; watchdog timed out
				measurement = MIN_DIST + (MIN_DIST/2.0);
				//measuring = false;
				
			}
		
		
	}


private:
	PWMPair signal;

};
