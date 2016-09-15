#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <d3d9.h>
#include <D3dx9core.h>
#include "Constants.h"
#include "Error.h"

#define D3D9 LPDIRECT3D9
#define D3DDEVICE LPDIRECT3DDEVICE9
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \ ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b) & 0xff)))
#define VECTOR2 D3DXVECTOR2

//con tro den anh texture
#define LP_TEXTURE LPDIRECT3DTEXTURE9
//con tro den anh sprite
#define LP_SPRITE LPD3DXSPRITE

//phan biet texture voi sprite. texture la 1 buc anh lon. con sprite la 1 phan cua texture co kich thuoc sprite.getRect()

namespace GraphicsNS
{
	const COLOR_ARGB ORANGE = D3DCOLOR_ARGB(255, 255, 165, 0);
	const COLOR_ARGB BROWN = D3DCOLOR_ARGB(255, 139, 69, 19);
	const COLOR_ARGB LTGRAY = D3DCOLOR_ARGB(255, 192, 192, 192);
	const COLOR_ARGB GRAY = D3DCOLOR_ARGB(255, 128, 128, 128);
	const COLOR_ARGB OLIVE = D3DCOLOR_ARGB(255, 128, 128, 0);
	const COLOR_ARGB PURPLE = D3DCOLOR_ARGB(255, 128, 0, 128);
	const COLOR_ARGB MAROON = D3DCOLOR_ARGB(255, 128, 0, 0);
	const COLOR_ARGB TEAL = D3DCOLOR_ARGB(255, 0, 128, 128);
	const COLOR_ARGB GREEN = D3DCOLOR_ARGB(255, 0, 128, 0);
	const COLOR_ARGB NAVY = D3DCOLOR_ARGB(255, 0, 0, 128);
	const COLOR_ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
	const COLOR_ARGB YELLOW = D3DCOLOR_ARGB(255, 255, 255, 0);
	const COLOR_ARGB MAGENTA = D3DCOLOR_ARGB(255, 255, 0, 255);
	const COLOR_ARGB RED = D3DCOLOR_ARGB(255, 255, 0, 0);
	const COLOR_ARGB CYAN = D3DCOLOR_ARGB(255, 0, 255, 255);
	const COLOR_ARGB LIME = D3DCOLOR_ARGB(255, 0, 255, 0);
	const COLOR_ARGB BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
	const COLOR_ARGB BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);
	const COLOR_ARGB FILTER = D3DCOLOR_ARGB(0, 0, 0, 0);
	const COLOR_ARGB ALPHA25 = D3DCOLOR_ARGB(64, 255, 255, 255);
	const COLOR_ARGB ALPHA50 = D3DCOLOR_ARGB(128, 255, 255, 255);
	const COLOR_ARGB TRANSCOLOR = D3DCOLOR_ARGB(0, 255, 0, 255);
	enum DISPLAY_MODE
	{
		TOGGLE,
		FULLSCREEN,
		WINDOW
	};
}
//cau truc thong tin anh sprite
struct SpriteData
{
	
	//kich thuoc
	int width;
	int height;
	//toa do
	float x;
	float y;
	//ti le phong to thu nho
	float scale;
	//goc xoay
	float angle;
	//kich thuoc cua no tren texture
	RECT rect;
	//texture chua no
	LP_TEXTURE texture;
	//anh dang bi lat ngang
	bool flipHorizontal;
	//anh dang bi lat doc lat doc
	bool flipVertical;
};

class Graphics
{
	int _width;
	int _height;
	bool _fullScreen;
	HWND _hwnd;
	D3DPRESENT_PARAMETERS _d3dpp;
	D3D9 _direct;
	D3DDEVICE _device;
	HRESULT _result;
	void initD3Dpp();
	D3DDISPLAYMODE _pMode;
	COLOR_ARGB  _backColor;


	LP_SPRITE _sprite;
public:
	Graphics();
	virtual ~Graphics();
	void releaseAll();
	void initialize(HWND hwnd, int width, int height, bool fullscreen);
	HRESULT showBackbuffer();
	bool isAdapterCompatible();
	void changeDisplayMode(GraphicsNS::DISPLAY_MODE mode);
	void setBackColor(COLOR_ARGB c) { _backColor = c; }
	D3D9 get3D() { return _direct; }
	D3DDEVICE get3Ddevice() { return _device; }
	bool getFullscreen() { return _fullScreen; }
	HRESULT reset();
	HRESULT getDeviceState();
	HRESULT beginScene();
	HRESULT endScene();

	HRESULT loadTexture(const char* fileName, COLOR_ARGB transColor, UINT &width, UINT &height, LP_TEXTURE &texture);
	void drawSprite(const SpriteData& data, COLOR_ARGB color);
	void spriteBegin() { _sprite->Begin(D3DXSPRITE_ALPHABLEND); }
	void spriteEnd() { _sprite->End(); }


	static float Vector2Length(const VECTOR2 *v) { return D3DXVec2Length(v); }
	static float Vector2Dot(const VECTOR2 *v1, const VECTOR2 *v2) { return D3DXVec2Dot(v1, v2); }
	static void Vector2Normalize(VECTOR2 *v) { D3DXVec2Normalize(v, v); }
	static VECTOR2* Vector2Transform(VECTOR2 *v, D3DXMATRIX *m) { return D3DXVec2TransformCoord(v, v, m); }

};

#endif