#define WIN32_LEAN_AND_MEAN
#include <windows.h>
//Tham khảo các kiểu dữ liệu trong bài tại đây nếu mọi người hk hiểu nha
//https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751(v=vs.85).aspx
//handle instance. quản lý 1 instance. còn instance quản lý 1 cửa sổ
HINSTANCE hinst;

//tên class (nếu thọ không nhâm mỗi cửa sổ có 1 class name. để window có thể gọi nó)
const char CLASS_NAME[] = "WinMain";

//tên của cửa sổ mà ta khởi tạo.
const char APP_TITLE[] = "Hello World";

//kích thước cửa sổ
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

//HWND handle window: tức truyền vào cửa sổ của chúng ta. nó đc quản lý bới biến hWnd

//UINT unsigned int: msg thực chất là 1 số nguyên không dấu. chứa tín hiệu điều khiển của hệ thống
//WPARAM là cái tên khác của UINT_PTR bản chất là 1 số nguyên không dấu
//LPARAM là cái tên khác của LONG_PTR bản chất là long
//WPARAM và LPARAM bổ sung thông tin cho msg

//LRESULT là 1 số long. trả về kết quả gọi hàm là 1 mã kiểu long
//mọi người nhớ để chúng ta khi gọi hàm có thể ép kiểu cho phù hợp

//cái này thọ hk hiểu cụ thể lắm. đại loại nó sẽ xử lý các thông điệp phát sinh trong quá trình chạy của chúng ta
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//kiểm tra tín hiệu của hệ thống
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//Như cái tên. create main window. khởi tạo của sổ chính của chúng ta.
//kết quả trả về là bool tương đương nếu thành công thì là true và ngược lại là false
//hInstance đã giải thích ở trên
//nCmdShow là tham số trạng thái của cửa sổ.
//mặc định là 1. tức SW_SHOWNORMAL. trạng thái bình thường của cửa sổ
//tham khảo thêm tham số tại đây https://msdn.microsoft.com/en-us/library/windows/desktop/ms633548(v=vs.85).aspx
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	//window class extend. nó là phần mở rộng của window class. chứa các thông tin của cửa sổ
	//https://msdn.microsoft.com/en-us/library/windows/desktop/ms633577(v=vs.85).aspx
	WNDCLASSEX wcx;

	//handle window: quản lý 1 cửa sổ
	HWND hwnd;

	//Khởi tạo các thông tin của sổ
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;

	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);

	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = NULL;

	//Đăng kí 1 cửa sổ window với hệ thống nếu không được thì thoát
	if (RegisterClassEx(&wcx) == 0)
		return false;


	hwnd = CreateWindow(
		CLASS_NAME,
		APP_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		(HWND)NULL,
		(HMENU)NULL,
		hInstance,
		(LPVOID)NULL);

	if (!hwnd)
		return false;
	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);
	return true;
}





//Giống như trước giờ trong C thì ở đây WinMain là hàm được gọi thay cho main
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	MSG msg;
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;

	int done = 0;
	while (!done)
	{
		//msg được gửi vào hàm PeekMessage để lấy về thông điệp hệ thống
		//kết quả trả về là bool tức đúng hay sai. nếu đúng thì kiểm tra thông điệp còn không thì làm lại
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//nếu mã thông điệp là WM_QUIT thì thoát
			if (msg.message == WM_QUIT)
				done = 1;

			//Dịch và gửi thông điệp
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}