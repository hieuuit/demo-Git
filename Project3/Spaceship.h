#pragma once
#ifndef SPACESHIP_H
#define SPACESHIP_H
#include "Player.h"
#include "PlayerBullet.h"
#include "Stone.h"
#include "Explosion.h"
#include <vector>













class SpaceShip : public Game
{
protected:
	TextureManager _playerT;
	TextureManager _backgroundT;
	TextureManager _stoneT;
	TextureManager _bulletT;
	TextureManager _exploT;

	Player _player;

	Image _background;

	std::vector<PlayerBullet> _bullets;
	std::vector<Stone> _stones;
	std::vector<Image> _explos;
public:
	SpaceShip() {}
	void initialize(HWND hwnd)
	{
		Game::initialize(hwnd);
		if (!_playerT.initialize(_graphics, "ship.png"))
			throw(Error(ErrorNS::FATAL_ERROR, "khong the khoi tao player"));
		if (!_backgroundT.initialize(_graphics, "orion.jpg"))
			throw(Error(ErrorNS::FATAL_ERROR, "khong the khoi tao background"));
		if (!_bulletT.initialize(_graphics, "bullet.png"))
			throw(Error(ErrorNS::FATAL_ERROR, "khong the khoi tao bullet"));
		if (!_stoneT.initialize(_graphics, "stones.png"))
			throw(Error(ErrorNS::FATAL_ERROR, "khong the khoi tao stone"));
		if (!_exploT.initialize(_graphics, "explosion.png"))
			throw(Error(ErrorNS::FATAL_ERROR, "khong the khoi tao explosion"));



		if (!_background.initialize(_graphics, 640, 480, 1, &_backgroundT))
			throw(Error(ErrorNS::FATAL_ERROR, "khong the khoi tao background"));

		if (!_player.initialize(this, 180, 250, 2, &_playerT))
			throw(Error(ErrorNS::FATAL_ERROR, "khong the khoi tao player"));

		float scale = 0.25;
		_player.setScale(scale);
		_player.setX(WIDTH / 2 - _player.getWidth() * scale / 2);
		_player.setY(HEIGHT - _player.getHeight() * scale);
		_player.setFrames(0, 1);
		_player.setCurrentFrame(0);
		_player.setFrameDelay(0.05f);

		_background.setX(0);
		_background.setY(0);
		_background.setScale(2);


		//_graphics->changeDisplayMode(GraphicsNS::DISPLAY_MODE::FULLSCREEN);
	}

	void update()
	{
		if (_input->isKeyDown(65)) {
			_player.setX(_player.getX() - 1);
		}
		if (_input->isKeyDown(68)) {
			_player.setX(_player.getX() + 1);
		}
		if (_input->isKeyDown(83)) {
			_player.setY(_player.getY() + 1);
		}
		if (_input->isKeyDown(87)) {
			_player.setY(_player.getY() - 1);
		}

		if (_input->wasKeyPressed(32)) {
			PlayerBullet b;
			b.initialize(this, 18, 18, 1, &_bulletT);
			b.setScale(0.5f);
			b.setX(_player.getCenterX() - b.getWidth() * b.getScale() / 2);
			b.setY(_player.getY());
			_bullets.push_back(b);
			_input->clearKeyPress(32);
		}
		if (rand() % 100 == 0) {
			Stone s;
			s.initialize(this, 200, 200, 1, &_stoneT);
			s.setScale(0.25f);
			s.setX(_player.getCenterX() - s.getWidth() * s.getScale() / 2);
			s.setY(0 - s.getHeight());

			_stones.push_back(s);
			_input->clearKeyPress(32);
		}
		for (int i = 0; i < _bullets.size(); i++) {
			_bullets.at(i).update(_frameTime);
			if (_bullets.at(i).getY() < 0) {
				_bullets.erase(_bullets.begin() + i);
			}
			else {
				_bullets.at(i).setY(_bullets.at(i).getY() - 3);
			}
		}
		for (int i = 0; i < _stones.size(); i++) {
			_stones.at(i).update(_frameTime);
			if (_stones.at(i).getY() > HEIGHT) {
				_stones.erase(_stones.begin() + i);
			}
			else {
				_stones.at(i).setY(_stones.at(i).getY() + 3);
			}
		}
		for (int i = 0; i < _explos.size(); i++) {
			if (_explos.at(i).getAnimationComplete())
			{
				_explos.erase(_explos.begin() + i);
				continue;
			}
			_explos.at(i).update(_frameTime);
		}
		_player.update(_frameTime);
	}
	void render()
	{
		_graphics->spriteBegin();

		_background.draw();
		for (int i = 0; i < _bullets.size(); i++) {
			_bullets.at(i).draw();
		}
		for (int i = 0; i < _stones.size(); i++) {
			_stones.at(i).draw();
		}

		_player.draw();

		for (int i = 0; i < _explos.size(); i++) {
			_explos.at(i).draw();
		}

		_graphics->spriteEnd();
	}

	void collisions()
	{
		VECTOR2 collisionVector;
		for(int i = 0; i < _stones.size(); i++)
		if (_player.collidesWith(_stones.at(i), collisionVector))
		{
			if (_player.getVisible())
			{
				Image img;
				img.initialize(_graphics, 96, 96, ExplosionNS::TEXTURE_COLS, &_exploT);
				img.setScale(2);
				img.setX(_player.getCenterX() - img.getWidth() * img.getScale() / 1.75f);
				img.setY(_player.getCenterY() - img.getHeight() * img.getScale() / 2);
				img.setFrames(ExplosionNS::SHIP_START_FRAME, ExplosionNS::SHIP_END_FRAME);
				img.setCurrentFrame(ExplosionNS::SHIP_START_FRAME);
				img.setFrameDelay(ExplosionNS::SHIP_ANIMATION_DELAY);
				img.setLoop(false);
				_explos.push_back(img);
				_player.damage(STONE);
			}
		}
	}
};
#endif