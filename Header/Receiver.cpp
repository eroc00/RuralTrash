#include <pigpio.h>
#include "Receiver.hpp"
#include "PinDefs.hpp"
#include <iostream>

#define TIMEOUT 5 //milliseconds
#define SWITCH_THRESH 1600

Receiver::Receiver(){
	// Initialize pins
	gpioSetMode(CH1, PI_INPUT);
	gpioSetMode(CH2, PI_INPUT);
	gpioSetMode(CH5, PI_INPUT);
	gpioSetMode(CH6, PI_INPUT);

	// Read pins whenever pins transition from LOW -> HIGH (or vice versa)
	gpioSetAlertFuncEx(CH1, _callbackExt, (void*)this);
	gpioSetAlertFuncEx(CH2, _callbackExt, (void*)this);
	gpioSetAlertFuncEx(CH5, _callbackExt, (void*)this);
	gpioSetAlertFuncEx(CH6, _callbackExt, (void*)this);

}

SignalPair& Receiver::readAnalogStick(){

	// Debugging
	std::cout << "Raw Up/Down signal: " << rcSignals[1].measurement << std::endl;
	std::cout << "Raw Left/Right signal: " << rcSignals[2].measurement << std::endl;

	signal(changeBounds(1000, 2000, 1023, rcSignals[1].measurement), 
		changeBounds(1000, 2000, 1023, rcSignals[2].measurement);

	return signal;

}

bool Receiver::readLeftSwitch(){

	return rcSignals[0].measurement > SWITCH_THRESH;

}

bool Receiver::readRightSwitch(){

	return rcSignals[3].measurement > SWITCH_THRESH;

}


void Receiver::_callbackExt(int gpio, int level, uint32_t tick, void* user){
	Receiver* self = (Receiver*) user;

	self->_measure(gpio, level, tick);

}

void Receiver::_measure(int gpio, int level, uint32_t tick){

	gpio = gpio - 17;
	gpio = gpio > 3 ? 3 : gpio;

	if (level == 1){ // start measuring pulsewidth
			rcSignals[gpio].pulseWidth = tick;
			gpioSetWatchdog(gpio, TIMEOUT);
			//rcSignals[gpio].measuring = true;

	}

	else if (level == 0){ // stop measuring pulsewidth and update measurement
		rcSignals[gpio].pulseWidth = tick - rcSignals[gpio].pulseWidth;
		if (rcSignals[gpio].pulseWidth > 0)
			rcSignals[gpio].measurement = rcSignals[gpio].pulseWidth / 1000.0; // meters
		//rcSignals[gpio].measuring = false;
	}

	else{ // Timed out
		rcSignals[gpio].measurement = 1500;
		//rcSignals[gpio].measuring = false;

	}


}







