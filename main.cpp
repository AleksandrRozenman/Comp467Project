/*
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

//using namespace cv;
//using namespace std;

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		std::cout << " Usage: display_image ImageToLoadAndDisplay" << std::endl;
		return -1;
	}

	cv::Mat image;
	image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

	if(!image.data)                              // Check for invalid input
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	cv::namedWindow("Display window", CV_WINDOW_AUTOSIZE);// Create a window for display.
	cv::imshow("Display window", image);                   // Show our image inside it.

	cv::waitKey(0);                                          // Wait for a keystroke in the window
	return 0;
}
//*/

/*
//===================================================================
// Win32Basic.cpp by Frank Luna (C) 2008 All Rights Reserved.
//
// Illustrates the minimal amount of the Win32 code needed for
// Direct3D programming.
//===================================================================

// Include the windows header file; this has all the Win32 API
// structures, types, and function declarations we need to program
// Windows.
#include <Windows.h>

// The main window handle; this is used to identify a
// created window.
HWND ghMainWnd = 0;

// Wraps the code necessary to initialize a Windows
// application. Function returns true if initialization
//was successful, otherwise it returns false.
bool InitWindowsApp(HINSTANCE instanceHandle, int show);

// Wraps the message loop code.
int Run();

// The window procedure handles events our window receives.
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Windows equivalent to main()
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        PSTR pCmdLine, int nShowCmd)
{
	// First call our wrapper function (InitWindowsApp) to create
	// and initialize the main application window, passing in the
	// hInstance and nShowCmd values as arguments.
	if(!InitWindowsApp(hInstance, nShowCmd))
		return 0;

	// Once our application has been created and initialized we
	// enter the message loop. We stay in the message loop until
	// a WM_QUIT message is received, indicating the application
	// should be terminated.
	return Run();
}

bool InitWindowsApp(HINSTANCE instanceHandle, int show)
{
	// The first task to creating a window is to describe some of its
	// characteristics by filling out a WNDCLASS structure.
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = instanceHandle;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"BasicWndClass";

	// Next, we register this WNDCLASS instance with Windows so
	// that we can create a window based on it.
	if(!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		return false;
	}

	// With our WNDCLASS instance registered, we can create a
	// window with the CreateWindow function. This function
	// returns a handle to the window it creates (an HWND).
	// If the creation failed, the handle will have the value
	// of zero. A window handle is a way to refer to the window,
	// which is internally managed by Windows. Many of the Win32 API
	// functions that operate on windows require an HWND as that
	// they know what window to act on.

	ghMainWnd = CreateWindow(
		L"BasicWndClass",    // Registered WNDCLASS instance to use.
		L"Win32Basic",       // window title
		WS_OVERLAPPEDWINDOW, // style flags
		CW_USEDEFAULT,       // x-coordinate
		CW_USEDEFAULT,       // y-coordinate
		CW_USEDEFAULT,       // width
		CW_USEDEFAULT,       // height
		0,                   // parent window
		0,                   // menu handle
		instanceHandle,      // app instance
		0);                  // extra creation parameters

	if(ghMainWnd == 0)
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		return false;
	}

	// Even though we just created a window, it is not initially
	// shown. Therefore, the final step is to show and update the
	// window we just created, which can be done with the following
	// two function calls. Observe that we pass the handle to the
	// window we want to show and update so that these functions know
	// which window to show and update.
	ShowWindow(ghMainWnd, show);
	UpdateWindow(ghMainWnd);

	return true;
}

int Run()
{
	MSG msg = {0};

	// Loop until we get a WM_QUIT message. The function
	// GetMessage will only return 0 (false) when a WM_QUIT message
	// is received, which effectively exits the loop. The function
	// returns -1 if there is an error. Also, note that GetMessage
	// puts the application thread to sleep until there is a
	// message.
	BOOL bRet = 1;
	while((bRet = GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if(bRet == -1)
		{
			MessageBox(0, L"GetMessage FAILED", L"Error", MB_OK);
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Handle some specific messages. Note that we we handle a
	// message, we should return 0.
	switch(msg)
	{
		// In the case the left mouse button was pressed,
		// then display a message box.
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello, World", L"Hello", MB_OK);
		return 0;

		// In the case the escape key was pressed, then
		// destroy the main application window.
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
			DestroyWindow(ghMainWnd);
		return 0;

		// In the case of a destroy message, then send a
		// quit message, which will terminate the message loop.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		MessageBox(0, L"Window created", L"Success!", MB_OK);
		return 0;
	}

	// Forward any other messages we did not handle above to the
	// default window procedure. Note that our window procedure
	// must return the return value of DefWindowProc.
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//*/

// C/C++ headers
#include <string>

// Windows headers
#include <Windows.h>
//#include <WindowsX.h> // Not sure if needed.

// OpenCV headers
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// Comp467Project headers


// Window descriptors.
HWND mainWindowHandle = 0;
HINSTANCE hInstance = 0;
std::wstring mainWindowCaption = L"Comp 467 Project: Group A2";

// Default window dimensions.
int windowX = CW_USEDEFAULT, windowY = CW_USEDEFAULT;
int windowWidth = 800, windowHeight = 600;

// Image
cv::Mat image;

int initMainWindow();
LRESULT CALLBACK mainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int run();

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR lpCmdLine, int nCmdShow)
{
	int retVal = 0;

	image = cv::imread("opencv-logo.png", CV_LOAD_IMAGE_COLOR);   // Read the file

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
	int width  = r.right - r.left;
	int height = r.bottom - r.top;

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

	// Display window w/ image when the main window is left-clicked on.
	case WM_LBUTTONDOWN:
		if(!image.data)                              // Check for invalid input
		{
			MessageBox(0, L"ERROR", L"Could not open or find the image", MB_OK | MB_ICONERROR);
			return -1;
		}

		cv::namedWindow("Display window", CV_WINDOW_AUTOSIZE);// Create a window for display.
		cv::imshow("Display window", image);
		return 0;

	case WM_MENUCHAR:
        // Don't beep when we alt-enter.
        return MAKELRESULT(0, MNC_CLOSE);

	// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200; 
		return 0;
	}

	// Forward any other messages not handled to default window procedure;
	// return the return value of DefWindowProc.
	return DefWindowProc(hwnd, msg, wParam, lParam);
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
