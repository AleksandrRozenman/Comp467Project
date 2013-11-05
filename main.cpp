/**
 * Comp467Project: Group A2
 * Jovan Ruano
 * Steven Wirsz
 * Aleksandr Rozenman
 */

// Headers

// C/C++ headers
#include <string>

// Windows headers
#include <Windows.h>
#include <WindowsX.h> // For testing checkboxes.

// OpenCV headers
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Window descriptors.
HWND mainWindowHandle = 0;
HINSTANCE hInstance = 0;
std::wstring mainWindowCaption = L"Comp 467 Project: Group A2";

// Default window dimensions.
int windowX = CW_USEDEFAULT, windowY = CW_USEDEFAULT;
int windowWidth = 800, windowHeight = 600;
int width = 0, height = 0;


// Controls.
// Contrast--proportional.
HWND propContrastCheckBox = 0;
HWND propContrastText = 0;
HWND propContrastButton = 0;

// Contrast--non-proportional.
HWND nonPropContrastCheckBox = 0;
HWND nonPropContrastText = 0;
HWND nonPropContrastButton = 0;

// Resizing
HWND resizeCheckBox = 0;
HWND resizeText = 0;
HWND resizeButton = 0;

// Others
HWND browseText = 0;
HWND browseButton = 0;
bool imageLoaded = false;
//HWND runAllCheckedText = 0;
HWND runAllCheckedButton = 0;
HWND saveButton = 0;

// Image
cv::Mat image;
cv::Mat correctedImage;
bool correctedImageIsNull = true;

char* name;
char* saveName;

// Functions
int initMainWindow();
LRESULT CALLBACK mainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool readImage();
bool saveImage();
cv::Mat propContrast(cv::Mat img);
cv::Mat nonPropContrast(cv::Mat img);
cv::Mat resize(cv::Mat img);
int run();

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR lpCmdLine, int nCmdShow)
{
	int retVal = 0;

	/*
	// Read image.
	image = cv::imread("opencv-logo.png", CV_LOAD_IMAGE_COLOR);
	if(!image.data) // Check for invalid input
	{
		MessageBox(0, L"Could not open or find the image", L"ERROR", MB_OK | MB_ICONERROR);
		return -1;
	}
	//*/

	hInstance = hInst;
	if(retVal = initMainWindow())
		return retVal;

	retVal = run();
	
	return retVal;
}

int initMainWindow()
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = mainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"WindowClassName";
	wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(0, L"RegisterClass failed.", L"initMainWindow()", MB_OK | MB_ICONERROR);
		return 2;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT r = {0, 0, windowWidth, windowHeight};
    AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, false);
	width  = r.right - r.left;
	height = r.bottom - r.top;

	mainWindowHandle = CreateWindowEx(0,
	                                  L"WindowClassName",
	                                  mainWindowCaption.c_str(),
	                                  WS_OVERLAPPEDWINDOW,
	                                  windowX,
	                                  windowY,
	                                  width,
	                                  height,
	                                  0,
	                                  0,
	                                  hInstance,
	                                  0
	                                 );

	if(!mainWindowHandle)
	{
		MessageBox(0, L"CreateWindow failed.", L"initMainWindow()", MB_OK | MB_ICONERROR);
		return 3;
	}

	browseText = CreateWindow(L"STATIC",  // Predefined class; Unicode assumed
	                          L"Click \"Browse...\" to select an image",      // Button text
	                          WS_VISIBLE | WS_CHILD | SS_LEFT,  // Styles
	                          10,         // x position
	                          15,         // y position
	                          250,        // Button width
	                          20,        // Button height
	                          mainWindowHandle,     // Parent window
	                          NULL,       // No menu.
	                          (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                          NULL  // Pointer not needed.
							 );

	browseButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                            L"Browse...",      // Button text
	                            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                            265,         // x position
	                            10,         // y position
	                            100,        // Button width
	                            25,        // Button height
	                            mainWindowHandle,     // Parent window
	                            NULL,       // No menu.
	                            (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                            NULL  // Pointer not needed.
							   );

	resizeCheckBox = CreateWindow(L"BUTTON",
	                              L"Automatic Resize",
	                              WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
	                              10,
	                              75,
	                              15,
	                              15,
	                              mainWindowHandle,
	                              NULL,
	                              (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                              NULL
	                             );

	resizeText = CreateWindow(L"STATIC",  // Predefined class; Unicode assumed
	                          L"Automatic Resize",      // Button text
	                          WS_VISIBLE | WS_CHILD | SS_LEFT,  // Styles
	                          30,         // x position
	                          75,         // y position
	                          250,        // Button width
	                          20,        // Button height
	                          mainWindowHandle,     // Parent window
	                          NULL,       // No menu.
	                          (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                          NULL  // Pointer not needed.
	                         );

	resizeButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                            L"Run",      // Button text
	                            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                            350,         // x position
	                            70,         // y position
	                            100,        // Button width
	                            25,        // Button height
	                            mainWindowHandle,     // Parent window
	                            NULL,       // No menu.
	                            (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                            NULL  // Pointer not needed.
	                           );

	propContrastCheckBox = CreateWindow(L"BUTTON",
	                                    L"Contrast stretching (proportional)",
	                                    WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
	                                    10,
	                                    105,
	                                    15,
	                                    15,
	                                    mainWindowHandle,
	                                    NULL,
	                                    (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                    NULL
	                                   );

	propContrastText = CreateWindow(L"STATIC",  // Predefined class; Unicode assumed
	                                L"Contrast stretching (proportional)",      // Button text
	                                WS_VISIBLE | WS_CHILD | SS_LEFT,  // Styles
	                                30,         // x position
	                                105,         // y position
	                                250,        // Button width
	                                20,        // Button height
	                                mainWindowHandle,     // Parent window
	                                NULL,       // No menu.
	                                (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                NULL  // Pointer not needed.
							       );

	propContrastButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                                  L"Run",      // Button text
	                                  WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                                  350,         // x position
	                                  100,         // y position
	                                  100,        // Button width
	                                  25,        // Button height
	                                  mainWindowHandle,     // Parent window
	                                  NULL,       // No menu.
	                                  (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                  NULL  // Pointer not needed.
							         );

	nonPropContrastCheckBox = CreateWindow(L"BUTTON",
	                                       L"Contrast stretching (non-proportional)",
	                                       WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
	                                       10,
	                                       135,
	                                       15,
	                                       15,
	                                       mainWindowHandle,
	                                       NULL,
	                                       (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                       NULL
	                                      );

	nonPropContrastText = CreateWindow(L"STATIC",  // Predefined class; Unicode assumed
	                                   L"Contrast stretching (non-proportional)",      // Button text
	                                   WS_VISIBLE | WS_CHILD | SS_LEFT,  // Styles
	                                   30,         // x position
	                                   135,         // y position
	                                   250,        // Button width
	                                   20,        // Button height
	                                   mainWindowHandle,     // Parent window
	                                   NULL,       // No menu.
	                                   (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                   NULL  // Pointer not needed.
							          );

	nonPropContrastButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                                     L"Run",      // Button text
	                                     WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                                     350,         // x position
	                                     130,         // y position
	                                     100,        // Button width
	                                     25,        // Button height
	                                     mainWindowHandle,     // Parent window
	                                     NULL,       // No menu.
	                                     (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                     NULL  // Pointer not needed.
							            );

	runAllCheckedButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                                   L"Run all checked operations",      // Button text
	                                   WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                                   400,         // x position
	                                   400,         // y position
	                                   250,        // Button width
	                                   25,        // Button height
	                                   mainWindowHandle,     // Parent window
	                                   NULL,       // No menu.
	                                   (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                   NULL  // Pointer not needed.
							          );

	saveButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                          L"Save image...",      // Button text
	                          WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                          525,         // x position
	                          430,         // y position
	                          125,        // Button width
	                          25,        // Button height
	                          mainWindowHandle,     // Parent window
	                          NULL,       // No menu.
	                          (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                          NULL  // Pointer not needed.
							 );

	ShowWindow(mainWindowHandle, SW_SHOW);
	UpdateWindow(mainWindowHandle);

	return 0;
}

LRESULT CALLBACK mainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	/*
	// Display window w/ image when the main window is left-clicked on.
	case WM_LBUTTONDOWN:
		cv::namedWindow("Display window", CV_WINDOW_AUTOSIZE);// Create a window for display.
		cv::imshow("Display window", image);
		return 0;
	//*/

	case WM_COMMAND:
		if((HWND)lParam == browseButton)
		{
			if(readImage())
			{
				imageLoaded = true;
				SetWindowTextA(mainWindowHandle, name);
			}
		}
		else if((HWND)lParam == propContrastButton)
		{
			if(imageLoaded)
			{
				correctedImage = propContrast(image);
				correctedImageIsNull = false;
				cv::namedWindow("Corrected Image", 1);
				cv::imshow("Corrected Image", correctedImage);
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == nonPropContrastButton)
		{
			if(imageLoaded)
			{
				correctedImage = nonPropContrast(image);
				correctedImageIsNull = false;
				cv::namedWindow("Corrected Image", 1);
				cv::imshow("Corrected Image", correctedImage);
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == resizeButton)
		{
			if(imageLoaded)
			{
				correctedImage = resize(image);
				correctedImageIsNull = false;
				cv::namedWindow("Corrected Image", 1);
				cv::imshow("Corrected Image", correctedImage);
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == runAllCheckedButton)
		{
			if(imageLoaded)
			{
				correctedImage = image;//nonPropContrast(image);

				if(BST_CHECKED == Button_GetCheck(resizeCheckBox))
					correctedImage = resize(correctedImage);
				if(BST_CHECKED == Button_GetCheck(propContrastCheckBox))
					correctedImage = propContrast(correctedImage);
				if(BST_CHECKED == Button_GetCheck(nonPropContrastCheckBox))
					correctedImage = nonPropContrast(correctedImage);

				correctedImageIsNull = false;
				cv::namedWindow("Corrected Image", 1);
				cv::imshow("Corrected Image", correctedImage);
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == saveButton)
		{
			if(correctedImageIsNull)
			{
				MessageBox(0, L"Error:\nPerform one or more cleanup operations.", L"Save Image Error", MB_OK | MB_ICONERROR);
			}
			else
			{
				if(saveImage())
				{
					try {
						cv::imwrite(saveName, correctedImage);
					}
					catch (std::exception& ex) {
						MessageBox(0, L"Error:\nCould not save image.", L"Save Image Error", MB_OK | MB_ICONERROR);
						//return 1;
					}
				}
				/*
				else
				{
					MessageBox(0, L"Error:\nCould not save image.", L"Save Image Error", MB_OK | MB_ICONERROR);
				}
				//*/
			}
		}
		
		return 0;

	case WM_MENUCHAR:
        // Don't beep when we alt-enter.
        return MAKELRESULT(0, MNC_CLOSE);

	// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = width;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = height;
		return 0;
	}

	// Forward any other messages not handled to default window procedure;
	// return the return value of DefWindowProc.
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool readImage()
{
	/*
	// Read image.
	image = cv::imread("opencv-logo.png", CV_LOAD_IMAGE_COLOR);
	if(!image.data) // Check for invalid input
	{
		MessageBox(0, L"Could not open or find the image", L"ERROR", MB_OK | MB_ICONERROR);
		return -1;
	}
	//*/

	OPENFILENAMEA ofn;       // common dialog box structure
	char szFile[MAX_PATH];       // buffer for file name
	//char fName[260];
	//HWND hwnd;              // owner window
	//FILE* hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = mainWindowHandle;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Widnows bitmap (*.bmp; *.dib)\0*.bmp;*.dib\0Portable image format (*.pbm; *.pgm; *.pmm)\0*.pbm;*.pgm;*.pmm\0Sun raster (*.sr; *.ras)\0*.sr;*.ras\0JPEG (*.jpg; *.jpeg; *.jpe)\0*.jpg;*.jpeg;*.jpe\0JPEG 2000 (*.jp2)\0*.jp2\0TIFF (*.tiff; *.tif)\0*.tiff;*.tif\0PNG (*.png)\0*.png\0All\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileNameA(&ofn)==TRUE)
	{
		/*
		hf = CreateFile(ofn.lpstrFile, 
		                GENERIC_READ,
		                0,
		                (LPSECURITY_ATTRIBUTES)NULL,
		                OPEN_EXISTING,
		                FILE_ATTRIBUTE_NORMAL,
		                (HANDLE)NULL
		               );
		//*/
		/*
		for(int i = 0; i < 260; i++)
		{
			fName[i] = (char)szFile[i];
		}
		//*/
		//*
		image = cv::imread(szFile, CV_LOAD_IMAGE_COLOR);
		if(!image.data) // Check for invalid input
		{
			MessageBox(0, L"Could not open or find the image", L"ERROR", MB_OK | MB_ICONERROR);
			return false;
		}
		//*/

		name = szFile;
		return true;
	}
	MessageBox(0, L"Could not open or find the image", L"ERROR", MB_OK | MB_ICONERROR);
	return false;
}

bool saveImage()
{
	OPENFILENAMEA ofn;       // common dialog box structure
	char szFile[MAX_PATH];       // buffer for file name
	//char fName[260];
	//HWND hwnd;              // owner window
	//FILE* hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = mainWindowHandle;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.lpstrFilter = "Widnows bitmap (*.bmp; *.dib)\0*.bmp;*.dib\0Portable image format (*.pbm; *.pgm; *.pmm)\0*.pbm;*.pgm;*.pmm\0Sun raster (*.sr; *.ras)\0*.sr;*.ras\0JPEG (*.jpg; *.jpeg; *.jpe)\0*.jpg;*.jpeg;*.jpe\0JPEG 2000 (*.jp2)\0*.jp2\0TIFF (*.tiff; *.tif)\0*.tiff;*.tif\0PNG (*.png)\0*.png\0All\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_OVERWRITEPROMPT;

	if(GetSaveFileNameA(&ofn) == TRUE)
	{
		saveName = szFile;
		return true;
	}
	return false;
}

cv::Mat propContrast(cv::Mat img)
{
	double maxval = 0;
	double minval = 255; // Stores minimum and maximum grayscale values

	// Read image given by user
	cv::Mat image = img;
	cv::Mat new_image = cv::Mat::zeros( image.size(), image.type() ); // initialize output image
	cv::Mat new_image2 = cv::Mat::zeros( image.size(), image.type() ); // initialize output image
	cv::Mat bw_image = cv::Mat::zeros( image.size(), image.type() ); // initialize temporary grayscale image
	cv::cvtColor( image, bw_image, cv::COLOR_BGR2GRAY ); // create greyscale image

	// Traverse through image, find minimum and maximum
	for( int y = 0; y < bw_image.rows; y++ ) { 
		for( int x = 0; x < bw_image.cols; x++ ) { 
			if (minval>cv::saturate_cast<uchar>(image.at<cv::Vec3b>(y,x)[1]))
				minval = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(y,x)[1]);
			if (maxval<cv::saturate_cast<uchar>(image.at<cv::Vec3b>(y,x)[1]))
				maxval = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(y,x)[1]);
		}
	}

	/*
	// debugging
	cout << "min: "<<minval<<endl;
	cout << "max: "<<maxval<<std::endl;
	cout << "stretch ratio: "<<(255/(maxval-minval))<<endl;
	//*/

	
	// Create new_image by shifting minimum contrast down to zero and multiplying to stretch maximum contrast to 255
	for( int y = 0; y < image.rows; y++ ) { 
		for( int x = 0; x < image.cols; x++ ) { 
			for( int c = 0; c < 3; c++ ) {
				new_image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( (255/(maxval-minval))*( image.at<cv::Vec3b>(y,x)[c] -minval) );
			}
		}
	}
	// Smooth new_image by 
	for( int y = 2; y < image.rows-2; y++ ) { 
		for( int x = 2; x < image.cols-2; x++ ) { 
			for( int c = 0; c < 3; c++ ) {
				// 75% of image contrast from original pixel
				new_image2.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( (new_image.at<cv::Vec3b>(y,x)[c])*0.75 );
				// 25% of image contrast mean of surrounding 25 pixels (5x5)
				for ( int a = 0; a < 5; a++ ) {
					for ( int b = 0; b < 5; b++ ) {
						new_image2.at<cv::Vec3b>(y,x)[c] += cv::saturate_cast<uchar>( (new_image.at<cv::Vec3b>(y-2+a,x-2+c)[c])/25*0.25 );
					}
				}
			}
		}
	}

	return new_image2;
}
cv::Mat nonPropContrast(cv::Mat img)
{
	double maxval[3] = {0, 0, 0};
	double minval[3] = {255, 255, 255}; // Stores min and max values

	// Read image given by user
	cv::Mat image = img;
	cv::Mat new_image = cv::Mat::zeros( image.size(), image.type() ); // initialize output image
	cv::Mat new_image2 = cv::Mat::zeros( image.size(), image.type() ); // initialize output image

	// Create new_image by shifting minimum contrast down to zero and multiplying to stretch maximum contrast to 255
	for( int y = 0; y < image.rows; y++ ) { 
		for( int x = 0; x < image.cols; x++ ) { 
			for( int c = 0; c < 3; c++ ) {
				if (minval[c]>cv::saturate_cast<uchar>(image.at<cv::Vec3b>(y,x)[c]))
					minval[c] = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(y,x)[c]);
				if (maxval[c]<cv::saturate_cast<uchar>(image.at<cv::Vec3b>(y,x)[c]))
					maxval[c] = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(y,x)[c]);
				new_image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( (255/(maxval[c]-minval[c]))*( image.at<cv::Vec3b>(y,x)[c] -minval[c]) );
			}
		}
	}

	/*
	// debugging	
	for( int c = 0; c < 3; c++ ) {
		cout << "color: "<<c<<endl;
		cout << "min: "<<minval[c]<<endl;
		cout << "min: "<<maxval[c]<<std::endl;
		cout << "stretch ratio: "<<(255/(maxval[c]-minval[c]))<<endl;
	}
	//*/

	// Smooth new_image by 
	for( int y = 2; y < image.rows-2; y++ ) { 
		for( int x = 2; x < image.cols-2; x++ ) { 
			for( int c = 0; c < 3; c++ ) {
				// 75% of image contrast from original pixel
				new_image2.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( (new_image.at<cv::Vec3b>(y,x)[c])*0.75 );
				// 25% of image contrast mean of surrounding 25 pixels (5x5)
				for ( int a = 0; a < 5; a++ ) {
					for ( int b = 0; b < 5; b++ ) {
						new_image2.at<cv::Vec3b>(y,x)[c] += cv::saturate_cast<uchar>( (new_image.at<cv::Vec3b>(y-2+a,x-2+c)[c])/25*0.25 );
					}
				}
			}
		}
	}

	return new_image2;
}

cv::Mat resize(cv::Mat img)
{
	cv::Mat image = img;
	cv::Mat bw_image = cv::Mat::zeros( image.size(), image.type() ); // initialize temporary grayscale image
	cv::cvtColor( image, bw_image, cv::COLOR_BGR2GRAY ); // create greyscale image
	cv::Canny( bw_image, bw_image, 10, 100, 3 ); // sharply define borders in the black-and-white image
	
	int left=1,right=bw_image.cols,top=1,bottom=bw_image.rows;

	// detect bottom of image
	bool diff=false;
	do {
		for( int n = left; n < right; n++ ) { 
			if (cv::saturate_cast<uchar>(bw_image.at<cv::Vec3b>(bottom,n)[1]) != cv::saturate_cast<uchar>(bw_image.at<cv::Vec3b>(bottom,left)[1]))
				diff=true;
		}
		bottom--;
	} while (diff==false && bottom >= 1);
	
	// detect top of image
	diff=false;
	do {
		for( int n = left; n < right; n++ ) { 
			if (cv::saturate_cast<uchar>(bw_image.at<cv::Vec3b>(top,n)[1]) != cv::saturate_cast<uchar>(bw_image.at<cv::Vec3b>(top,left)[1]))
				diff=true;
		}
		top++;
	} while (diff==false && top < bottom);
	
	// detect right of image
	diff=false;
	do {
		for( int n = top; n < bottom; n++ ) { 
			if (cv::saturate_cast<uchar>(bw_image.at<cv::Vec3b>(n,right)[1]) != cv::saturate_cast<uchar>(bw_image.at<cv::Vec3b>(top,right)[1]))
				diff=true;
		}
		right--;
	} while (diff==false && right >= 1);

	// detect left of image
	diff=false;
	do {
		for( int n = 1; n < bw_image.rows; n++ ) { 
			if (cv::saturate_cast<uchar>(bw_image.at<cv::Vec3b>(n,left)[1]) != cv::saturate_cast<uchar>(bw_image.at<cv::Vec3b>(top,left)[1])) {
				diff=true;
			}
		}
		left++;
	} while (diff==false && left<right);

	// define rectangle with new dimensions and resize
	cv::Rect rec(left, top, right-left, bottom-top);
	cv::Mat new_image = image(rec);
	
	// Show original and modified
	return new_image;
}

int run()
{
	MSG msg = {0};
	
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
            TranslateMessage(&msg);
            DispatchMessage(&msg);
		}
		/*
		else
        {	
			timer.tick();

			if(!paused)
			{
				calcFrameStats();
				updateScene(timer.getDeltaTime());	
				drawScene();
			}
			else
			{
				Sleep(100);
			}
        }
		//*/
	}

	// Try destroying the window; return an error on failure.
	if(!DestroyWindow(mainWindowHandle))
		return 1;

	return (int)msg.wParam;
}
