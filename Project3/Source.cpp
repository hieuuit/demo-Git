

#define _CTRDBG_MAP_ALLOC
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <time.h>
#include <crtdbg.h>
#include "Spaceship.h"

SpaceShip *spaceship;

HINSTANCE hInsta;

LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	return spaceship->messageHandler(hwnd, msg, w, l);
}
bool CreateMainWindow(HWND &hwnd, HINSTANCE hInst, int n)
{
	COLORREF c = 10;
	WNDCLASSEX wcx;
	wcx.cbClsExtra = 0;
	wcx.cbSize = sizeof(wcx);
	wcx.cbWndExtra = 0;
	wcx.hbrBackground = CreateSolidBrush(RGB(255,255, 0));
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hIcon = 0;
	wcx.hIconSm = 0;
	wcx.hInstance = hInst;
	wcx.lpfnWndProc = WinProc;
	//
	wcx.lpszClassName = CLASS_NAME;
	wcx.lpszMenuName = 0;
	wcx.style = 0;

	if (RegisterClassEx(&wcx) == 0)
		return false;


	hwnd = CreateWindow(
		CLASS_NAME,
		APP_TITLE,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		WIDTH,
		HEIGHT,
		0,
		0,
		hInst,
		0
	);

	if (hwnd == NULL)
		return false;

	ShowWindow(hwnd, n);

	UpdateWindow(hwnd);
	return true;
}
int WINAPI WinMain(HINSTANCE h, HINSTANCE p, LPSTR l, int nCmdShow)
{
	MSG msg;
	HWND hwnd = NULL;
	srand(time(0));
	spaceship = new SpaceShip;
	if (!CreateMainWindow(hwnd, h, nCmdShow))
	{
		return 0;
	}
	try
	{
		spaceship->initialize(hwnd);
		int done = 0;
		while (!done)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					done = 1;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				spaceship->run(hwnd);
		}

		SAFE_DELETE(spaceship);
		return msg.wParam;
	}
	catch (const Error& e)
	{
		spaceship->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, e.getMsg(), "ERROR", MB_OK);
	}
	catch (...)
	{
		spaceship->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, "Unknown Error", "ERROR", MB_OK);
	}

	SAFE_DELETE(spaceship);
	return 1;
}