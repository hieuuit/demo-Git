#pragma once
#include <Windows.h>

//d3d9.h chi la 1 file header chua nhung nguyen mau ham
//chua he dc cai dat

#include <d3d9.h>

//Tranh truong hop 1 vung nho bi xoa 2 lan

//giai phong vung nho
template <typename T>
void safeRelease(T &ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = NULL;
	}
}

#define SAFE_RELEASE safeRelease

//huy 1 vung nho
template <typename T>
void safeDelete(T &ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = 0;
	}
}

#define SAFE_DELETE safeDelete

//huy 1 mang
template <typename T>
void safeDeleteArray(T &ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = NULL;
	}
}

#define SAFE_DELETE_ARRAY safeDeleteArray


const int WIDTH = 1280;
const int HEIGHT = 768;
const bool FULLSCREEN = false;

const char CLASS_NAME[] = "HELLO";
const char APP_TITLE[] = "FIRST PRO..";


const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;
const float MIN_FRAME_RATE = 10.0f;
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;

enum WEAPON { TORPEDO, SHIP, STONE };
//#define TRANSCOLOR SETCOLOR_ARGB(0,255,0,255)
