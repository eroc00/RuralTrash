#pragma once

#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>
#include <raspicam/cvversioning.h>
#include <string>


typedef raspicam::RaspiCam_Cv Camera;
typedef cv::Mat Image;

class ImageProcessor {
public:

	ImageProcessor();
	~ImageProcessor();

	bool open();

/*  Use camera sensor to capture and save a 960x1280 image */ 
	void capture();

/*	Read Image from local files*/
	void readImage(char* imageName);
	void saveImage(std::string imageName);

/*  Capture an image and apply Hough Transform to obtain
	distance from and orientation of the road */
	void getRoadCharacteristics(double& dist, double& angle);

private:
	// TODO: add data structue that is compatible with storing an image
	Image _im;
	Image _mask;
	Image _maskSunny;
	Image _dilate_element;
	Image _erode_element;

	void averageLines(double& dist, double& angle);
	unsigned int getDistance(unsigned int xVal, unsigned int linDist, double theta);

	cv::Scalar nsrlb;
	cv::Scalar nsrhb;
	cv::Scalar srlb;
	cv::Scalar srhb;

	cv::Size blurWindow;
	//unsigned int blurDev;

	Camera b0055;
	std::vector<cv::Vec2f> lines;

	unsigned int lineIncr;

};
