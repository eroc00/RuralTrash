#include "../Header/ImageProcessor.hpp"
#include <iostream>
#include <chrono>

using namespace std::chrono;

int main(){

	ImageProcessor camera;

	if (!camera.open()) { 
		std::cout << "Could not open camera\n";
		return -1;
	}

	bool done = false;
	std::string line;
	auto t1 = system_clock::now(), t2 = system_clock::now();
	
	unsigned int dist;
	double angle;

	while (!done){
		std::cout << "Save next image to which file: ";
		getline(std::cin, line);

		if (line == "exit" || line == "quit"){
			done = false;
			continue;
		}
		
		camera.saveImage(line);

		// Timing camera functionality
		t1 = system_clock::now();
		camera.getRoadCharacteristics(dist, angle);
		t2 = system_clock::now();

		std::cout << "Saved Image. \n\n";
		std::cout << "Time it took to process image in milliseconds: " 
				<< duration_cast<milliseconds>(t2-t1).count() << "\n\n";

	}


	return 0;
}
