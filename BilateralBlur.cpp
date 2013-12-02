// Bilateral Blur
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
  namedWindow( "Bilateral Blur", WINDOW_AUTOSIZE );
  createTrackbar( "Bilateral",
                  "Bilateral Blur", &threshold_value,
                  20, Threshold_Demo );

    Threshold_Demo( 0, 0 );
	waitKey(); // pause before clearing images
}

void Threshold_Demo( int, void* )
{
	bilateralFilter( src, dst, threshold_value*2+1, threshold_value*4+1, threshold_value+1 );
	imshow( "Bilateral Blur", dst );
}