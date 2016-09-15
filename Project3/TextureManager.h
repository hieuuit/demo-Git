#pragma once
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H


#include "graphics.h"
#include "constants.h"



class TextureManager
{
	Graphics *_graphics;
	const char *_filename;
	HRESULT _result;
	UINT _width;
	UINT _height;
	LP_TEXTURE _texture;
	bool _initialized;
public:
	//thiet lap mac dinh cac bien con tro la null, int = 0 va bool la false
	TextureManager();
	virtual ~TextureManager();
	//lay ve _texture
	LP_TEXTURE getTexture() const { return _texture; }
	//lay kich thuoc _texture
	UINT getWidth() const { return _width; }
	UINT getHeight() const { return _height; }

	//Mat ket noi toi thiet bi thi chuyen _initialized = false va giai phong _texture
	virtual void onLostDevice();
	//khoi tao lai _texture
	virtual void onResetDevice();
	//gan 1 vai thu can thiet va load _texture
	virtual bool initialize(Graphics *graphics, const char *fileName);
};

#endif