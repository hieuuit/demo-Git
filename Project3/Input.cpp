#include "Input.h"

Input::Input()
{
	//chuyen trang thai tat ca cac nut ve false
	for (int i = 0; i < InputNS::KEYS_ARRAY_LEN; i++)
	{
		_keyDown[i] = false;
		_keyPressed[i] = false;
	}

	//chua nhap gi nen newLine la true, charIn = null, textIn la null
	_newLine = true;
	_textIn.clear();
	_charIn = 0;

	//Toa do chuot bang 0
	_mouseRawX = _mouseRawY = _mouseX = _mouseY = 0;

	//trang thai an chuot la false
	_mouseMiddleButton = _mouseX1Button = _mouseX2Button = _mouseLeftButton = _mouseRightButton = false;

	//cai dat thoi gian rung cua cac thiet bi bang 0
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		_controllerState[i].vibrateTimeLeft = _controllerState[i].vibrateTimeRight = 0;
	}

}

Input::~Input()
{
}

void Input::initialize(HWND hwnd, bool capture)
{
	try
	{
		_mouseCaptured = capture;
		//dang ki input voi cua so game
		_rid[0].dwFlags = RIDEV_INPUTSINK;
		//cua so game
		_rid[0].hwndTarget = hwnd;
		_rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		_rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		//dang ki
		RegisterRawInputDevices(_rid, 1, sizeof(_rid[0]));
		if(_mouseCaptured)
			SetCapture(hwnd);
		//dua gia tri cac o nho ve 0
		ZeroMemory(_controllerState, sizeof(ControllerState) * MAX_CONTROLLERS);
		//kiem tra cac thiet bi ket noi
		checkControllers();
	}
	catch (...) 
	{
		throw (Error(ErrorNS::FATAL_ERROR, "Error initializing input system"));
	}
}

void Input::checkControllers()
{
	DWORD result;
	//lan luot lay trang thai thiet bi
	for (DWORD i = 0; i <MAX_CONTROLLERS; i++)
	{
		result = XInputGetState(i, &_controllerState[i].state);
		//thanh cong thi connected = true va nguoc lai
		if (result == ERROR_SUCCESS)
			_controllerState[i].connected = true;
		else
			_controllerState[i].connected = false;
	}
}


void Input::keyDown(WPARAM w)
{
	if (w < InputNS::KEYS_ARRAY_LEN)
	{
		_keyDown[w] = true;
		_keyPressed[w] = true;
	}
}

void Input::keyUp(WPARAM w)
{
	if (w < InputNS::KEYS_ARRAY_LEN)
		_keyDown[w] = false;
}

void Input::keyIn(WPARAM w)
{
	if (_newLine)
	{
		_textIn.clear();
		_newLine = false;
	}
	if (w == '\b')
	{
		if (_textIn.length() > 0)
			_textIn.erase(_textIn.size() - 1);
	}
	else
	{
		_textIn += w;
		_charIn = w;
	}
	if ((char)w == '\r')
		_newLine = true;
}

bool Input::isKeyDown(UCHAR w) const
{
	if (w < InputNS::KEYS_ARRAY_LEN)
		return _keyDown[w];
	return false;
}

bool Input::wasKeyPressed(UCHAR w) const
{
	if (w < InputNS::KEYS_ARRAY_LEN)
		return _keyPressed[w];
	return false;
}

bool Input::anyKeyPressed() const
{
	for (int i = 0; i < InputNS::KEYS_ARRAY_LEN; i++)
		if (_keyPressed[i] == true)
			return true;
	return false;
}

void Input::clearKeyPress(UCHAR w)
{
	if (w < InputNS::KEYS_ARRAY_LEN)
		_keyPressed[w] = false;
}
void Input::clearAll()
{
	clear(InputNS::KEYS_MOUSE_TEXT);
}
void Input::clear(UCHAR what)
{
	if (what & InputNS::KEYS_DOWN)
	{
		for (int i = 0; i < InputNS::KEYS_ARRAY_LEN; i++)
			_keyDown[i] = false;
	}
	if (what & InputNS::KEYS_PRESSED)
	{
		for (int i = 0; i < InputNS::KEYS_ARRAY_LEN; i++)
			_keyPressed[i] = false;
	}
	if (what & InputNS::MOUSE)
	{
		_mouseX = _mouseRawX = _mouseY = _mouseRawY = 0;
	}
	if (what & InputNS::TEXT_IN)
	{
		_textIn.clear();
	}
}
void Input::mouseIn(LPARAM l)
{

	//thu vien windowsx.h
	_mouseX = GET_X_LPARAM(l);
	_mouseY = GET_Y_LPARAM(l);
}

void Input::mouseRawIn(LPARAM l)
{
	UINT dwSize = 40;
	static BYTE lbp[40];
	GetRawInputData((HRAWINPUT)l, RID_INPUT, lbp, &dwSize, sizeof(RAWINPUTHEADER));
	RAWINPUT *rawInput = (RAWINPUT *)lbp;
	if (rawInput->header.dwType == RIM_TYPEMOUSE)
	{
		_mouseRawX = rawInput->data.mouse.lLastX;
		_mouseRawY = rawInput->data.mouse.lLastY;
	}
}


void Input::readController()
{
	DWORD result;
	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
	{
		if (_controllerState[i].connected)
		{
			result = XInputGetState(i, &_controllerState[i].state);
			if (result == ERROR_DEVICE_NOT_CONNECTED)
			{
				_controllerState[i].connected = false;
			}
		}
	}
}

const ControllerState * Input::getControllerState(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return &_controllerState[n];
}

const WORD Input::getGamepadButtons(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return _controllerState[n].state.Gamepad.wButtons;
}


bool Input::getGamepadDPadUp(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return ((_controllerState[n].state.Gamepad.wButtons&GAMEPAD_DPAD_UP) != 0);
}

bool Input::getGamepadDPadDown(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return ((_controllerState[n].state.Gamepad.wButtons&GAMEPAD_DPAD_DOWN) != 0);
}

bool Input::getGamepadDPadLeft(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return ((_controllerState[n].state.Gamepad.wButtons&GAMEPAD_DPAD_LEFT) != 0);
}

bool Input::getGamepadDPadRight(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return bool((_controllerState[n].state.Gamepad.wButtons&GAMEPAD_DPAD_RIGHT) != 0);
}

bool Input::getGamepadStart(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return bool((_controllerState[n].state.Gamepad.wButtons&GAMEPAD_START_BUTTON) != 0);
}

bool Input::getGamepadBack(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return bool((_controllerState[n].state.Gamepad.wButtons&GAMEPAD_BACK_BUTTON) != 0);
}

bool Input::getGamepadLeftThumb(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return bool((_controllerState[n].state.Gamepad.wButtons&GAMEPAD_LEFT_THUMB) != 0);
}

bool Input::getGamepadRightThumb(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return bool((_controllerState[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_THUMB) != 0);
}


bool Input::getGamepadLeftShoulder(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return bool((_controllerState[n].state.Gamepad.wButtons&GAMEPAD_LEFT_SHOULDER) != 0);
}


bool Input::getGamepadRightShoulder(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return bool((_controllerState[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_SHOULDER) != 0);
}


bool Input::getGamepadA(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return bool((_controllerState[n].state.Gamepad.wButtons&GAMEPAD_A) != 0);
}


bool Input::getGamepadB(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return bool((_controllerState[n].state.Gamepad.wButtons&GAMEPAD_B) != 0);
}


bool Input::getGamepadX(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return bool((_controllerState[n].state.Gamepad.wButtons&GAMEPAD_X) != 0);
}


bool Input::getGamepadY(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return bool((_controllerState[n].state.Gamepad.wButtons&GAMEPAD_Y) != 0);
}

BYTE Input::getGamepadLeftTrigger(UINT n)
{
	BYTE value = getGamepadLeftTriggerUndead(n);
	if (value > _triggerDeadzone)
		value = (value - _triggerDeadzone) * 255 / (255 - _triggerDeadzone);
	else
		value = 0;
	return value;
}

BYTE Input::getGamepadLeftTriggerUndead(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return _controllerState[n].state.Gamepad.bLeftTrigger;
}

BYTE Input::getGamepadRightTrigger(UINT n)
{
	BYTE value = getGamepadRightTriggerUndead(n);
	if (value > _triggerDeadzone)
		value = (value - _triggerDeadzone) * 255 /
		(255 - _triggerDeadzone);
	else
		value = 0;
	return value;
}

BYTE Input::getGamepadRightTriggerUndead(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return _controllerState[n].state.Gamepad.bRightTrigger;
}

SHORT Input::getGamepadThumbLX(UINT n)
{
	int x = getGamepadThumbLXUndead(n);
	if (x > _thumbstickDeadzone)
		x = (x - _thumbstickDeadzone) * 32767 / (32767 - _thumbstickDeadzone);
	else if (x < -_thumbstickDeadzone)
		x = (x + _thumbstickDeadzone) * 32767 / (32767 - _thumbstickDeadzone);
	else
		x = 0;
	return static_cast<SHORT>(x);
}

SHORT Input::getGamepadThumbLXUndead(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return _controllerState[n].state.Gamepad.sThumbLX;
}

SHORT Input::getGamepadThumbLY(UINT n)
{
	int y = getGamepadThumbLYUndead(n);
	if (y > _thumbstickDeadzone)
		y = (y - _thumbstickDeadzone) * 32767 / (32767 - _thumbstickDeadzone);
	else if (y < -_thumbstickDeadzone)
		y = (y + _thumbstickDeadzone) * 32767 / (32767 - _thumbstickDeadzone);
	else
		y = 0;
	return static_cast<SHORT>(y);
}

SHORT Input::getGamepadThumbLYUndead(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return _controllerState[n].state.Gamepad.sThumbLY;
}

SHORT Input::getGamepadThumbRX(UINT n)
{
	int x = getGamepadThumbRXUndead(n);
	if (x > _thumbstickDeadzone)
		x = (x - _thumbstickDeadzone) * 32767 / (32767 - _thumbstickDeadzone);
	else if (x < -_thumbstickDeadzone)
		x = (x + _thumbstickDeadzone) * 32767 / (32767 - _thumbstickDeadzone);
	else
		x = 0;
	return static_cast<SHORT>(x);
}

SHORT Input::getGamepadThumbRXUndead(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return _controllerState[n].state.Gamepad.sThumbRX;
}

SHORT Input::getGamepadThumbRY(UINT n)
{
	int y = getGamepadThumbRYUndead(n);
	if (y > _thumbstickDeadzone)
		y = (y - _thumbstickDeadzone) * 32767 / (32767 - _thumbstickDeadzone);
	else if (y < -_thumbstickDeadzone)
		y = (y + _thumbstickDeadzone) * 32767 / (32767 - _thumbstickDeadzone);
	else
		y = 0;
	return static_cast<SHORT>(y);
}

SHORT Input::getGamepadThumbRYUndead(UINT n)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	return _controllerState[n].state.Gamepad.sThumbRY;
}

void Input::gamePadVibrateLeft(UINT n, WORD speed, float sec)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	_controllerState[n].vibration.wLeftMotorSpeed = speed;
	_controllerState[n].vibrateTimeLeft = sec;
}

void Input::gamePadVibrateRight(UINT n, WORD speed, float sec)
{
	if (n > MAX_CONTROLLERS - 1)
		n = MAX_CONTROLLERS - 1;
	_controllerState[n].vibration.wRightMotorSpeed = speed;
	_controllerState[n].vibrateTimeRight = sec;
}

void Input::vibrateControllers(float frameTime)
{
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		if (_controllerState[i].connected)
		{
			_controllerState[i].vibrateTimeLeft -= frameTime;
			if (_controllerState[i].vibrateTimeLeft < 0)
			{
				_controllerState[i].vibrateTimeLeft = 0;
				_controllerState[i].vibration.wLeftMotorSpeed = 0;
			}
			_controllerState[i].vibrateTimeRight -= frameTime;
			if (_controllerState[i].vibrateTimeRight < 0)
			{
				_controllerState[i].vibrateTimeRight = 0;
				_controllerState[i].vibration.wRightMotorSpeed = 0;
			}
			XInputSetState(i, &_controllerState[i].vibration);
		}
	}
}