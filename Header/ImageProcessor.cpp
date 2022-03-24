#include "ImageProcessor.hpp"
#include <iostream>

#define IMG_HEIGHT 960
#define IMG_WIDTH 1280
#define BLUR_SIZE 13
#define DIL_SIZE 3
#define ERO_SIZE 5
#define MAX_CANNY_THR 250

// Image calibration parameters
#define Fx 380
#define Fy 380
#define Cx 620
#define Cy 487

// Hough Transform parameters
#define LINSEP 25
#define ANGLESEP 0.4*(CV_PI/180)
#define VOTE_THR 600

#define COLUMNTOCHECK IMG_WIDTH/2.0

ImageProcessor::ImageProcessor() {

	_im.create(IMG_HEIGHT, IMG_WIDTH, CV_8UC3);
	_undistort.create(IMG_HEIGHT, IMG_WIDTH, CV_8UC3);
	std::cout << "a \n";
	_mask.create(IMG_HEIGHT, IMG_WIDTH, CV_8UC1);
	_maskSunny.create(IMG_HEIGHT, IMG_WIDTH, CV_8UC1);
	camMtx.create(3, 3, CV_8UC1);
	distCoeff.create(1, 4, CV_64FC1);

	_dilate_element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * DIL_SIZE + 1, 2 * DIL_SIZE + 1),
		cv::Point(DIL_SIZE, DIL_SIZE));

	_erode_element = cv::getStructuringElement(cv::MORPH_CROSS,
		cv::Size(2 * ERO_SIZE + 1, 2 * ERO_SIZE + 1),
		cv::Point(ERO_SIZE, ERO_SIZE));

	nsrlb = cv::Scalar(0, 0, 0);
	nsrhb = cv::Scalar(60, 135, 122);
	srlb = cv::Scalar(80, 0, 0);
	srhb = cv::Scalar(255, 41, 155);
	std::cout << "b \n";

	camMtx.at<int>(0, 0) = Fx;
	camMtx.at<int>(0, 2) = Cx;
	camMtx.at<int>(1, 1) = Fy;
	camMtx.at<int>(1, 2) = Cy;
	std::cout << "c \n";

	distCoeff.at<double>(0) = -0.2;
	distCoeff.at<double>(1) = 0.026;
	std::cout << "d \n";
	blurWindow = cv::Size(BLUR_SIZE, BLUR_SIZE);
	lineIncr = 0;

}

ImageProcessor::~ImageProcessor() {
	b0055.release();

}

bool ImageProcessor::open() {
	return b0055.open();
}


/*  Use camera sensor to capture and save a 960x1280 image */ 
void ImageProcessor::capture() {
	std::cout << "start \n";
	b0055.grab();
	std::cout << "Hello";
	b0055.retrieve(_undistort);
	std::cout << "Hi";
	cv::undistort(_undistort, _im, camMtx, distCoeff);
	std::cout << _im.rows << " " << _im.cols;
}

void ImageProcessor::readImage(char* imageName){
	_im = cv::imread(imageName);
	cv::cvtColor(_im, _im, cv::COLOR_RGB2HSV);
}

void ImageProcessor::saveImage(std::string imageName){
	capture();
	imageName = "/home/pi/RuralTrash/TestImages/" + imageName;
	cv::imwrite(imageName, _im);
}

/*  Capture an image and apply Hough Transform to obtain
	distance from and orientation of the road */
void ImageProcessor::getRoadCharacteristics(unsigned int& dist, double& angle) {

	// Capture Image
	capture();

	// Convert to HSV
	
	cv::cvtColor(_im, _im, cv::COLOR_BGR2HSV);

	/* Separate road from terrain */
	cv::inRange(_im, nsrlb, nsrhb, _mask);
	cv::inRange(_im, srlb, srhb, _maskSunny);

	_mask |= _maskSunny;

	cv::dilate(_mask, _mask, _dilate_element);
	cv::erode(_mask, _mask, _erode_element);
	cv::blur(_mask, _mask, blurWindow);
	cv::Canny(_mask, _mask, 0, MAX_CANNY_THR, 3);

	cv::HoughLines(_mask, lines, LINSEP, ANGLESEP, VOTE_THR);

	averageLines(dist, angle);

}

void ImageProcessor::averageLines(unsigned int& dist, double& angle) {

	for (lineIncr = 0, dist = 0, angle = 0; 
		lineIncr < lines.size() && lineIncr < 10; 
		lineIncr++) {

		dist += lines[lineIncr][0];
		angle += lines[lineIncr][1];

	}

	if (lineIncr | 0) lineIncr = 1;
	dist /= lineIncr;
	angle /= lineIncr;

	dist = getDistance(COLUMNTOCHECK, dist, angle);

}

unsigned int ImageProcessor::getDistance(unsigned int xVal, unsigned int linDist, double theta) {
	//return -(sin(theta) / cos(theta))*xVal + (linDist / cos(theta)); // line spans from top to botton
	return -(cos(theta) / sin(theta))*xVal + (linDist / sin(theta)); // line spans from left to right
}

