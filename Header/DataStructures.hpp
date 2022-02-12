#pragma once

#define MAXPWMVAL 1023

struct SignalPair {

	SignalPair();
	
	// input
	SignalPair& operator()(unsigned int a, unsigned int b);

	// output
	unsigned int left() const;
	unsigned int right() const;

private:
	unsigned int bits;

	void bpf(unsigned int& num);

};

unsigned int changeBounds(unsigned int lowerB, unsigned int higherB, unsigned int newHigherBound, unsigned int value);