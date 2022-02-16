#include "Header/HeadwayTracker.hpp"
#include "Header/Movement.hpp"
#include "Header/Receiver.hpp"
#include "Header/OpController.hpp"
#include "Header/ImageProcessor.hpp"
#include "Header/LateralController.hpp"


class TrashCollector {

	// Default constructor.
	// Usage: TrashCollector tc;
	// NOTE: CANNOT CALL DEFAULT CONSTRUCTOR
	TrashCollector() = delete;

	// Custom constructor, 1 input.
	// Usage: TrashCollector tc(distance_threshold);
	TrashCollector(double distance_threshold);

	// Destructor
	~TrashCollector();

	// Function to call in main.cpp
	void run();

	void reset();

	void checkState();


private:
	OpController mode;
	Mode nextState;

	HeadwayTracker lidar;
	Movement motors;
	Receiver rc;
	SignalPair signals;
	ImageProcessor camera;
	LateralController pdCon;
	bool running;

	void standByMode();

	void manualMode();

	void testAutoMode();

	void automatedMode();

	// TODO: Create a file object to log messages into.

    // Test Comment
    // Test Comment 2
};
