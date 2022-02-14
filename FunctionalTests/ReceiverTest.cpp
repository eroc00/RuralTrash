#include "../Header/Movement.hpp"
#include <iostream>
#include <string>
#include "../Header/Receiver.hpp"
#include "../Header/OpController.hpp"

int main(int argc, char** argv){

	
	if(gpioInitialise() < 0){
		std::cerr << "Initialization failed \n";
		return -1;
	}

	// Code goes here
	Receiver rc;
	Movement motors;
	SignalPair* signals;
	OpController operationalState;
	bool lbStatus;
	bool rbStatus;
	std::cout << "Press the enter key to read values "
			  << "(input 'exit' to close application) \n";
	
	// Read Channels 5 and 6
	bool done = false;
	std::string line;
	while (!done){
		/*
		getline(std::cin, line);
		if (line == "exit"){
			done == true;
			break;
		}
		*/

		//else{
			signals = &(rc.readAnalogStick());
			std::cout << "Analog longitudinal value: " << signals->left() << std::endl;
			std::cout << "Analog lateral value: " << signals->right() << std::endl;

			std::cout << "Left Switch on?: " << rc.readLeftSwitch() << std::endl;
			std::cout << "Right Switch on?: " << rc.readRightSwitch() << std::endl;

			std::cout << "Op Mode state: " 
				<< operationalState.setMode((lbStatus << 1) | (rbStatus)) << std::endl;

			motors(*signals);
			
			gpioDelay(100000);
			//std::cout << "\nPress enter again to read values\n\n";
		//}

	}

	gpioTerminate();
	return 0;
}
