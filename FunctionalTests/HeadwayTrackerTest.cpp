
#include "../Header/HeadwayTracker.hpp"
#include <iostream>
#include <pigpio.h>

int main(){
	if (gpioInitialise() >= 0){
		std::cout << "PIGPIO INITIALIZED \n";
		HeadwayTracker lidar;
		
		
		gpioTerminate();
	}
	else{
		std::cout << "COULD NOT INITIALIZE PIGPIO LIBRARY \n";
	}
	
	
	
	
	
	
}
