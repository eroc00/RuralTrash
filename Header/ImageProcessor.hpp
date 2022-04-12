#pragma once

#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>
#include <raspicam/cvversioning.h>
#include <string>


typedef raspicam::RaspiCam_Cv Camera;
typedef cv::Mat Image;
typedef cv::Mat Matrix;

class ImageProcessor {
public:

	ImageProcessor();
	~ImageProcessor();

	bool open();

/*  Use camera sensor to capture and save a 960x1280 image */ 
	void capture();

/*	Read Image from local files*/
	void readImage(std::string imageName);
	void saveImage(std::string imageName);
	
	void undistort();
	
	cv::Mat undistort(cv::Mat& img);

/*  Capture an image and apply Hough Transform to obtain
	distance from and orientation of the road */
	void getRoadCharacteristics(int& dist, double& angle);

private:
	// TODO: add data structue that is compatible with storing an image
	Image _im;
	Image _undistort;
	Image _distort;
	Image _mask;
	Image _maskSunny;
	Image _dilate_element;
	Image _erode_element;
	
	Image img[3];

	cv::Rect subselection;

	Matrix camMtx;
	Matrix distCoeff;

	void averageLines(int& dist, double& angle);
	int getDistance(unsigned int xVal, int linDist, double theta);



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
