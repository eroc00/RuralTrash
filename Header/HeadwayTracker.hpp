#pragma once


/*
	The HeadwayTracker class abstracts the functionality of the GArmin v3 LiDAR
	for software implementation. This class uses PWM to interface with the sensor.

	LiDAR Module: https://www.garmin.com/en-US/p/557294/pn/010-01722-00#specs
	Datasheet: https://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf
	12C Implementation: https://github.com/garmin/LIDARLite_Arduino_Library/tree/master/src



*/

#include <stdint.h>
#define TRIGGER	23	// output
#define MONITOR	24	// input
#define MIN_DIST	50.0// meters
#define TIMEOUT 60 //milliseconds


class HeadwayTracker {
public:

	HeadwayTracker();

	// Returns true if it measures a value greater than the safe distance
	bool safeDistance();

	// TODO: Send LOW signal to TRIGGER pin and measure MONITOR pin. Set back to high when done
	double readDistance();
	


private:
	double measurement;
	uint32_t pulseWidth;
	bool measuring;

	void _measure(int gpio, int level, uint32_t tick);
	static void _callbackExt(int gpio, int level, uint32_t tick, void *user);

};
