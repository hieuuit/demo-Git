#pragma once
#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "Image.h"

namespace ExplosionNS
{
	const int WIDTH = 960;
	const int HEIGHT = 480;
	const int X = ::WIDTH / 2 - WIDTH / 2;
	const int Y = ::HEIGHT / 2 - HEIGHT / 2;
	const float ROTATION_RATE = 0;
	const int   TEXTURE_COLS = 10;
	const int   SHIP_START_FRAME = 0;
	const int   SHIP_END_FRAME = 44;
	const float SHIP_ANIMATION_DELAY = 0.1f;
}


class Explosion : public Image
{
public:
	Explosion()
	{
		_spriteData.width = ExplosionNS::WIDTH;
		_spriteData.height = ExplosionNS::HEIGHT;
		_spriteData.x = ExplosionNS::X;
		_spriteData.y = ExplosionNS::Y;
		_spriteData.rect.bottom = ExplosionNS::HEIGHT;
		_spriteData.rect.right = ExplosionNS::WIDTH;
		_currentFrame = _startFrame;
	}
	~Explosion();
};




#endif // !EXPLOSION_H
