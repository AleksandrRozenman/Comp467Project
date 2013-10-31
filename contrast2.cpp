// Contrast stretching #2 - non-proportional scaling
// by Steven Wirsz

#include <iostream>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;


int main( int argc, char** argv )
{
	double maxval[3] = {0, 0, 0};
	double minval[3] = {255, 255, 255}; // Stores min and max values

	// Read image given by user
	Mat image = imread( argv[1] );
	Mat new_image = Mat::zeros( image.size(), image.type() ); // initialize output image
	Mat new_image2 = Mat::zeros( image.size(), image.type() ); // initialize output image

	// Create new_image by shifting minimum contrast down to zero and multiplying to stretch maximum contrast to 255
	for( int y = 0; y < image.rows; y++ ) { 
		for( int x = 0; x < image.cols; x++ ) { 
			for( int c = 0; c < 3; c++ ) {
				if (minval[c]>saturate_cast<uchar>(image.at<Vec3b>(y,x)[c]))
					minval[c] = saturate_cast<uchar>(image.at<Vec3b>(y,x)[c]);
				if (maxval[c]<saturate_cast<uchar>(image.at<Vec3b>(y,x)[c]))
					maxval[c] = saturate_cast<uchar>(image.at<Vec3b>(y,x)[c]);
				new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( (255/(maxval[c]-minval[c]))*( image.at<Vec3b>(y,x)[c] -minval[c]) );
			}
		}
	}

	
	// debugging	
	for( int c = 0; c < 3; c++ ) {
		cout << "color: "<<c<<endl;
		cout << "min: "<<minval[c]<<endl;
		cout << "min: "<<maxval[c]<<std::endl;
		cout << "stretch ratio: "<<(255/(maxval[c]-minval[c]))<<endl;
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