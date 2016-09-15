#include <d3d9.h>
#include <Windows.h>

HINSTANCE _hinst; //nam giu cua so
HWND _hwnd;

LRESULT WINAPI WinProc(HWND _hwnd, UINT msg, WPARAM w, LPARAM l) //Ham dieu khien
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return DefWindowProc(_hwnd, msg, w, l);
}

bool CreateMainWinDown(HINSTANCE hinst, int cmd)
{
	WNDCLASSEX wcx;
	wcx.cbClsExtra = 0;
	wcx.cbSize = sizeof(wcx);
	wcx.cbWndExtra = 0;
	wcx.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wcx.hCursor = LoadCursor(0, IDC_ARROW);
	wcx.hIcon = 0;
	wcx.hIconSm = 0;
	wcx.hInstance = hinst;
	wcx.lpfnWndProc = WinProc;
	wcx.lpszClassName = "MinhHieu";
	wcx.lpszMenuName = "MENU";
	wcx.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wcx))
		return false;

	_hwnd = CreateWindow
		(
			"MinhHieu",
			"ABC",
			WS_OVERLAPPEDWINDOW,
			0,
			0,
			640,
			480,
			0,
			0,
			hinst,
			0
			);

	if (!_hwnd)
		return 0;

	ShowWindow(_hwnd, cmd);
	UpdateWindow(_hwnd);

	return true;
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE p, LPSTR lp, int cmd)
{
	MSG msg;
	if (!CreateMainWinDown(_hinst, 1))
		return -1;

	while(1)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
			
	}
	return 1;
}