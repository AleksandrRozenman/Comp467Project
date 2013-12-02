// Add GaussianNoise
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
  namedWindow( "Add GaussianNoise", WINDOW_AUTOSIZE );
  createTrackbar( "% noise",
                  "Add GaussianNoise", &threshold_value,
                  100, Threshold_Demo );

    Threshold_Demo( 0, 0 );
	waitKey(); // pause before clearing images
}

void Threshold_Demo( int, void* )
{
	threshold( src, dst, 255, 0, 4 );
	dst = src.clone();
	Mat noisyI;
	noisyI.create (dst.rows,dst.cols,CV_32FC(1));
	noisyI.setTo (cv::Scalar::all (0));

	vector <Mat>  _channel;
	dst.convertTo (dst,CV_32FC(3),1.0,0);
	cv::split(dst,_channel);
	for(int i=0;i<dst.channels ();i++)
	{
		randn(noisyI, Scalar::all(0), Scalar::all((double)threshold_value)/2);
		add(_channel[i],noisyI,_channel[i]);
	}

	cv::merge (_channel,dst);
	dst.convertTo (dst,CV_8UC(3),1.0,0);

	imshow( "Add GaussianNoise", dst );
}