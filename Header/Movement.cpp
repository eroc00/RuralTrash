#include <pigpio.h>
#include "PinDefs.hpp"
#include "Movement.hpp"

//#define DEVICE "/dev/tty0" // sudo dmesg | grep tty
#define RESIS 10E3
#define CAP 0.1E-6
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

void Movement::operator()(PWMPair& signal){


	gpioServo(LEFTMOTOR, toPulseWidth(signal.left()));
	gpioServo(RIGHTMOTOR, toPulseWidth(signal.right()));

}

unsigned int Movement::toPulseWidth(unsigned int pwmSig){

	return (pwmSig*1000/1023) + 1000;

}

unsigned int Movement::sensitiveMode(unsigned int sig){
	if (sig < 384)
		sig = 384;
	return sig;
}

//void Movement::moveMotor(bool motor, unsigned int signal){
	




