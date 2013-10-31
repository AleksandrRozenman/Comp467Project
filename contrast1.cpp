// Contrast stretching #1 - proportional scaling
// by Steven Wirsz

#include <iostream>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main2( int argc, char** argv )
{
	double maxval = 0;
	double minval = 255; // Stores minimum and maximum grayscale values

	// Read image given by user
	Mat image = imread( argv[1] );
	Mat new_image = Mat::zeros( image.size(), image.type() ); // initialize output image
	Mat new_image2 = Mat::zeros( image.size(), image.type() ); // initialize output image
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
	cout << "max: "<<maxval<<std::endl;
	cout << "stretch ratio: "<<(255/(maxval-minval))<<endl;

	
	// Create new_image by shifting minimum contrast down to zero and multiplying to stretch maximum contrast to 255
	for( int y = 0; y < image.rows; y++ ) { 
		for( int x = 0; x < image.cols; x++ ) { 
			for( int c = 0; c < 3; c++ ) {
				new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( (255/(maxval-minval))*( image.at<Vec3b>(y,x)[c] -minval) );
			}
		}
	}
	// Smooth new_image by 
	for( int y = 2; y < image.rows-2; y++ ) { 
		for( int x = 2; x < image.cols-2; x++ ) { 
			for( int c = 0; c < 3; c++ ) {
				// 75% of image contrast from original pixel
				new_image2.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( (new_image.at<Vec3b>(y,x)[c])*0.75 );
				// 25% of image contrast mean of surrounding 25 pixels (5x5)
				for ( int a = 0; a < 5; a++ ) {
					for ( int b = 0; b < 5; b++ ) {
						new_image2.at<Vec3b>(y,x)[c] += saturate_cast<uchar>( (new_image.at<Vec3b>(y-2+a,x-2+c)[c])/25*0.25 );
					}
				}
			}
		}
	}
	// Show original and modified
	namedWindow("Original Image", 1);
	namedWindow("Contrast Stretched Image", 1);
	namedWindow("Smoothed and Stretched Image", 1);
	imshow("Original Image", image);
	imshow("Contrast Stretched Image", new_image);
	imshow("Smoothed and Stretched Image", new_image2);

	waitKey(); // pause before clearing images
}