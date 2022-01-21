#include "HeadwayTracker.hpp"
#include <pigpio.h>

HeadwayTracker::HeadwayTracker(){
	gpioSetMode(TRIGGER, PI_OUTPUT);
	gpioWrite(TRIGGER, 1);
	gpioSetMode(MONITOR, PI_INPUT);
	measurement = MIN_DIST + (MIN_DIST/2.0);
	pulseWidth = 0;
	measuring = false;
	gpioSetAlertFuncEx(MONITOR, _callbackExt, (void*)this);
	
}

bool HeadwayTracker::safeDistance(){
	return (readDistance() > MIN_DIST);
	
}

double HeadwayTracker::readDistance(){
			
	gpioTrigger(TRIGGER, 10, 0); // inputs: pin Number, number of microseconds, high or low
	
	while (measuring){
		gpioDelay(10);
	}
	
	return measurement;
}

void HeadwayTracker::_callbackExt(int gpio, int level, uint32_t tick, void *user){
	HeadwayTracker* self = (HeadwayTracker *) user;
	
	self->_measure(gpio, level, tick);
	
}

void HeadwayTracker::_measure(int gpio, int level, uint32_t tick){
	
	if (level == 1){ // if it starts measuring distance
			pulseWidth = tick;
			gpioSetWatchdog(gpio, TIMEOUT);
			measuring = true;
		
		}
		
		else if (level == 0){ // if it finishes measuring distance
			pulseWidth = tick - pulseWidth;
			if (pulseWidth < 0)
				measurement = pulseWidth / 1000.0; // meters
			measuring = false;
		}
		
		else{ // if distance is too far for LiDAR; watchdog timed out
			measurement = MIN_DIST + (MIN_DIST/2.0);
			measuring = false;
			
		}
	
	
}


