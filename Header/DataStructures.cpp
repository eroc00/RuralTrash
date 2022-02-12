#include "DataStructures.hpp"

SignalPair::SignalPair() : bits(0) {}

// input
SignalPair& SignalPair::operator()(unsigned int a, unsigned int b) {

	// Keep numbers between 0 and 1023
	bpf(a); bpf(b);

	// right motor PWM bits: 0-9
	bits = (b & 0x3FF); // right motor signal

	// left motor PWM bits: 10-19
	bits = bits | (a & 0x3FF) << 10;

	return *this;
}

// output
unsigned int SignalPair::left() const { return (bits & 0xFFC00) >> 10; }
unsigned int SignalPair::right() const { return (bits & 0x003FF); }

void SignalPair::bpf(unsigned int& num) {
	if (num > MAXPWMVAL)
		num = MAXPWMVAL;
}

unsigned int changeBounds(unsigned int lowerB, unsigned int higherB, unsigned int newHigherBound, unsigned int value) {

	return (pwmSig * (higherB - lowerB) / newHigherBound) + lowerB;

}