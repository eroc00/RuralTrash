#pragma once

struct PWMPair {

	PWMPair() : bits(0) {}
	
	// input
	PWMPair& operator()(unsigned int a, unsigned int b) {

		// right motor PWM bits: 0-9
		bits = b & 0x3FF; // right motor signal

		// left motor PWM bits: 10-19
		bits = bits | (a & 0x3FF) << 10; 

		return *this;
	}

	// output
	unsigned int left() const { return (bits & 0xFFC00) >> 10; }
	unsigned int right() const { return (bits & 0x003FF); }

private:
	unsigned int bits;

};