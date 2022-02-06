#include "DataStructures.hpp"
#include <pigpio.h>
#include "PinDefs.hpp"
#include "Movement.hpp"

Movement::Movement(){
	gpioSetMode(LEFTMOTOR, PI_ALT0);
	gpioSetMode(RIGHTMOTOR, PI_ALT0);
	gpioSetPWMfrequency(LEFTMOTOR, FREQUENCY);
	gpioSetPWMfrequency(RIGHTMOTOR, PI_ALT0);
	gpioSetPWMrange(LEFTMOTOR, PWM_RANGE);
	gpioSetPWMrange(RIGHTMOTOR, PWM_RANGE);

}

void Movement::operator()(PWMPair& signal){

	gpioPWM(LEFTMOTOR, signal.left());
	gpioPWM(RIGHTMOTOR, signal.right());

}
