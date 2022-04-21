#include "ImageProcessor.hpp"

#define IMG_HEIGHT 960
#define IMG_WIDTH 1280
#define BLUR_SIZE 13
#define DIL_SIZE 3
#define ERO_SIZE 5
#define MAX_CANNY_THR 200
//#define SUNNY_LOWERBOUND cv::Scalar(0, 92, 0)
//#define SUNNY_HIGHERBOUND cv::Scalar(56, 255, 255)
#define SHADOW_LOWERBOUND cv::Scalar(0, 125, 0)
#define SHADOW_HIGHERBOUND cv::Scalar(56, 255, 255)

// Image calibration parameters
#define Fx 380.0
#define Fy 380.0
#define Cx 620.0
#define Cy 487.0

// Hough Transform parameters
#define LINSEP 25
#define ANGLESEP 0.4*(CV_PI/180)
#define VOTE_THR 600

#define CROPX IMG_WIDTH/2

#define COLUMNTOCHECK CROPX/2
#define IDEALDIST 250


ImageProcessor::ImageProcessor() {

	//_im.create(IMG_HEIGHT, IMG_WIDTH, CV_64FC1);
	_undistort.create(IMG_HEIGHT, IMG_WIDTH, CV_8UC3);
	_distort.create(IMG_HEIGHT, IMG_WIDTH, CV_8UC3);
	_mask.create(IMG_HEIGHT, IMG_WIDTH, CV_8UC1);
	_maskSunny.create(IMG_HEIGHT, IMG_WIDTH, CV_8UC1);

	_dilate_element = cv::getStructuringElement(cv::MORPH_RECT,
		cv::Size(2 * DIL_SIZE + 1, 2 * DIL_SIZE + 1),
		cv::Point(DIL_SIZE, DIL_SIZE));

	_erode_element = cv::getStructuringElement(cv::MORPH_RECT,
		cv::Size(2 * ERO_SIZE + 1, 2 * ERO_SIZE + 1),
		cv::Point(ERO_SIZE, ERO_SIZE));

	nsrlb = SHADOW_LOWERBOUND;
	nsrhb = SHADOW_HIGHERBOUND;
//	srlb = SUNNY_LOWERBOUND;
//	srhb = SUNNY_HIGHERBOUND;

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

	x0 = 0;
	y0 = 0;
	m = 0;
	a = 0;
	b = 0;
	denom = 0;

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
	imageName = "/home/ruraltrash/RuralTrash/TestImages/" + imageName;
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

	// Histogram Equalization
	split(_im, img);
	equalizeHist(img[1], img[1]);
	merge(img, 3, _im);

	/* Separate road from terrain */
	cv::inRange(_im, nsrlb, nsrhb, _mask);
	//cv::inRange(_im, srlb, srhb, _maskSunny);

	//_mask |= _maskSunny;

	cv::dilate(_mask, _mask, _dilate_element);
	cv::erode(_mask, _mask, _erode_element);
	//cv::imwrite("cannyImg.jpg", _mask);

	cv::blur(_mask, _mask, blurWindow);
	cv::Canny(_mask, _mask, 0, MAX_CANNY_THR, 3);
	
	cv::HoughLines(_mask, lines, LINSEP, ANGLESEP, VOTE_THR);

	averageLines(dist, angle);

}

void ImageProcessor::averageLines(int& dist, double& angle) {

	resetPoints();

	for (lineIncr = 0, dist = 0, angle = 0; 
		lineIncr < lines.size() && lineIncr < 10; 
		lineIncr++) {

		dist = lines[lineIncr][0];
		angle = lines[lineIncr][1];

		a = cos(theta);
		b = sin(theta);

		x0 = a*dist;
		y0 = b*dist;

		// accumulate all point coordinates into respective variables

		pt1.x += (x0 + 1000 * (-b));
		pt1.y += (y0 + 1000 * a);
		pt2.x += (x0 - 1000 * (-b));
		pt2.y += (y0 - 1000 * a);

	}

	// If no line was found
	if (lineIncr == 0){
		pt1.x = 200;
		pt1.y = IDEALDIST;
		pt2.x = 600;
		pt2.y = IDEALDIST;
		lineIncr = 1;
	}

	// Average points
	pt1.x /= lineIncr;
	pt1.y /= lineIncr;
	pt2.x /= lineIncr;
	pt2.y /= lineIncr;

	dist = getDistance() - IDEALDIST; // distance error
	angle = getAngle(); // angle of road w respect to headway

}

int ImageProcessor::getDistance() {

	denom = (pt2.x - pt1.x);
	if (denom == 0)
		denom = 0.00001;
	
	m = (pt2.y - pt1.y)/denom;
	b = pt1.y - (m * pt1.x);

	return (m * COLUMNTOCHECK) + b;

}

// Angle ranges from -pi/2 to pi/2
double ImageProcessor::getAngle() {
	return atan(m);
}

void ImageProcessor::resetPoints() {
	pt1.x = 0;
	pt1.y = 0;
	pt2.x = 0;
	pt2.y = 0;
}