#pragma once


/*
	The HeadwayTracker class abstracts the functionality of the GArmin v3 LiDAR
	for software implementation. This class uses Pulse Widths to interface with the sensor.

	LiDAR Module: https://www.garmin.com/en-US/p/557294/pn/010-01722-00#specs
	Datasheet: https://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf

*/

#include <stdint.h>
#include "PinDefs.hpp"

#define MIN_DIST	2// meters
#define TIMEOUT 100 //milliseconds

class HeadwayTracker {
public:

	HeadwayTracker(double thresh = MIN_DIST);

	// Returns true if it measures a value greater than the safe distance
	bool safeDistance();

	// Send LOW signal to TRIGGER pin and measure MONITOR pin
	double readDistance();

	// Changes the threshold value
	void setDistanceThreshold(double thr);
	


private:
	double measurement;
	double _mindist;
	uint32_t pulseWidth;
	bool measuring;

	void _measure(int gpio, int level, uint32_t tick);
	static void _callbackExt(int gpio, int level, uint32_t tick, void *user);

};
