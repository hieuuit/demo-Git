#pragma once
#ifndef PLAYER_BULLET
#define PLAYER_BULLET
#include "Entity.h"

namespace PlayerBulletNS 
{
	const int WIDTH = 18;
	const int HEIGHT = 18;
	const int X = ::WIDTH / 2 - WIDTH / 2;
	const int Y = ::HEIGHT / 2 - HEIGHT / 2;
	const float ROTATION_RATE = 0;
	const float SPEED = 100;
	const float MASS = 300.0f;
	const int   TEXTURE_COLS = 1;
}

class PlayerBullet : public Entity
{
public:
	PlayerBullet()
	{
		_spriteData.width = PlayerBulletNS::WIDTH;
		_spriteData.height = PlayerBulletNS::HEIGHT;
		_spriteData.x = PlayerBulletNS::X;
		_spriteData.y = PlayerBulletNS::Y;
		_spriteData.rect.bottom = PlayerBulletNS::HEIGHT;
		_spriteData.rect.right = PlayerBulletNS::WIDTH;
		_velocity.x = 0;
		_velocity.y = 0;
		_currentFrame = _startFrame;
		_radius = PlayerBulletNS::WIDTH / 2.0;
		_collisionType = EntityNS::CIRCLE;
	}
	~PlayerBullet();
};

#endif // !PLAYER_BULLET


