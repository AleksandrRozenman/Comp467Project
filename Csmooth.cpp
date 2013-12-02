// Mean Smoothing Function (custom)
// by Steven Wirsz


#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

int threshold_value = 100;
Mat src, dst;

void Threshold_Demo( int, void* );

int main( int, char** argv )
{
  src = imread( argv[1], 1 );
  namedWindow( "Mean Smoothing", WINDOW_AUTOSIZE );
  createTrackbar( "% original",
                  "Mean Smoothing", &threshold_value,
                  100, Threshold_Demo );
    Threshold_Demo( 0, 0 );

	waitKey(); // pause before clearing images
}

void Threshold_Demo( int, void* )
{
  threshold( src, dst, 255, 0, 4 );
	for( int y = 2; y < src.rows-2; y++ ) { 
		for( int x = 2; x < src.cols-2; x++ ) { 
			for( int c = 0; c < 3; c++ ) {
				// 75% of image contrast from original pixel
				dst.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( (src.at<Vec3b>(y,x)[c])*(float)threshold_value/100 );
				// 25% of image contrast mean of surrounding 25 pixels (5x5)
				for ( int a = 0; a < 5; a++ ) {
					for ( int b = 0; b < 5; b++ ) {
						dst.at<Vec3b>(y,x)[c] += saturate_cast<uchar>( (src.at<Vec3b>(y-2+a,x-2+b)[c])/25*(1-(float)threshold_value/100) );
					}
				}
			}
		}
	}
  imshow( "Mean Smoothing", dst );
}
