// Gaussian Blur
// by Steven Wirsz

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

int threshold_value = 0;
Mat src, dst;

void Threshold_Demo( int, void* );

int main( int, char** argv )
{
  src = imread( argv[1], 1 );
  namedWindow( "Gaussian Blur", WINDOW_AUTOSIZE );
  createTrackbar( "Blur",
                  "Gaussian Blur", &threshold_value,
                  30, Threshold_Demo );

    Threshold_Demo( 0, 0 );
	waitKey(); // pause before clearing images
}

void Threshold_Demo( int, void* )
{
	GaussianBlur( src, dst, Size( threshold_value*2+1, threshold_value*2+1 ), 0, 0 );

	imshow( "Gaussian Blur", dst );
}