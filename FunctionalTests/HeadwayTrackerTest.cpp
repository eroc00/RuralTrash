
#include "../Header/HeadwayTracker.hpp"
#include <iostream>
#include <pigpio.h>
#include <stdint.h>

int main(){
	if (gpioInitialise() >= 0){
		std::cout << "PIGPIO INITIALIZED \n";
		HeadwayTracker lidar;
		gpioSetMode(14, PI_OUTPUT);
		
		while (true){
			std::cout << "Distance = " << lidar.readDistance() << std::endl << std::endl;
			gpioWrite(14, (unsigned)lidar.safeDistance());
			gpioDelay(1000);
			

		}
		
		gpioTerminate();
	}
	else{
		std::cout << "COULD NOT INITIALIZE PIGPIO LIBRARY \n";
	}
	
	
	
	
	
	
}
