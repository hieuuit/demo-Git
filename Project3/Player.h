#ifndef  PLAYER_H
#define PLAYER_H
#include "Entity.h"

namespace PlayerNS
{
	const int WIDTH = 900;
	const int HEIGHT = 250;
	const int X = ::WIDTH / 2 - WIDTH / 2;
	const int Y = ::HEIGHT / 2 - HEIGHT / 2;
	const float ROTATION_RATE = 0;
	const float SPEED = 100;
	const float MASS = 300.0f;
	const int   TEXTURE_COLS = 5;
	const int   SHIP_START_FRAME = 0;
	const int   SHIP_END_FRAME = 1;
	const float SHIP_ANIMATION_DELAY = 0.1f;
}

class Player : public Entity
{
public:
	Player()
	{
		_spriteData.width = PlayerNS::WIDTH;
		_spriteData.height = PlayerNS::HEIGHT;
		_spriteData.x = PlayerNS::X;
		_spriteData.y = PlayerNS::Y;
		_spriteData.rect.bottom = PlayerNS::HEIGHT;
		_spriteData.rect.right = PlayerNS::WIDTH;
		_velocity.x = 0;
		_velocity.y = 0;
		_frameDelay = PlayerNS::SHIP_ANIMATION_DELAY;
		_startFrame = PlayerNS::SHIP_START_FRAME;
		_endFrame = PlayerNS::SHIP_END_FRAME;
		_currentFrame = _startFrame;
		_radius = PlayerNS::WIDTH / PlayerNS::TEXTURE_COLS / 2.0;
		_collisionType = EntityNS::CIRCLE;
	}
	~Player(){}
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM)
	{
		return(Entity::initialize(gamePtr, width, height, ncols, textureM));
	}
	void update(float frameTime)
	{
		Entity::update(frameTime);
	}
	void damage(int weapon)
	{
		_visible = false;
	}
private:

};

#endif // ! PLAYER_H