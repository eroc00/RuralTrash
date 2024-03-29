#pragma once 
#include <pigpio.h>
#include <stdint.h>
#include "DataStructures.hpp"
#include "PinDefs.hpp"


/** Class to read signals from the RC Receiver **/
class Receiver {
public:

	Receiver();

	~Receiver() = default;

	SignalPair& readAnalogStick();

	int getSwitchState();

	bool changedSwitches();

	bool readLeftSwitch();

	bool readRightSwitch();

private:

	struct PinData {
		unsigned int measurement = 0;
		unsigned int pulseWidth = 0;
		//bool measuring = false;
	};

	//** Signal indexes **//
	// index = gpioPin - 17;	[0, 3]
	// 0: Left Switch
	// 1: Joystick Up/Down
	// 2: Joystick Left/Right
	// 3: Right Switch
	PinData rcSignals[4];

	// Analog Signals received from remote
	SignalPair signal;

	// Button states
	bool lbSwitch;
	bool rbSwitch;

	// Private Functions

	// EVERYTHING ON RECEIVER SENDS DATA IN PULSEWIDTHS
	static void _callbackExt(int gpio, int level, uint32_t tick, void* user);

	void _measure(int gpio, int level, uint32_t tick);


};

