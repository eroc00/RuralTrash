
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
//#include <raspicam/raspicam_cv.h>

#define ANGLE_CUTOFF 40*(CV_PI/180)

int main(int argc, char** argv){

	if (argc != 5){
		std::cout << "Invalid number of arguments.\n";
		return -1;
	}

	int blurDeviation = std::__cxx11::stoi(argv[1]);
	int lowThr = std::__cxx11::stoi(argv[2]);
	int max_lowThr = std::__cxx11::stoi(argv[3]);
	//int ratio = 3;
	int kernel_size = std::__cxx11::stoi(argv[4]);


	cv::Mat origimage, image, hsvImg, mask, maskSunny, edgeImg;
	origimage = cv::imread("../TestImages/roadside.jpg");
	cv::resize(origimage, image, cv::Size(1280, 960));

	cv::cvtColor(image, hsvImg, cv::COLOR_RGB2HSV);

	cv::inRange(hsvImg, 
			cv::Scalar(15, 6, 127),
			cv::Scalar(255, 255, 164), 
		    mask);

	cv::inRange(hsvImg, 
			cv::Scalar(15, 90, 0),
			cv::Scalar(255, 255, 164), 
		    maskSunny);

	blur(maskSunny, edgeImg, cv::Size(blurDeviation, blurDeviation));
	Canny(edgeImg, edgeImg, lowThr, max_lowThr, kernel_size);

	// Hough Transform
	std::vector<cv::Vec2f> lines;
	cv::HoughLines(edgeImg, lines, 100, 0.8*CV_PI/(180), 400, 0, 0, 0, ANGLE_CUTOFF);

	std::cout << "Lines found: " << lines.size() << std::endl;
	for (std::size_t i = 0; i < lines.size(); i++){
		float rho = lines[i][0], theta = lines[i][1];
		cv::Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*a);
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*a);
		
		if (i < 10)
			std::cout << "Distance: " << rho
				  << " Angle: " << theta << std::endl;
	
		line( image, pt1, pt2, cv::Scalar(0, 0, 255), 5, cv::LINE_AA);


	}



/*
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
	cv::imwrite("/home/pi/Desktop/Mask.jpg", mask);

	cv::imwrite("/home/pi/Desktop/MaskSunny.jpg", maskSunny);

	cv::imwrite("/home/pi/Desktop/CannyFilter.jpg", edgeImg);

	cv::imwrite("/home/pi/Desktop/lines.jpg", image);

	std::cout << "Finished Processing image \n";

	return 0;
}
