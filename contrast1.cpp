// Contrast stretching
// by Steven Wirsz

// To do: 

// Ignore outliers
// apply statistical color blending
// Collect a number of test images to demonstrate code

#include <iostream>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

double maxval, minval; // Stores minimum and maximum grayscale values

int main( int argc, char** argv )
{
	// Read image given by user
	Mat image = imread( argv[1] );
	Mat new_image = Mat::zeros( image.size(), image.type() ); // initialize output image
	Mat bw_image = Mat::zeros( image.size(), image.type() ); // initialize temporary grayscale image
	cvtColor( image, bw_image, COLOR_BGR2GRAY ); // create greyscale image

	// Traverse through image, find minimum and maximum
	for( int y = 0; y < bw_image.rows; y++ ) { 
		for( int x = 0; x < bw_image.cols; x++ ) { 
			if (minval>saturate_cast<uchar>(image.at<Vec3b>(y,x)[1]))
				minval = saturate_cast<uchar>(image.at<Vec3b>(y,x)[1]);
			if (maxval<saturate_cast<uchar>(image.at<Vec3b>(y,x)[1]))
				maxval = saturate_cast<uchar>(image.at<Vec3b>(y,x)[1]);
		}
	}

	// debugging
	cout << "min: "<<minval<<endl;
	cout << "min: "<<maxval<<std::endl;
	cout << "stretch ratio: "<<(255/(maxval-minval))<<endl;

	
	// Create new_image by shifting minimum contrast down to zero and multiplying to stretch maximum contrast to 255
	for( int y = 0; y < image.rows; y++ ) { 
		for( int x = 0; x < image.cols; x++ ) { 
			for( int c = 0; c < 3; c++ ) {
				new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( (255.0/(maxval-minval))*( image.at<Vec3b>(y,x)[c] ) - minval );
			}
		}
	}

	// Show original and modified
	namedWindow("Original Image", 1);
	namedWindow("Modified Image", 1);
	imshow("Original Image", image);
	imshow("Modified Image", new_image);

	waitKey(); // pause before clearing images
}