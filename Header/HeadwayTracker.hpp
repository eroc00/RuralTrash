#pragma once

/*
	The HeadwayTracker class abstracts the functionality of the GArmin v3 LiDAR
	for software implementation. This class uses PWM to interface with the sensor.

	LiDAR Module: https://www.garmin.com/en-US/p/557294/pn/010-01722-00#specs
	Datasheet: https://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf
	12C Implementation: https://github.com/garmin/LIDARLite_Arduino_Library/tree/master/src



*/


#define TRIGGER		// input
#define MONITOR		// output
#define MINDIST	50// meters


class HeadwayTracker {
public:

	// Returns true if it measures a value greater than the safe distance
	bool safeDistance() { return (readDistance() > MINDIST); }

	// TODO: Send LOW signal to TRIGGER pin and measure MONITOR pin. Set back to high when done
	unsigned int readDistance();

private:
	// LIDARLite

	






};