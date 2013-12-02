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
#include <CommCtrl.h>

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
//*
// Contrast--proportional.
HWND propContrastCheckBox = 0;
//HWND propContrastText = 0;
//HWND propContrastButton = 0;

// Contrast--non-proportional.
HWND nonPropContrastCheckBox = 0;
//HWND nonPropContrastText = 0;
//HWND nonPropContrastButton = 0;
//*/

// Resizing
HWND resizeCheckBox = 0;
HWND resizeText = 0;
HWND resizeButton = 0;

// Contrast--proportional
HWND propContrastText = 0;
HWND propContrastButton = 0;
HWND propContrastApplyButton = 0;
int propContrastValue = 0;
bool propContrastLoaded = false;
cv::Mat propContrastTempImage;
double propContrastMaxval = 0;
double propContrastMinval = 255; // Stores minimum and maximum grayscale values

// Contrast--non-proportional
HWND nonPropContrastText = 0;
HWND nonPropContrastButton = 0;
HWND nonPropContrastApplyButton = 0;
int nonPropContrastValue = 0;
bool nonPropContrastLoaded = false;
cv::Mat nonPropContrastTempImage;
double nonPropContrastMaxval[3] = {0, 0, 0};
double nonPropContrastMinval[3] = {255, 255, 255}; // Stores min and max values

// Gaussian blur
HWND blurText = 0;
HWND blurButton = 0;
HWND blurApplyButton = 0;
int blurValue = 0;
bool blurLoaded = false;
cv::Mat blurTempImage;

// Gaussian noise
HWND noiseText = 0;
HWND noiseButton = 0;
HWND noiseApplyButton = 0;
int noiseValue = 0;
bool noiseLoaded = false;
cv::Mat noiseTempImage;

// Bilateral filter
HWND bilateralText = 0;
HWND bilateralButton = 0;
HWND bilateralApplyButton = 0;
int bilateralValue = 0;
bool bilateralLoaded = false;
cv::Mat bilateralTempImage;

// Mean smoothing filter
HWND smoothText = 0;
HWND smoothButton = 0;
HWND smoothApplyButton = 0;
int smoothValue = 100;
bool smoothLoaded = false;
cv::Mat smoothTempImage;

// White to zero
HWND whiteText = 0;
HWND whiteButton = 0;
HWND whiteApplyButton = 0;
int whiteValue = 255;
bool whiteLoaded = false;
cv::Mat whiteTempImage;

// Black to zero
HWND blackText = 0;
HWND blackButton = 0;
HWND blackApplyButton = 0;
int blackValue = 0;
bool blackLoaded = false;
cv::Mat blackTempImage;

// Others
HWND browseText = 0;
HWND browseButton = 0;
bool imageLoaded = false;
//HWND runAllCheckedText = 0;
HWND runAllCheckedButton = 0;
HWND saveButton = 0;
HWND resetButton = 0;
HWND showCurrentButton = 0;
HWND runCombinedButton = 0;
HWND applyCombinedButton = 0;
bool combinedLoaded = false;

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
void blurCallback(int, void*);
void noiseCallback(int, void*);
void bilateralCallback(int, void*);
void smoothCallback(int, void*);
void whiteCallback(int, void*);
void blackCallback(int, void*);
void propContrastCallback(int, void*);
void nonPropContrastCallback(int, void*);
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
	/*
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
	//*/
	/*
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
	//*/

	propContrastText = CreateWindow(L"STATIC",  // Predefined class; Unicode assumed
	                                L"Contrast Stretching (Proportional)",      // Button text
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

	propContrastApplyButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                                       L"Apply",      // Button text
	                                       WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                                       455,         // x position
	                                       100,         // y position
	                                       100,        // Button width
	                                       25,        // Button height
	                                       mainWindowHandle,     // Parent window
	                                       NULL,       // No menu.
	                                       (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                       NULL  // Pointer not needed.
	                                      );

	nonPropContrastText = CreateWindow(L"STATIC",  // Predefined class; Unicode assumed
	                                   L"Contrast Stretching (Non-Proportional)",      // Button text
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

	nonPropContrastApplyButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                                          L"Apply",      // Button text
	                                          WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                                          455,         // x position
	                                          130,         // y position
	                                          100,        // Button width
	                                          25,        // Button height
	                                          mainWindowHandle,     // Parent window
	                                          NULL,       // No menu.
	                                          (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                          NULL  // Pointer not needed.
	                                         );

	blurText = CreateWindow(L"STATIC",  // Predefined class; Unicode assumed
	                        L"Gaussian Blur",      // Button text
	                        WS_VISIBLE | WS_CHILD | SS_LEFT,  // Styles
	                        30,         // x position
	                        165,         // y position
	                        250,        // Button width
	                        20,        // Button height
	                        mainWindowHandle,     // Parent window
	                        NULL,       // No menu.
	                        (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                        NULL  // Pointer not needed.
	                       );

	/*
	blurTrackbar = CreateWindowEx(0,
	                              L"TRACKBAR_CLASS",  // Predefined class; Unicode assumed
	                              L"Gaussian Blur",      // Button text
	                              WS_VISIBLE | WS_CHILD | TBS_AUTOTICKS,  // Styles
	                              130,         // x position
	                              165,         // y position
	                              215,        // Button width
	                              100,        // Button height
	                              mainWindowHandle,     // Parent window
	                              NULL,       // No menu.
	                              (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                              NULL  // Pointer not needed.
	                             );

	SendMessage(blurTrackbar, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(0, 100));
	//*/

	blurButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                          L"Run",      // Button text
	                          WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                          350,         // x position
	                          160,         // y position
	                          100,        // Button width
	                          25,        // Button height
	                          mainWindowHandle,     // Parent window
	                          NULL,       // No menu.
	                          (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                          NULL  // Pointer not needed.
	                         );

	blurApplyButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                               L"Apply",      // Button text
	                               WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                               455,         // x position
	                               160,         // y position
	                               100,        // Button width
	                               25,        // Button height
	                               mainWindowHandle,     // Parent window
	                               NULL,       // No menu.
	                               (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                               NULL  // Pointer not needed.
	                              );

	noiseText = CreateWindow(L"STATIC",  // Predefined class; Unicode assumed
	                         L"Gaussian Noise",      // Button text
	                         WS_VISIBLE | WS_CHILD | SS_LEFT,  // Styles
	                         30,         // x position
	                         195,         // y position
	                         250,        // Button width
	                         20,        // Button height
	                         mainWindowHandle,     // Parent window
	                         NULL,       // No menu.
	                         (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                         NULL  // Pointer not needed.
	                        );

	noiseButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                           L"Run",      // Button text
	                           WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                           350,         // x position
	                           190,         // y position
	                           100,        // Button width
	                           25,        // Button height
	                           mainWindowHandle,     // Parent window
	                           NULL,       // No menu.
	                           (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                           NULL  // Pointer not needed.
	                          );

	noiseApplyButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                                L"Apply",      // Button text
	                                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                                455,         // x position
	                                190,         // y position
	                                100,        // Button width
	                                25,        // Button height
	                                mainWindowHandle,     // Parent window
	                                NULL,       // No menu.
	                                (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                NULL  // Pointer not needed.
	                               );

	bilateralText = CreateWindow(L"STATIC",  // Predefined class; Unicode assumed
	                             L"Bilateral Filter",      // Button text
	                             WS_VISIBLE | WS_CHILD | SS_LEFT,  // Styles
	                             30,         // x position
	                             225,         // y position
	                             250,        // Button width
	                             20,        // Button height
	                             mainWindowHandle,     // Parent window
	                             NULL,       // No menu.
	                             (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                             NULL  // Pointer not needed.
	                            );

	bilateralButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                               L"Run",      // Button text
	                               WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                               350,         // x position
	                               220,         // y position
	                               100,        // Button width
	                               25,        // Button height
	                               mainWindowHandle,     // Parent window
	                               NULL,       // No menu.
	                               (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                               NULL  // Pointer not needed.
	                              );

	bilateralApplyButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                                    L"Apply",      // Button text
	                                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                                    455,         // x position
	                                    220,         // y position
	                                    100,        // Button width
	                                    25,        // Button height
	                                    mainWindowHandle,     // Parent window
	                                    NULL,       // No menu.
	                                    (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                    NULL  // Pointer not needed.
	                                   );

	smoothText = CreateWindow(L"STATIC",  // Predefined class; Unicode assumed
	                          L"Mean Smoothing",      // Button text
	                          WS_VISIBLE | WS_CHILD | SS_LEFT,  // Styles
	                          30,         // x position
	                          255,         // y position
	                          250,        // Button width
	                          20,        // Button height
	                          mainWindowHandle,     // Parent window
	                          NULL,       // No menu.
	                          (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                          NULL  // Pointer not needed.
	                         );

	smoothButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                            L"Run",      // Button text
	                            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                            350,         // x position
	                            250,         // y position
	                            100,        // Button width
	                            25,        // Button height
	                            mainWindowHandle,     // Parent window
	                            NULL,       // No menu.
	                            (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                            NULL  // Pointer not needed.
	                           );

	smoothApplyButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                                 L"Apply",      // Button text
	                                 WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                                 455,         // x position
	                                 250,         // y position
	                                 100,        // Button width
	                                 25,        // Button height
	                                 mainWindowHandle,     // Parent window
	                                 NULL,       // No menu.
	                                 (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                 NULL  // Pointer not needed.
	                                );

	whiteText = CreateWindow(L"STATIC",  // Predefined class; Unicode assumed
	                         L"White To Zero",      // Button text
	                         WS_VISIBLE | WS_CHILD | SS_LEFT,  // Styles
	                         30,         // x position
	                         285,         // y position
	                         250,        // Button width
	                         20,        // Button height
	                         mainWindowHandle,     // Parent window
	                         NULL,       // No menu.
	                         (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                         NULL  // Pointer not needed.
	                        );

	whiteButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                           L"Run",      // Button text
	                           WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                           350,         // x position
	                           280,         // y position
	                           100,        // Button width
	                           25,        // Button height
	                           mainWindowHandle,     // Parent window
	                           NULL,       // No menu.
	                           (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                           NULL  // Pointer not needed.
	                          );

	whiteApplyButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                                L"Apply",      // Button text
	                                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                                455,         // x position
	                                280,         // y position
	                                100,        // Button width
	                                25,        // Button height
	                                mainWindowHandle,     // Parent window
	                                NULL,       // No menu.
	                                (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                NULL  // Pointer not needed.
	                               );

	blackText = CreateWindow(L"STATIC",  // Predefined class; Unicode assumed
	                         L"Black To Zero",      // Button text
	                         WS_VISIBLE | WS_CHILD | SS_LEFT,  // Styles
	                         30,         // x position
	                         315,         // y position
	                         250,        // Button width
	                         20,        // Button height
	                         mainWindowHandle,     // Parent window
	                         NULL,       // No menu.
	                         (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                         NULL  // Pointer not needed.
	                        );

	blackButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                           L"Run",      // Button text
	                           WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                           350,         // x position
	                           310,         // y position
	                           100,        // Button width
	                           25,        // Button height
	                           mainWindowHandle,     // Parent window
	                           NULL,       // No menu.
	                           (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                           NULL  // Pointer not needed.
	                          );

	blackApplyButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                                L"Apply",      // Button text
	                                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                                455,         // x position
	                                310,         // y position
	                                100,        // Button width
	                                25,        // Button height
	                                mainWindowHandle,     // Parent window
	                                NULL,       // No menu.
	                                (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                NULL  // Pointer not needed.
	                               );

	/*
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
	//*/

	resetButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                           L"Reset",      // Button text
	                           WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                           400,         // x position
	                           430,         // y position
	                           100,        // Button width
	                           25,        // Button height
	                           mainWindowHandle,     // Parent window
	                           NULL,       // No menu.
	                           (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                           NULL  // Pointer not needed.
	                          );

	saveButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                          L"Save image",      // Button text
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

	showCurrentButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                                 L"Show current corrected image",      // Button text
	                                 WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                                 400,         // x position
	                                 460,         // y position
	                                 250,        // Button width
	                                 25,        // Button height
	                                 mainWindowHandle,     // Parent window
	                                 NULL,       // No menu.
	                                 (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                 NULL  // Pointer not needed.
							        );

	runCombinedButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                                 L"Run all in parallel",      // Button text
	                                 WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                                 400,         // x position
	                                 490,         // y position
	                                 145,        // Button width
	                                 25,        // Button height
	                                 mainWindowHandle,     // Parent window
	                                 NULL,       // No menu.
	                                 (HINSTANCE)GetWindowLong(mainWindowHandle, GWL_HINSTANCE),
	                                 NULL  // Pointer not needed.
							        );

	applyCombinedButton = CreateWindow(L"BUTTON",  // Predefined class; Unicode assumed
	                                   L"Apply",      // Button text
	                                   WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles
	                                   550,         // x position
	                                   490,         // y position
	                                   100,        // Button width
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
				correctedImage = image;
				SetWindowTextA(mainWindowHandle, name);
				cv::namedWindow("Original Image", 1);
				cv::imshow("Original Image", image);
			}
		}
		/*
		else if((HWND)lParam == propContrastButton)
		{
			if(imageLoaded)
			{
				correctedImage = propContrast(correctedImage);
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
		//*/
		else if((HWND)lParam == propContrastButton)
		{
			if(imageLoaded)
			{
				cv::namedWindow("Contrast Stretching (Proportional)", cv::WINDOW_AUTOSIZE);
				cv::createTrackbar("% modified", "Contrast Stretching (Proportional)", &propContrastValue, 100, propContrastCallback);

				cv::Mat bw;
				cv::cvtColor( correctedImage, bw, cv::COLOR_BGR2GRAY ); // create greyscale image

				// Traverse through image, find minimum and maximum
				for( int y = 0; y < bw.rows; y++ ) { 
					for( int x = 0; x < bw.cols; x++ ) { 
						if (propContrastMinval>cv::saturate_cast<uchar>(bw.at<cv::Vec3b>(y,x)[0]))
							propContrastMinval = cv::saturate_cast<uchar>(bw.at<cv::Vec3b>(y,x)[0]);
						if (propContrastMaxval<cv::saturate_cast<uchar>(bw.at<cv::Vec3b>(y,x)[0]))
							propContrastMaxval = cv::saturate_cast<uchar>(bw.at<cv::Vec3b>(y,x)[0]);
					}
				}

				propContrastCallback(0, 0);
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == propContrastApplyButton)
		{
			if(imageLoaded)
			{
				if(propContrastLoaded)
				{
					correctedImage = propContrastTempImage;
					cv::destroyWindow("Contrast Stretching (Proportional)");
					propContrastLoaded = false;
					correctedImageIsNull = false;
					cv::namedWindow("Corrected Image", 1);
					cv::imshow("Corrected Image", correctedImage);
				}
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
				cv::namedWindow("Contrast Stretching (Non-Proportional)", cv::WINDOW_AUTOSIZE);
				cv::createTrackbar("% modified", "Contrast Stretching (Non-Proportional)", &nonPropContrastValue, 100, nonPropContrastCallback);

				// Traverse through image, find minimum and maximum
				for( int y = 0; y < correctedImage.rows; y++ ) { 
					for( int x = 0; x < correctedImage.cols; x++ ) { 
						for( int c = 0; c < 3; c++ ) {
							if (nonPropContrastMinval[c]>cv::saturate_cast<uchar>(correctedImage.at<cv::Vec3b>(y,x)[c]))
								nonPropContrastMinval[c] = cv::saturate_cast<uchar>(correctedImage.at<cv::Vec3b>(y,x)[c]);
							if (nonPropContrastMaxval[c]<cv::saturate_cast<uchar>(correctedImage.at<cv::Vec3b>(y,x)[c]))
								nonPropContrastMaxval[c] = cv::saturate_cast<uchar>(correctedImage.at<cv::Vec3b>(y,x)[c]);
						}
					}
				}

				nonPropContrastCallback(0, 0);
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == nonPropContrastApplyButton)
		{
			if(imageLoaded)
			{
				if(nonPropContrastLoaded)
				{
					correctedImage = nonPropContrastTempImage;
					cv::destroyWindow("Contrast Stretching (Non-Proportional)");
					nonPropContrastLoaded = false;
					correctedImageIsNull = false;
					cv::namedWindow("Corrected Image", 1);
					cv::imshow("Corrected Image", correctedImage);
				}
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == blurButton)
		{
			if(imageLoaded)
			{
				cv::namedWindow("Gaussian Blur", cv::WINDOW_AUTOSIZE);
				cv::createTrackbar("Blur", "Gaussian Blur", &blurValue, 30, blurCallback);
				blurCallback(0, 0);
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == blurApplyButton)
		{
			if(imageLoaded)
			{
				if(blurLoaded)
				{
					correctedImage = blurTempImage;
					cv::destroyWindow("Gaussian Blur");
					blurLoaded = false;
					correctedImageIsNull = false;
					cv::namedWindow("Corrected Image", 1);
					cv::imshow("Corrected Image", correctedImage);
				}
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == noiseButton)
		{
			if(imageLoaded)
			{
				cv::namedWindow("Gaussian Noise", cv::WINDOW_AUTOSIZE);
				cv::createTrackbar("% Noise", "Gaussian Noise", &noiseValue, 100, noiseCallback);
				noiseCallback(0, 0);
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == noiseApplyButton)
		{
			if(imageLoaded)
			{
				if(noiseLoaded)
				{
					correctedImage = noiseTempImage;
					cv::destroyWindow("Gaussian Noise");
					noiseLoaded = false;
					correctedImageIsNull = false;
					cv::namedWindow("Corrected Image", 1);
					cv::imshow("Corrected Image", correctedImage);
				}
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == bilateralButton)
		{
			if(imageLoaded)
			{
				cv::namedWindow("Bilateral Filter", cv::WINDOW_AUTOSIZE);
				cv::createTrackbar("Bilateral", "Bilateral Filter", &bilateralValue, 20, bilateralCallback);
				bilateralCallback(0, 0);
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == bilateralApplyButton)
		{
			if(imageLoaded)
			{
				if(bilateralLoaded)
				{
					correctedImage = bilateralTempImage;
					cv::destroyWindow("Bilateral Filter");
					bilateralLoaded = false;
					correctedImageIsNull = false;
					cv::namedWindow("Corrected Image", 1);
					cv::imshow("Corrected Image", correctedImage);
				}
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == smoothButton)
		{
			if(imageLoaded)
			{
				cv::namedWindow("Mean Smoothing", cv::WINDOW_AUTOSIZE);
				cv::createTrackbar("% original", "Mean Smoothing", &smoothValue, 100, smoothCallback);
				smoothCallback(0, 0);
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == smoothApplyButton)
		{
			if(imageLoaded)
			{
				if(smoothLoaded)
				{
					correctedImage = smoothTempImage;
					cv::destroyWindow("Mean Smoothing");
					smoothLoaded = false;
					correctedImageIsNull = false;
					cv::namedWindow("Corrected Image", 1);
					cv::imshow("Corrected Image", correctedImage);
				}
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == whiteButton)
		{
			if(imageLoaded)
			{
				cv::namedWindow("White To Zero", cv::WINDOW_AUTOSIZE);
				cv::createTrackbar("White->0", "White To Zero", &whiteValue, 255, whiteCallback);
				whiteCallback(0, 0);
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == whiteApplyButton)
		{
			if(imageLoaded)
			{
				if(whiteLoaded)
				{
					correctedImage = whiteTempImage;
					cv::destroyWindow("White To Zero");
					whiteLoaded = false;
					correctedImageIsNull = false;
					cv::namedWindow("Corrected Image", 1);
					cv::imshow("Corrected Image", correctedImage);
				}
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == blackButton)
		{
			if(imageLoaded)
			{
				cv::namedWindow("Black To Zero", cv::WINDOW_AUTOSIZE);
				cv::createTrackbar("Black->0", "Black To Zero", &blackValue, 255, blackCallback);
				blackCallback(0, 0);
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == blackApplyButton)
		{
			if(imageLoaded)
			{
				if(blackLoaded)
				{
					correctedImage = blackTempImage;
					cv::destroyWindow("Black To Zero");
					blackLoaded = false;
					correctedImageIsNull = false;
					cv::namedWindow("Corrected Image", 1);
					cv::imshow("Corrected Image", correctedImage);
				}
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
				correctedImage = resize(correctedImage);
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
				//correctedImage = image;//nonPropContrast(image);

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
		else if((HWND)lParam == resetButton)
		{
			if(!correctedImageIsNull)
			{
				correctedImage = image;
				cv::imshow("Corrected Image", correctedImage);
			}
		}
		else if((HWND)lParam == showCurrentButton)
		{
			if(imageLoaded)
			{
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
				//if(saveImage())
				{
					try {
						cv::imwrite("Corrected Image.png", correctedImage);
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
		else if((HWND)lParam == runCombinedButton)
		{
			if(imageLoaded)
			{
				combinedLoaded = true;
				cv::namedWindow("Combined Editing", cv::WINDOW_AUTOSIZE);
				cv::createTrackbar("prop %", "Combined Editing", &propContrastValue, 100, propContrastCallback);
				cv::createTrackbar("nonprop %", "Combined Editing", &nonPropContrastValue, 100, nonPropContrastCallback);
				cv::createTrackbar("Blur", "Combined Editing", &blurValue, 30, blurCallback);
				cv::createTrackbar("% Noise", "Combined Editing", &noiseValue, 100, noiseCallback);
				cv::createTrackbar("Bilateral", "Combined Editing", &bilateralValue, 20, bilateralCallback);
				cv::createTrackbar("smooth %", "Combined Editing", &smoothValue, 100, smoothCallback);
				cv::createTrackbar("White->0", "Combined Editing", &whiteValue, 255, whiteCallback);
				cv::createTrackbar("Black->0", "Combined Editing", &blackValue, 255, blackCallback);

				// Traverse through image, find minimum and maximum
				for( int y = 0; y < correctedImage.rows; y++ ) { 
					for( int x = 0; x < correctedImage.cols; x++ ) { 
						for( int c = 0; c < 3; c++ ) {
							if (nonPropContrastMinval[c]>cv::saturate_cast<uchar>(correctedImage.at<cv::Vec3b>(y,x)[c]))
								nonPropContrastMinval[c] = cv::saturate_cast<uchar>(correctedImage.at<cv::Vec3b>(y,x)[c]);
							if (nonPropContrastMaxval[c]<cv::saturate_cast<uchar>(correctedImage.at<cv::Vec3b>(y,x)[c]))
								nonPropContrastMaxval[c] = cv::saturate_cast<uchar>(correctedImage.at<cv::Vec3b>(y,x)[c]);
						}
					}
				}

				cv::Mat bw;
				cv::cvtColor( correctedImage, bw, cv::COLOR_BGR2GRAY ); // create greyscale image

				// Traverse through image, find minimum and maximum
				for( int y = 0; y < bw.rows; y++ ) { 
					for( int x = 0; x < bw.cols; x++ ) { 
						if (propContrastMinval>cv::saturate_cast<uchar>(bw.at<cv::Vec3b>(y,x)[0]))
							propContrastMinval = cv::saturate_cast<uchar>(bw.at<cv::Vec3b>(y,x)[0]);
						if (propContrastMaxval<cv::saturate_cast<uchar>(bw.at<cv::Vec3b>(y,x)[0]))
							propContrastMaxval = cv::saturate_cast<uchar>(bw.at<cv::Vec3b>(y,x)[0]);
					}
				}

				//noiseCallback(0, 0);
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
			}
		}
		else if((HWND)lParam == applyCombinedButton)
		{
			if(imageLoaded)
			{
				if(combinedLoaded)
				{
					//*
					//correctedImage = blackTempImage;
					cv::destroyWindow("Combined Editing");
					combinedLoaded = false;
					correctedImageIsNull = false;
					cv::namedWindow("Corrected Image", 1);
					cv::imshow("Corrected Image", correctedImage);
					//*/
				}
			}
			else
			{
				MessageBox(0, L"Error:\nSelect an image.", L"Run Error", MB_OK | MB_ICONERROR);
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
						new_image2.at<cv::Vec3b>(y,x)[c] += cv::saturate_cast<uchar>( (new_image.at<cv::Vec3b>(y-2+a,x-2+b)[c])/25*0.25 );
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

	
	// Traverse through image, find minimum and maximum
	for( int y = 0; y < image.rows; y++ ) { 
		for( int x = 0; x < image.cols; x++ ) { 
			for( int c = 0; c < 3; c++ ) {
				if (minval[c]>cv::saturate_cast<uchar>(image.at<cv::Vec3b>(y,x)[c]))
					minval[c] = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(y,x)[c]);
				if (maxval[c]<cv::saturate_cast<uchar>(image.at<cv::Vec3b>(y,x)[c]))
					maxval[c] = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(y,x)[c]);
			}
		}
	}

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
						new_image2.at<cv::Vec3b>(y,x)[c] += cv::saturate_cast<uchar>( (new_image.at<cv::Vec3b>(y-2+a,x-2+b)[c])/25*0.25 );
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

void blurCallback(int, void*)
{
	cv::GaussianBlur(correctedImage, blurTempImage, cv::Size( blurValue*2+1, blurValue*2+1 ), 0, 0);
	if(combinedLoaded)
	{
		correctedImage = blurTempImage;
		imshow("Combined Editing", correctedImage);
	}
	else
	{
		cv::imshow( "Gaussian Blur", blurTempImage );
		blurLoaded = true;
	}
}

void noiseCallback(int, void*)
{
	cv::threshold(correctedImage, noiseTempImage, 255, 0, 4 );
	noiseTempImage = correctedImage.clone();
	cv::Mat noisyI;
	noisyI.create (noiseTempImage.rows,noiseTempImage.cols,CV_32FC(1));
	noisyI.setTo (cv::Scalar::all (0));

	cv::vector <cv::Mat>  _channel;
	noiseTempImage.convertTo (noiseTempImage,CV_32FC(3),1.0,0);
	cv::split(noiseTempImage,_channel);
	for(int i=0;i<noiseTempImage.channels ();i++)
	{
		cv::randn(noisyI, cv::Scalar::all(0), cv::Scalar::all((double)noiseValue)/2);
		add(_channel[i],noisyI,_channel[i]);
	}

	cv::merge (_channel,noiseTempImage);
	noiseTempImage.convertTo (noiseTempImage,CV_8UC(3),1.0,0);

	if(combinedLoaded)
	{
		correctedImage = noiseTempImage;
		imshow("Combined Editing", correctedImage);
	}
	else
	{
		cv::imshow( "Gaussian Noise", noiseTempImage );
		noiseLoaded = true;
	}
}

void bilateralCallback(int, void*)
{
	cv::bilateralFilter( correctedImage, bilateralTempImage, bilateralValue*2+1, bilateralValue*4+1, bilateralValue+1 );
	
	if(combinedLoaded)
	{
		correctedImage = bilateralTempImage;
		imshow("Combined Editing", correctedImage);
	}
	else
	{
		cv::imshow( "Bilateral Filter", bilateralTempImage );
		bilateralLoaded = true;
	}
}

void smoothCallback(int, void*)
{
	cv::threshold( correctedImage, smoothTempImage, 255, 0, 4 );
	for( int y = 2; y < correctedImage.rows-2; y++ ) { 
		for( int x = 2; x < correctedImage.cols-2; x++ ) { 
			for( int c = 0; c < 3; c++ ) {
				// 75% of image contrast from original pixel
				smoothTempImage.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( (correctedImage.at<cv::Vec3b>(y,x)[c])*(float)smoothValue/100 );
				// 25% of image contrast mean of surrounding 25 pixels (5x5)
				for ( int a = 0; a < 5; a++ ) {
					for ( int b = 0; b < 5; b++ ) {
						smoothTempImage.at<cv::Vec3b>(y,x)[c] += cv::saturate_cast<uchar>( (correctedImage.at<cv::Vec3b>(y-2+a,x-2+b)[c])/25*(1-(float)smoothValue/100) );
					}
				}
			}
		}
	}
	
	if(combinedLoaded)
	{
		correctedImage = smoothTempImage;
		imshow("Combined Editing", correctedImage);
	}
	else
	{
		cv::imshow( "Mean Smoothing", smoothTempImage );
		smoothLoaded = true;
	}
}

void whiteCallback(int, void*)
{
	cv::threshold( correctedImage, whiteTempImage, whiteValue, 0, 2 );
	
	if(combinedLoaded)
	{
		correctedImage = whiteTempImage;
		imshow("Combined Editing", correctedImage);
	}
	else
	{
		cv::imshow( "White To Zero", whiteTempImage );
		whiteLoaded = true;
	}
}

void blackCallback(int, void*)
{
	cv::threshold( correctedImage, blackTempImage, blackValue, 0, 3 );

	if(combinedLoaded)
	{
		correctedImage = blackTempImage;
		imshow("Combined Editing", correctedImage);
	}
	else
	{
		cv::imshow( "Black To Zero", blackTempImage );
		blackLoaded = true;
	}
}

void propContrastCallback(int, void*)
{
	cv::threshold( correctedImage, propContrastTempImage, 255, 0, 4 );
	for( int y = 0; y < correctedImage.rows; y++ ) { 
		for( int x = 0; x < correctedImage.cols; x++ ) { 
			for( int c = 0; c < 3; c++ ) {
		// Create new_image by shifting minimum contrast down to zero and multiplying to stretch maximum contrast to 255
				double shift = -(propContrastMinval)*((double)propContrastValue/100);
				double scale = 1+((255/(propContrastMaxval-propContrastMinval))-1)*((double)propContrastValue/100);
				
				propContrastTempImage.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(scale*( correctedImage.at<cv::Vec3b>(y,x)[c] + shift) );
			}
		}
	}
	
	if(combinedLoaded)
	{
		correctedImage = propContrastTempImage;
		imshow("Combined Editing", correctedImage);
	}
	else
	{
		cv::imshow( "Contrast Stretching (Proportional)", propContrastTempImage );
		propContrastLoaded = true;
	}
}

void nonPropContrastCallback(int, void*)
{
	cv::threshold( correctedImage, nonPropContrastTempImage, 255, 0, 4 );
	for( int y = 0; y < correctedImage.rows; y++ ) { 
		for( int x = 0; x < correctedImage.cols; x++ ) { 
			for( int c = 0; c < 3; c++ ) {
		// Create new_image by shifting minimum contrast down to zero and multiplying to stretch maximum contrast to 255
				double shift = -(nonPropContrastMinval[c])*((double)nonPropContrastValue/100);
				double scale = 1+((255/(nonPropContrastMaxval[c]-nonPropContrastMinval[c]))-1)*((double)nonPropContrastValue/100);
				
				nonPropContrastTempImage.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(scale*( correctedImage.at<cv::Vec3b>(y,x)[c] + shift) );
			}
		}
	}
	
	if(combinedLoaded)
	{
		correctedImage = nonPropContrastTempImage;
		imshow("Combined Editing", correctedImage);
	}
	else
	{
		cv::imshow( "Contrast Stretching (Non-Proportional)", nonPropContrastTempImage );
		nonPropContrastLoaded = true;
	}
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
