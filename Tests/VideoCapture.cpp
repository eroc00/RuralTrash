#include <raspicam/raspicam_cv.h>
#include <raspicam/cvversioning.h>
//#include <opencv2/imgcodecs.hpp>
#include <iostream> // Might replace with fstream during integration testing
#include <string>

typedef raspicam::RaspiCam_Cv Cam;

int main(int argc, char** argv){

	if (argc != 3){
		std::cerr << "Invalid number of arguments. \n";
		return -3;

	}

	// Command line parsing
	unsigned int numberOfFrames = std::__cxx11::stoi(argv[1]);

	if (numberOfFrames == 0 || numberOfFrames > 30){
		std::cerr << "Invalid number of frames: Must be greater than 0 and less than 31 \n";
		return -2;

	}


	std::string filename(argv[2]);
	filename = filename.substr(filename.size() - 4, 4);

	if (filename != ".jpg"){
		std::cerr << "Directory does not exist";
		return -2;

	}

	Cam Camera;
	//Camera.set(CV_CAP_PROP_FORMAT, CV_8UC1); // sets the video to grayscale


	std::cout << "Opening Camera... \n";
	if (!Camera.open()) {std::cerr << "Error opening the camera \n"; return -1;}

	std::cout << "Initialized Camera Module \n";
	cv::Mat image(Camera.get(CV_CAP_PROP_FRAME_HEIGHT), Camera.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC1);

	std::cout << "\n Image Height: " << Camera.get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl;
	std::cout << "Image Width: " << Camera.get(CV_CAP_PROP_FRAME_WIDTH) << std::endl;
	std::cout << "Frame Rate: " << Camera.getFrameRate() << std::endl << std::endl;

	// Capture Frame
	Camera.grab();
	Camera.retrieve(image);
	std::cout << "Output Height: " << image.rows << std::endl;
	std::cout << "Output Width: " << image.cols << std::endl;


	std::cout << "Obtained frame \n";

	// Free memory used by camera object
	Camera.release();

	std::cout << "Released Camera Memory \n";

	cv::imwrite(argv[2], image);
	std::cout << "Image saved to: " << argv[2] << std::endl;



	return 0;
}


