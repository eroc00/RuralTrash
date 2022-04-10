#include "ImageProcessor.hpp"

#define IMG_HEIGHT 960
#define IMG_WIDTH 1280
#define BLUR_SIZE 13
#define DIL_SIZE 3
#define ERO_SIZE 5
#define MAX_CANNY_THR 250
#define SUNNY_LOWERBOUND cv::Scalar(0, 0, 0)
#define SUNNY_HIGHERBOUND cv::Scalar(60, 135, 122)
#define SHADOW_LOWERBOUND cv::Scalar(80, 0, 0)
#define SHADOW_HIGHERBOUND cv::Scalar(179, 41, 155)


// Image calibration parameters
#define Fx 380.0
#define Fy 380.0
#define Cx 620.0
#define Cy 487.0

// Hough Transform parameters
#define LINSEP 25
#define ANGLESEP 0.4*(CV_PI/180)
#define VOTE_THR 525

#define CROPX IMG_WIDTH/2

#define COLUMNTOCHECK CROPX/2
#define IDEALDIST 250


ImageProcessor::ImageProcessor() {

	//_im.create(IMG_HEIGHT, IMG_WIDTH, CV_64FC1);
	_undistort.create(IMG_HEIGHT, IMG_WIDTH, CV_8UC3);
	_distort.create(IMG_HEIGHT, IMG_WIDTH, CV_8UC3);
	_mask.create(IMG_HEIGHT, IMG_WIDTH, CV_8UC1);
	_maskSunny.create(IMG_HEIGHT, IMG_WIDTH, CV_8UC1);

	_dilate_element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * DIL_SIZE + 1, 2 * DIL_SIZE + 1),
		cv::Point(DIL_SIZE, DIL_SIZE));

	_erode_element = cv::getStructuringElement(cv::MORPH_CROSS,
		cv::Size(2 * ERO_SIZE + 1, 2 * ERO_SIZE + 1),
		cv::Point(ERO_SIZE, ERO_SIZE));

	nsrlb = SHADOW_LOWERBOUND;
	nsrhb = SHADOW_HIGHERBOUND;
	srlb = SUNNY_LOWERBOUND;
	srhb = SUNNY_HIGHERBOUND;

	camMtx = cv::Mat::zeros(3, 3, CV_64FC1);
	camMtx.at<double>(0, 0) = Fx;
	camMtx.at<double>(0, 2) = Cx;
	camMtx.at<double>(1, 1) = Fy;
	camMtx.at<double>(1, 2) = Cy;
	camMtx.at<double>(2, 2) = 1.0;


	distCoeff = cv::Mat::zeros(4, 1, CV_64FC1);
	distCoeff.at<double>(0) = -0.2;
	distCoeff.at<double>(1) = 0.026;

	blurWindow = cv::Size(BLUR_SIZE, BLUR_SIZE);
	lineIncr = 0;
	
	subselection = cv::Rect(CROPX, 0, IMG_WIDTH-CROPX, 700);

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
	b0055.retrieve(_distort);

}

void ImageProcessor::undistort(){
	cv::undistort(_distort, _undistort, camMtx, distCoeff);

}

cv::Mat ImageProcessor::undistort(cv::Mat& img){
	cv::undistort(img, _undistort, camMtx, distCoeff);
	return _undistort;
}

void ImageProcessor::readImage(std::string imageName){
	_undistort = cv::imread(imageName);

	//cv::cvtColor(_im, _im, cv::COLOR_RGB2HSV);
}

void ImageProcessor::saveImage(std::string imageName){
	capture();
	imageName = "/home/pi/RuralTrash/TestImages/" + imageName;
	cv::imwrite(imageName, _undistort);
}

/*  Capture an image and apply Hough Transform to obtain
	distance from and orientation of the road */
void ImageProcessor::getRoadCharacteristics(int& dist, double& angle) {

	// Capture Image
	capture();
	undistort();

	// Convert to HSV
	_im = _undistort(subselection);
	cv::cvtColor(_im, _im, cv::COLOR_BGR2HSV);

	/* Separate road from terrain */
	cv::inRange(_im, nsrlb, nsrhb, _mask);
	cv::inRange(_im, srlb, srhb, _maskSunny);

	_mask |= _maskSunny;

	cv::dilate(_mask, _mask, _dilate_element);
	cv::erode(_mask, _mask, _erode_element);
	cv::blur(_mask, _mask, blurWindow);
	cv::Canny(_mask, _mask, 0, MAX_CANNY_THR, 3);

	cv::HoughLines(_mask, lines, LINSEP, ANGLESEP, VOTE_THR, 0, 0, 0.0175, CV_PI-0.0175);

	averageLines(dist, angle);

}

void ImageProcessor::averageLines(int& dist, double& angle) {

	for (lineIncr = 0, dist = 0, angle = 0; 
		lineIncr < lines.size() && lineIncr < 10; 
		lineIncr++) {

		dist += lines[lineIncr][0];
		angle += lines[lineIncr][1];

	}

	if (lineIncr == 0){
		lineIncr = IDEALDIST + 1;
		dist = 0;
		angle = CV_PI/2;
	}
	dist /= lineIncr;

	dist = getDistance(COLUMNTOCHECK, dist, angle) - IDEALDIST;

}

int ImageProcessor::getDistance(unsigned int xVal, int linDist, double theta) {
	//return -(sin(theta) / cos(theta))*xVal + (linDist / cos(theta)); // line spans from top to botton
	return -(cos(theta) / sin(theta))*xVal + (linDist / sin(theta)); // line spans from left to right
}

