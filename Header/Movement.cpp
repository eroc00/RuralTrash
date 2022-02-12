#include <pigpio.h>
#include "PinDefs.hpp"
#include "Movement.hpp"

//#define DEVICE "/dev/tty0" // sudo dmesg | grep tty
#define FREQUENCY 2000

Movement::Movement(){

	gpioSetMode(LEFTMOTOR, PI_OUTPUT);
	gpioSetMode(RIGHTMOTOR, PI_OUTPUT);
	gpioSetPWMfrequency(LEFTMOTOR, FREQUENCY);
	gpioSetPWMfrequency(RIGHTMOTOR, FREQUENCY);
	gpioSetPWMrange(LEFTMOTOR, 1E6/FREQUENCY);
	gpioSetPWMrange(RIGHTMOTOR, 1E6/FREQUENCY);
	//prevSignal(511, 511);
	
}

Movement::~Movement() {
	//serClose(handle);
	

}

void Movement::operator()(SignalPair& signal){


	gpioServo(LEFTMOTOR, toPulseWidth(MAXPWMVAL - signal.left()));
	gpioServo(RIGHTMOTOR, toPulseWidth(MAXPWMVAL - signal.right()));

}


/***********
	Converts a value with a range of 0-1023 to a range 
	of 1000-2000 (for pulsewidth timings in nanoseconds)

	@param pwmSig - The PWM signal [0, 1023] to convert

	@output the desired pulsewidth [1000, 2000] given a value
*/
unsigned int Movement::toPulseWidth(unsigned int pwmSig){

	return (pwmSig*1000/MAXPWMVAL) + 1000;

}



