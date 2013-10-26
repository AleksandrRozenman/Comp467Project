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
