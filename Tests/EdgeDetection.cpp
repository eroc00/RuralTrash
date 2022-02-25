
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

	// Convert RGB img to HSV img
	cv::cvtColor(image, hsvImg, cv::COLOR_RGB2HSV);

	/* SEPARATE ROAD FROM TERRAIN */
	cv::inRange(hsvImg, // non-Sunny road
			cv::Scalar(0, 0, 0), // 0, 45, 0
			cv::Scalar(60, 135, 122), // 60, 150, 122
		    mask);

	cv::inRange(hsvImg, // sunny road
			cv::Scalar(80, 0, 0),
			cv::Scalar(255, 41, 155), 
		    maskSunny);
		    
		    // Segment Grass: lower-(30, 57, 0), upper-(255, 255, 218)

	edgeImg = mask | maskSunny;
	
	/* ERODE/DILATE */
	int dilation_size = 3;
	int dilation_type = cv::MORPH_CROSS; // RECT, cross, ellipse
	cv::Mat dilate_element = cv::getStructuringElement( cv::MORPH_ELLIPSE,
						cv::Size( 2*dilation_size + 1, 2*dilation_size + 1), 
						cv::Point( dilation_size, dilation_size ) );
	
	dilation_size = 5;
	cv::Mat erode_element = cv::getStructuringElement( dilation_type,
					cv::Size( 2*dilation_size + 1, 2*dilation_size + 1), 
					cv::Point( dilation_size, dilation_size ) );

	cv::dilate(edgeImg, edgeImg, dilate_element);
	cv::erode(edgeImg, edgeImg, erode_element);

	cv::imwrite("/home/pi/Desktop/TrueMask.jpg", edgeImg);

	blur(edgeImg, edgeImg, cv::Size(blurDeviation, blurDeviation));
	Canny(edgeImg, edgeImg, lowThr, max_lowThr, kernel_size);

	// Hough Transform
	std::vector<cv::Vec2f> lines;
	cv::HoughLines(edgeImg, lines, 25, 0.4*CV_PI/(180), 300, 0, 0, 0);

	std::cout << "Lines found: " << lines.size() << std::endl;
	cv::Point avgpt1, avgpt2;
	size_t i = 0;
	for (; 
			i < lines.size() && i < 10;
			i++){
		float rho = lines[i][0], theta = lines[i][1];
		cv::Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*a);
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*a);
		
		std::cout << "Distance: " << rho
				  << " Angle: " << theta << std::endl;
	
		//line( image, pt1, pt2, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);

		avgpt1.x += cvRound(x0 + 1000*(-b));
		avgpt1.y += cvRound(y0 + 1000*a);
		avgpt2.x += cvRound(x0 - 1000*(-b));
		avgpt2.y += cvRound(y0 - 1000*a);
	}
	
	/* DRAW AVERAGE LINE */
	if (i < 1) i = 1;
	
	avgpt1.x /= (double)i;
	avgpt1.y /= (double)i;
	avgpt2.x /= (double)i;
	avgpt2.y /= (double)i;
	line( image, avgpt1, avgpt2, cv::Scalar(255, 0, 0), 5, cv::LINE_AA);


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
	cv::imwrite("/home/pi/Desktop/MaskNonSunny.jpg", mask);

	cv::imwrite("/home/pi/Desktop/MaskSunny.jpg", maskSunny);

	cv::imwrite("/home/pi/Desktop/CannyFilter.jpg", edgeImg);

	cv::imwrite("/home/pi/Desktop/lines.jpg", image);

	std::cout << "Finished Processing image \n";

	return 0;
}
