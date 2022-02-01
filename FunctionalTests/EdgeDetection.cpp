#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
//#include <raspicam/raspicam_cv.h>

int main(int argc, char** argv){

	if (argc != 5){
		std::cout << "Invalid number of arguments.\n";
		return -1;
	}

	using namespace cv;
	int blurDeviation = std::__cxx11::stoi(argv[1]);
	int lowThr = std::__cxx11::stoi(argv[2]);
	int max_lowThr = std::__cxx11::stoi(argv[3]);
	//int ratio = 3;
	int kernel_size = std::__cxx11::stoi(argv[4]);

	Mat image, edgeImg;
	image = imread("../TestImages/roadside.jpg");


	if (!image.data){
		std::cout << "No image data \n";
		return -1;
	}

	// Convert to Grayscale and filter using Edge Detection
	cvtColor(image, edgeImg, COLOR_RGB2GRAY);

	blur(edgeImg, edgeImg, Size(blurDeviation, blurDeviation));
	Canny(edgeImg, edgeImg, lowThr, max_lowThr, kernel_size);
	
/*
	Mat dst;
	dst.create(image.size(), image.type());
	dst = Scalar::all(0);
	image.copyTo(dst, edgeImg);
*/
	imwrite("/home/pi/Desktop/Result.jpg", edgeImg);

	std::cout << "Finished Processing image \n";

	return 0;
}
