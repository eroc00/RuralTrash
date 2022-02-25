#pragma once
#include "ImageProcessor.hpp"

#define IMG_HEIGHT 960
#define IMG_WIDTH 1280
#define BLUR_SIZE 13
#define DIL_SIZE 3
#define ERO_SIZE 5
#define MAX_CANNY_THR 250

// Hough Transform params
#define LINSEP 25
#define ANGLESEP
#define VOTE_THR 300


ImageProcessor::ImageProcessor() {
	
	_im.create(IMG_HEIGHT, IMG_WIDTH, cv::CV_8UC3);
	_mask.create(IMG_HEIGHT, IMG_WIDTH, cv::CV_8UC1);
	_maskSunny.create(IMG_HEIGHT, IMG_WIDTH, cv::CV_8UC1);


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
	b0055.grab();
	b0055.retrieve(_im);
	
}

/*  Capture an image and apply Hough Transform to obtain
	distance from and orientation of the road */
void ImageProcessor::getRoadCharacteristics(double& dist, double& angle) {

	// Capture Image
	takePicture();

	// Convert to HSV
	cv::cvtColor(_im, _im, cv::COLOR_RGB2HSV);

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

void ImageProcessor::averageLines(double& dist, double& angle) {
	
	for (lineIncr = 0; lineIncr < lines.size() && lineIncr < 10; i++) {



	}


}

void ImageProcessor::getDistance(unsigned int xVal, unsigned int linDist, double theta) {
	return -(sin(theta) / cos(theta))*xVal + (linDist / cos(theta)); // line spans from top to botton
	//return -(cos(theta) / sin(theta))*xVal + (linDist / sin(theta)); // line spans from left to right
}
