#pragma once
#ifndef STONE_H
#define STONE_H

#include "Entity.h"

namespace StoneNS
{
	const int WIDTH = 200;
	const int HEIGHT = 200;
	const int X = ::WIDTH / 2 - WIDTH / 2;
	const int Y = ::HEIGHT / 2 - HEIGHT / 2;
	const float ROTATION_RATE = 0;
	const float SPEED = 100;
	const float MASS = 300.0f;
	const int   TEXTURE_COLS = 1;
	const int   SHIP_START_FRAME = 0;
	const int   SHIP_END_FRAME = 0;
	const float SHIP_ANIMATION_DELAY = 0.1f;
}

class Stone : public Entity
{
public:
	Stone()
	{
		_spriteData.width = StoneNS::WIDTH;
		_spriteData.height = StoneNS::HEIGHT;
		_spriteData.x = StoneNS::X;
		_spriteData.y = StoneNS::Y;
		_spriteData.rect.bottom = StoneNS::HEIGHT;
		_spriteData.rect.right = StoneNS::WIDTH;
		_velocity.x = 0;
		_velocity.y = 0;
		_currentFrame = _startFrame;
		_radius = StoneNS::WIDTH / 2.0;
		_collisionType = EntityNS::CIRCLE;
	}
};

#endif // !STONE_H