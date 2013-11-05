// Contrast stretching #1 - proportional scaling
// by Steven Wirsz

#include <iostream>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
	// Read image given by user
	Mat image = imread( argv[1] );
	Mat bw_image = Mat::zeros( image.size(), image.type() ); // initialize temporary grayscale image
	cvtColor( image, bw_image, COLOR_BGR2GRAY ); // create greyscale image
	Canny( bw_image, bw_image, 10, 100, 3 ); // sharply define borders in the black-and-white image
	
	int left=1,right=bw_image.cols,top=1,bottom=bw_image.rows;

	// detect bottom of image
	bool diff=false;
	do {
		for( int n = left; n < right; n++ ) { 
			if (saturate_cast<uchar>(bw_image.at<Vec3b>(bottom,n)[1]) != saturate_cast<uchar>(bw_image.at<Vec3b>(bottom,left)[1]))
				diff=true;
		}
		bottom--;
	} while (diff==false && bottom >= 1);
	
	// detect top of image
	diff=false;
	do {
		for( int n = left; n < right; n++ ) { 
			if (saturate_cast<uchar>(bw_image.at<Vec3b>(top,n)[1]) != saturate_cast<uchar>(bw_image.at<Vec3b>(top,left)[1]))
				diff=true;
		}
		top++;
	} while (diff==false && top < bottom);
	
	// detect right of image
	diff=false;
	do {
		for( int n = top; n < bottom; n++ ) { 
			if (saturate_cast<uchar>(bw_image.at<Vec3b>(n,right)[1]) != saturate_cast<uchar>(bw_image.at<Vec3b>(top,right)[1]))
				diff=true;
		}
		right--;
	} while (diff==false && right >= 1);

	// detect left of image
	diff=false;
	do {
		for( int n = 1; n < bw_image.rows; n++ ) { 
			if (saturate_cast<uchar>(bw_image.at<Vec3b>(n,left)[1]) != saturate_cast<uchar>(bw_image.at<Vec3b>(top,left)[1])) {
				diff=true;
			}
		}
		left++;
	} while (diff==false && left<right);


	// debugging
	cout << "top: "<<top<<endl;
	cout << "left: "<<left<<endl;
	cout << "bottom: "<<bottom<<endl;
	cout << "right: "<<right<<endl;

	// define rectangle with new dimensions and resize
	cv::Rect rec(left, top, right-left, bottom-top);
	cv::Mat new_image = image(rec);
	
	// Show original and modified
	namedWindow("Original Image", 1);
	namedWindow("Contrast Stretched Image", 1);
	imshow("Original Image", bw_image);
	imshow("Contrast Stretched Image", new_image);

	waitKey(); // pause before clearing images
}
