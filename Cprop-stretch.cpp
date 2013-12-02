// Proportional Contrast Stretching
// by Steven Wirsz

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

double maxval = 0;
double minval = 255; // Stores minimum and maximum grayscale values

int threshold_value = 0;
Mat src, dst, bw;

void Threshold_Demo( int, void* );

int main( int, char** argv )
{
  src = imread( argv[1], 1 );
  namedWindow( "Proportional Contrast Stretching", WINDOW_AUTOSIZE );
  createTrackbar( "% modified",
                  "Proportional Contrast Stretching", &threshold_value,
                  100, Threshold_Demo );

  
	cv::cvtColor( src, bw, cv::COLOR_BGR2GRAY ); // create greyscale image

	// Traverse through image, find minimum and maximum
	for( int y = 0; y < bw.rows; y++ ) { 
		for( int x = 0; x < bw.cols; x++ ) { 
			if (minval>cv::saturate_cast<uchar>(bw.at<cv::Vec3b>(y,x)[0]))
				minval = cv::saturate_cast<uchar>(bw.at<cv::Vec3b>(y,x)[0]);
			if (maxval<cv::saturate_cast<uchar>(bw.at<cv::Vec3b>(y,x)[0]))
				maxval = cv::saturate_cast<uchar>(bw.at<cv::Vec3b>(y,x)[0]);
		}
	}

    Threshold_Demo( 0, 0 );
	waitKey(); // pause before clearing images
}

void Threshold_Demo( int, void* )
{
  threshold( src, dst, 255, 0, 4 );
	for( int y = 0; y < src.rows; y++ ) { 
		for( int x = 0; x < src.cols; x++ ) { 
			for( int c = 0; c < 3; c++ ) {
		// Create new_image by shifting minimum contrast down to zero and multiplying to stretch maximum contrast to 255
				double shift = -(minval)*((double)threshold_value/100);
				double scale = 1+((255/(maxval-minval))-1)*((double)threshold_value/100);
				
				dst.at<Vec3b>(y,x)[c] = saturate_cast<uchar>(scale*( src.at<Vec3b>(y,x)[c] + shift) );
			}
		}
	}

  imshow( "Proportional Contrast Stretching", dst );
}