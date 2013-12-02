// Color contrast stretching - non-proportional scaling
// by Steven Wirsz

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

double maxval[3] = {0, 0, 0};
double minval[3] = {255, 255, 255}; // Stores min and max values

int threshold_value = 0;
Mat src, dst;

void Threshold_Demo( int, void* );

int main( int, char** argv )
{
  src = imread( argv[1], 1 );
  namedWindow( "Color Stretching", WINDOW_AUTOSIZE );
  createTrackbar( "% modified",
                  "Color Stretching", &threshold_value,
                  100, Threshold_Demo );

  	// Traverse through image, find minimum and maximum
	for( int y = 0; y < src.rows; y++ ) { 
		for( int x = 0; x < src.cols; x++ ) { 
			for( int c = 0; c < 3; c++ ) {
				if (minval[c]>saturate_cast<uchar>(src.at<Vec3b>(y,x)[c]))
					minval[c] = saturate_cast<uchar>(src.at<Vec3b>(y,x)[c]);
				if (maxval[c]<saturate_cast<uchar>(src.at<Vec3b>(y,x)[c]))
					maxval[c] = saturate_cast<uchar>(src.at<Vec3b>(y,x)[c]);
			}
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
				double shift = -(minval[c])*((double)threshold_value/100);
				double scale = 1+((255/(maxval[c]-minval[c]))-1)*((double)threshold_value/100);
				
				dst.at<Vec3b>(y,x)[c] = saturate_cast<uchar>(scale*( src.at<Vec3b>(y,x)[c] + shift) );
			}
		}
	}

  imshow( "Color Stretching", dst );
}