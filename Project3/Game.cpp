#include "Game.h"

Game::Game()
{
	_input = new Input;
	_graphics = NULL;
	_paused = false;
	_initialized = false;
}

Game::~Game()
{
	deleteAll();
	ShowCursor(true);
}

LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	if (_initialized)
	{
		switch (msg)
		{
			//xử lý sự kiện thoát
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			//xử lý sự kiện ấn phím
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			_input->keyDown(w);
			return 0;
			//xử lý sự kiện nhả phím
		case WM_KEYUP:
		case WM_SYSKEYUP:
			_input->keyUp(w);
			return 0;
			//nhập kí tự
		case WM_CHAR:
			_input->keyIn(w);
			return 0;
			//xử lý chuột di chuyển
		case WM_MOUSEMOVE:
			_input->mouseIn(l);
			return 0;
			//
		case WM_INPUT:
			_input->mouseRawIn(l);
			return 0;
			//Các sự kiến ấn và nhả chuột
		case WM_LBUTTONDOWN:
			_input->setMouseLeftButton(true);
			_input->mouseIn(l);
			return 0;
		case WM_LBUTTONUP:
			_input->setMouseLeftButton(false);
			_input->mouseIn(l);
			return 0;
		case WM_RBUTTONDOWN:
			_input->setMouseRightButton(true);
			_input->mouseIn(l);
			return 0;
		case WM_RBUTTONUP:
			_input->setMouseRightButton(false);
			_input->mouseIn(l);
			return 0;
		case WM_MBUTTONDOWN:
			_input->setMouseMiddleButton(true);
			_input->mouseIn(l);
			return 0;
		case WM_MBUTTONUP:
			_input->setMouseMiddleButton(false);
			_input->mouseIn(l);
			return 0;
			//sự kiện x button
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
			_input->setMouseXButton(w);
			_input->mouseIn(l);
			return 0;
			//Thay đổi thiết bị
		case WM_DEVICECHANGE:
			_input->checkControllers();
			return 0;
		default:
			break;
		}
	}
	return DefWindowProc(hwnd, msg, w, l);
}

void Game::initialize(HWND hwnd)
{
	_hwnd = hwnd;

	_graphics = new Graphics;
	_graphics->initialize(_hwnd, WIDTH, HEIGHT, FULLSCREEN);

	_input->initialize(_hwnd, false);

	if (QueryPerformanceFrequency(&_timeFreq) == false)
	{
		throw (Error(ErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));
	}
	QueryPerformanceCounter(&_timeStart);
	_initialized = true;
}

void Game::releaseAll()
{
}

void Game::resetAll()
{
}

void Game::deleteAll()
{
	releaseAll();
	safeDelete(_graphics);
	safeDelete(_input);
	_initialized = false;
}

void Game::renderGame()
{
	if (SUCCEEDED(_graphics->beginScene()))
	{
		render();
		_graphics->endScene();
	}

	handleLostGraphicsDevice();
	_graphics->showBackbuffer();
}

void Game::handleLostGraphicsDevice()
{
	_result = _graphics->getDeviceState();
	if (FAILED(_result))
	{
		if (_result == D3DERR_DEVICELOST)
		{
			Sleep(100);
			return;
		}
		else
			if (_result == D3DERR_DEVICENOTRESET)
			{
				releaseAll();
				_graphics->reset();
				if (FAILED(_result))
				{
					return;
				}
				resetAll();
			}
			else
				return;
	}
}

Graphics * Game::getGraphics()
{
	return _graphics;
}

Input * Game::getInput()
{
	return _input;
}

void Game::exitGame()
{
	PostMessage(_hwnd, WM_DESTROY, 0, 0);
}

void Game::run(HWND hwnd)
{
	if (_graphics == NULL)
		return;

	QueryPerformanceCounter(&_timeEnd);
	_frameTime = (float)(_timeEnd.QuadPart - _timeStart.QuadPart) /
		(float)_timeFreq.QuadPart;

	if (_frameTime < MIN_FRAME_TIME)
	{
		_sleepTime = (DWORD)((MIN_FRAME_TIME - _frameTime) * 1000);
		timeBeginPeriod(1);
		Sleep(_sleepTime);
		timeEndPeriod(1);
		return;
	}
	
	if (_frameTime > 0.0)
		_fps = (_fps*0.99f) + (0.01f / _frameTime);
	if (_frameTime > MAX_FRAME_TIME)
		_frameTime = MAX_FRAME_TIME;
	_timeStart = _timeEnd;

	if (!_paused)
	{
		update();
		ai();
		collisions();
		_input->vibrateControllers(_frameTime);
	}
	renderGame();
	_input->readController();
	_input->clear(InputNS::KEYS_PRESSED);
}
