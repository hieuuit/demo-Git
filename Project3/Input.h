#pragma once
#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <Windows.h>
#include <windowsx.h>
#include <Xinput.h>
#include "Error.h"

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

//cac tham so cho gamepad: thiet bi dieu khien cam tay thi phai
const DWORD GAMEPAD_DPAD_UP = 0x0001;
const DWORD GAMEPAD_DPAD_DOWN = 0x0002;
const DWORD GAMEPAD_DPAD_LEFT = 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT = 0x0008;
const DWORD GAMEPAD_START_BUTTON = 0x0010;
const DWORD GAMEPAD_BACK_BUTTON = 0x0020;
const DWORD GAMEPAD_LEFT_THUMB = 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB = 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER = 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
const DWORD GAMEPAD_A = 0x1000;
const DWORD GAMEPAD_B = 0x2000;
const DWORD GAMEPAD_X = 0x4000;
const DWORD GAMEPAD_Y = 0x8000;


//namespace luu lai thong tin cua lop input
namespace InputNS
{
	//bang ma assci co 256 ki tu
	const int KEYS_ARRAY_LEN = 256;

	//do minh quy uoc
	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR TEXT_IN = 8;
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}
const DWORD MAX_CONTROLLERS = 4;

struct ControllerState
{
	XINPUT_STATE state;
	XINPUT_VIBRATION vibration;
	float vibrateTimeLeft;
	float vibrateTimeRight;
	bool connected;
};

class Input
{
	//
	//mang bool keyDown luu lai trang thai ban phim
	//mang bool keyPressed
	//tuong duong mang 256 phan tu bool
	bool _keyDown[InputNS::KEYS_ARRAY_LEN];
	bool _keyPressed[InputNS::KEYS_ARRAY_LEN];

	//std::string textIn : chuoi ki tu nguoi dung da nhap cho den khi an enter
	std::string _textIn;

	//charIn ki tu vua nhap
	char _charIn;

	//bool newLine tuc la se xoa het nhung ki tu trong textIn. dua con tro ve dau dong
	bool _newLine;

	// toa do hien tai chua chuot
	int _mouseX, _mouseY;

	// cung la toa do chuot, co tinh bao mat cao hon
	int _mouseRawX, _mouseRawY;

	//khong ro lam tai sao lai dung co 1 phan tu ma khai bao mang
	RAWINPUTDEVICE _rid[1];

	//da ket noi chuot chua
	bool _mouseCaptured;

	//nguoi dung an chuot trai
	bool _mouseLeftButton;

	//nguoi dung an chuot phai
	bool _mouseRightButton;

	//nguoi dung an nut giua(nut scroll thi phai??)
	bool _mouseMiddleButton;

	//nguoi dung an nut ben trai chuot
	bool _mouseX1Button;

	//nguoi dung an nut ben phai chuot
	bool _mouseX2Button;

	//Trang thai ket noi cac thiet bi bao gom chuot, ban phim, gamepad
	ControllerState _controllerState[MAX_CONTROLLERS];

	//gamepad
	short _thumbstickDeadzone;
	short _triggerDeadzone;
public:
	Input();
	~Input();

	void initialize(HWND hwnd, bool capture);
	void checkControllers();

	//luu y. wparam chua thong tin ve ban phim
	//lparam chua thong tin ve chuot, ngoai tru x1, x2 button no lay du lieu trong wparam

	//cac phuong thuc cua chuot

	//an phim
	void keyDown(WPARAM w);

	//tha phim
	void keyUp(WPARAM w);

	//su kien nhan phim (chaarIn, textIn, newLine
	void keyIn(WPARAM w);

	//kiem tra phim co ma w dang duoc an hay khong
	bool isKeyDown(UCHAR w) const;

	//kiem tra phim co ma w da duoc an hay chua
	bool wasKeyPressed(UCHAR w) const;

	//kiem tra co an phim nao khong
	bool anyKeyPressed() const;

	//lay ve chuoi da nhap
	std::string getTextIn() { return _textIn; }

	//lay ve ki tu vua nhap
	char getCharIn() { return _charIn; }

	//dua trang thai phim da an ve false
	void clearKeyPress(UCHAR w);

	//xoa noi dung chuoi da nhap
	void clearTextIn() { _textIn.clear(); }

	//xoa toan bo
	void clearAll();

	//su dung or bit 
	//vi du xoa ban phim + chuot
	//clear(InputNS::KEYS_DOWN | InputNS::KEYS_PRESSED | InputNS::MOUSE);
	void clear(UCHAR what);

	//mouse input
	//di chuyen chuot
	void mouseIn(LPARAM l);
	void mouseRawIn(LPARAM l);

	//set trang thai click cac button
	void setMouseLeftButton(bool b) { _mouseLeftButton = b; }
	void setMouseRightButton(bool b) { _mouseRightButton = b; }
	void setMouseMiddleButton(bool b) { _mouseMiddleButton = b; }
	void setMouseXButton(WPARAM w) { _mouseX1Button = (w & MK_XBUTTON1) ? true : false; _mouseX2Button = (w & MK_XBUTTON2) ? true : false; }

	//lay ve toa do chuot, trang thai click chuot
	int getMouseX() const { return _mouseX; }
	int getMouseY() const { return _mouseY; }
	int getMouseRawX() const { return _mouseRawX; }
	int getMouseRawY() const { return _mouseRawY; }
	bool getMouseLeftButton() const { return _mouseLeftButton; }
	bool getMouseRightButton() const { return _mouseRightButton; }
	bool getMouseMiddleButton() const { return _mouseMiddleButton; }
	bool getMouseX1() const { return _mouseX1Button; }
	bool getMouseX2() const { return _mouseX2Button; }


	//gamepad. cai nay tho cung hk ro, chung ta cung hk can dat nang lam. moi nguoi bo sung vao cho day du thoi
	void readController();
	const ControllerState* getControllerState(UINT n);
	const WORD getGamepadButtons(UINT n);

	bool getGamepadDPadUp(UINT n);
	bool getGamepadDPadDown(UINT n);
	bool getGamepadDPadLeft(UINT n);
	bool getGamepadDPadRight(UINT n);
	bool getGamepadStart(UINT n);
	bool getGamepadBack(UINT n);
	bool getGamepadLeftThumb(UINT n);
	bool getGamepadRightThumb(UINT n);
	bool getGamepadLeftShoulder(UINT n);
	bool getGamepadRightShoulder(UINT n);
	bool getGamepadA(UINT n);
	bool getGamepadB(UINT n);
	bool getGamepadX(UINT n);
	bool getGamepadY(UINT n);

	//Gamepad Left
	BYTE getGamepadLeftTrigger(UINT n);
	BYTE getGamepadLeftTriggerUndead(UINT n);
	SHORT getGamepadThumbLX(UINT n);
	SHORT getGamepadThumbLXUndead(UINT n);
	SHORT getGamepadThumbLY(UINT n);
	SHORT getGamepadThumbLYUndead(UINT n);
	void gamePadVibrateLeft(UINT n, WORD speed, float sec);

	//Gamepad right
	BYTE getGamepadRightTrigger(UINT n);
	BYTE getGamepadRightTriggerUndead(UINT n);
	SHORT getGamepadThumbRX(UINT n);
	SHORT getGamepadThumbRXUndead(UINT n);
	SHORT getGamepadThumbRY(UINT n);
	SHORT getGamepadThumbRYUndead(UINT n);
	void gamePadVibrateRight(UINT n, WORD speed, float sec);

	//
	void vibrateControllers(float frameTime);
};

#endif