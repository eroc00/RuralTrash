#include "HeadwayTracker.hpp"
#include <pigpio.h>
#include <iostream>

#define CUTOFF_FREQ 12
#define UPDATETIME 0.1 //ms

HeadwayTracker::HeadwayTracker(double thresh){
	gpioSetMode(TRIGGER, PI_OUTPUT);
	gpioWrite(TRIGGER, 0);
	gpioSetMode(MONITOR, PI_INPUT);
	_mindist = thresh;
	prevMeas = 1.5 * _mindist;
	pulseWidth = 0;
	measuring = false;
	gpioSetAlertFuncEx(MONITOR, _callbackExt, (void*)this);

	
}

bool HeadwayTracker::safeDistance(){
	return (readDistance() > _mindist);
	
}

void HeadwayTracker::setDistanceThreshold(double thr) { _mindist = thr; }

double HeadwayTracker::readDistance(){

	//gpioTrigger(TRIGGER, 10, 0); // inputs: pin Number, number of microseconds, high or low

	while (measuring){
		gpioDelay(10);
	}

	prevMeas = (CUTOFF_FREQ*UPDATETIME*measurement + prevMeas)/(1.0 + CUTOFF_FREQ*UPDATETIME);

	return prevMeas;
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
			if (pulseWidth > 0)
				measurement = pulseWidth / 1000.0; // meters
			measuring = false;
		}
		
		else{ // if distance is too far for LiDAR; watchdog timed out
			measurement = _mindist + (_mindist/2.0);
			measuring = false;
			
		}
	
	
}


