// Threshold White to Zero

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

int threshold_value = 255;
Mat src, dst;

void Threshold_Demo( int, void* );

int main( int, char** argv )
{
  src = imread( argv[1], 1 );
  namedWindow( "Demo", WINDOW_AUTOSIZE );
  createTrackbar( "White->0",
                  "Demo", &threshold_value,
                  255, Threshold_Demo );
    Threshold_Demo( 0, 0 );

	waitKey(); // pause before clearing images
}

void Threshold_Demo( int, void* )
{
  threshold( src, dst, threshold_value, 0, 2 );
  imshow( "Demo", dst );
}
