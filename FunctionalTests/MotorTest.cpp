#include "../Header/Movement.hpp"
#include <iostream>
#include <string>

int main(int argc, char** argv){

	
	if(gpioInitialise() < 0){
		std::cerr << "Initialization failed \n";
		return -1;
	}

	Movement motors;
	SignalPair signal;
	bool done = false;
	std::string line;
	
	// Set Motors to 0 speed
	signal(511, 511);
	motors(signal);

	std::cout << "Enter a pair of numbers (num1 num2) "
		 << "in the range 0-1023 to set motor speeds. Type 'exit' to end program.  \n";

	while (!done){
		getline(std::cin, line);
		if (line == "exit"){
			done == true;
			break;
		}
		std::size_t spaceLoc = line.find(' ');

		if (spaceLoc == std::string::npos){
			std::cout << "Cannot read values \n\n";
			continue;
		}

		int speed1 = std::__cxx11::stoi(line.substr(0, spaceLoc));
		int speed2 = std::__cxx11::stoi(line.substr(spaceLoc, line.size() - spaceLoc + 1));

		signal(speed1, speed2);
		
		// Echo and set motor speeds
		std::cout << "Left Motor at " << signal.left() << std::endl;
		std::cout << "Right Motor at " << signal.right() << std::endl << std::endl;

		motors(signal);

	}





	gpioTerminate();
	return 0;
}
