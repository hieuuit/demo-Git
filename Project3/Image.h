#pragma once
#ifndef IMAGE_H
#define IMAGE_H

#include "TextureManager.h"
//o day la thiet ke cho animation
class Image
{
protected:
	Graphics *_graphics;
	TextureManager *_textureManager;
	SpriteData _spriteData;
	COLOR_ARGB _colorFilter;
	int _cols;
	int _startFrame;
	int _endFrame;
	int _currentFrame;
	float _frameDelay;
	float _animTimer;
	HRESULT _result;
	bool _loop;
	bool _visible;
	bool _initialized;
	bool _animComplete;
public:
	//thiet lap cac gia tri mac dinh cho cac bien ben tren
	//khong co nhieu y nghia. chu yeu do ham init ben duoi quyet dinh
	Image();
	virtual ~Image();
	//Lay ve thong tin anh sprite
	const virtual SpriteData& getSpriteInfo() { return _spriteData; }
	//trang thai co hien thi hay khong
	virtual bool  getVisible() { return _visible; }
	//lay ve x
	virtual float getX() { return _spriteData.x; }
	//lay ve y
	virtual float getY() { return _spriteData.y; }
	//lay ve ti le thu phong
	virtual float getScale() { return _spriteData.scale; }
	//lay ve kich thuoc sprite
	virtual int getWidth() { return _spriteData.width; }
	virtual int getHeight() { return _spriteData.height; }
	//lay ve toa do tam cua sprite
	virtual float getCenterX() { return _spriteData.x + _spriteData.width / 2 * getScale(); }
	virtual float getCenterY() { return _spriteData.y + _spriteData.height / 2 * getScale(); }
	//lay ve goc xoay theo radian va theo degree
	virtual float getDegrees() { return _spriteData.angle*(180.0f / (float)PI); }
	virtual float getRadians() { return _spriteData.angle; }

	//cai nay danh cho animation
	//1 animation co nhieu frame, animation hieu nhu 1 anh dong(*.gif)
	//khoang thoi gian giua 2 frame
	virtual float getFrameDelay() { return _frameDelay; }
	//lay ve chi so frame dau tien, cuoi cung
	virtual int getStartFrame() { return _startFrame; }
	virtual int getEndFrame() { return _endFrame; }
	//frame hien tai
	virtual int getCurrentFrame() { return _currentFrame; }

	//lay ve kich thuoc cua sprite tren texture
	virtual RECT getSpriteDataRect() { return _spriteData.rect; }
	//trang thai hoan thanh cua animation. tuc la current frame = end frame
	virtual bool getAnimationComplete() { return _animComplete; }
	//day la mau sac khi ve 1 hinh anh len man hinh
	//vi du anh png se co nhung phan trong suot, _colorFilter se thay cho cai trong suot do
	virtual COLOR_ARGB getColorFilter() { return _colorFilter; }


	//Thiet lap nguoc lai nhung cai ben tren
	virtual void setX(float newX) { _spriteData.x = newX; }
	virtual void setY(float newY) { _spriteData.y = newY; }
	virtual void setScale(float s) { _spriteData.scale = s; }
	virtual void setDegrees(float deg) { _spriteData.angle = deg*((float)PI / 180.0f); }
	virtual void setRadians(float rad) { _spriteData.angle = rad; }
	virtual void setVisible(bool v) { _visible = v; }
	virtual void setFrameDelay(float delay) { _frameDelay = delay; }
	virtual void setFrames(int s, int e) { _startFrame = s; _endFrame = e; }
	virtual void setCurrentFrame(int currentFrame);
	virtual void setRect();
	virtual void setSpriteDataRect(RECT rect) { _spriteData.rect = rect; }
	virtual void setLoop(bool loop) { _loop = loop; }
	virtual void setAnimationComplete(bool animComplete) { _animComplete = animComplete; };
	virtual void setColorFilter(COLOR_ARGB color) { _colorFilter = color; }
	virtual void setTextureManager(TextureManager *textureManager) { _textureManager = textureManager; }

	//thiet lap cac gia tri cho _spriteData
	virtual bool Image::initialize(Graphics *graphics, int width, int height, int ncols, TextureManager *textureM);
	//lat ngang, doc sprite
	virtual void flipHorizontal(bool flip) { _spriteData.flipHorizontal = flip; }
	virtual void flipVertical(bool flip) { _spriteData.flipVertical = flip; }

	//ve mau sac len man hinh
	virtual void draw(COLOR_ARGB color = GraphicsNS::WHITE);
	//ve sprite voi colorFilter = color
	virtual void draw(SpriteData spriteData, COLOR_ARGB color = GraphicsNS::WHITE);
	//thay doi cac frame hinh
	virtual void update(float frameTime);
};



#endif // !IMAGE_H
