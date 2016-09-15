#pragma once
#ifndef GAME_H
#define GAME_H
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <Mmsystem.h>
#include "Graphics.h"
#include "Input.h"
#include "Image.h"

class Game
{
protected:
	Graphics *_graphics;
	Input *_input;
	HWND _hwnd;
	HRESULT _result;
	LARGE_INTEGER _timeStart;
	LARGE_INTEGER _timeEnd;
	LARGE_INTEGER _timeFreq;
	float _frameTime;
	float _fps;
	DWORD _sleepTime;
	bool _paused;
	bool _initialized;
public:
	Game();
	virtual ~Game();
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
	virtual void initialize(HWND);
	virtual void releaseAll();
	virtual void resetAll();
	virtual void deleteAll();
	virtual void renderGame();
	virtual void handleLostGraphicsDevice();
	Graphics* getGraphics();
	Input* getInput();
	void exitGame();
	virtual void update()
	{
	}
	virtual void ai() {}
	virtual void collisions() {}
	virtual void render() 
	{
	}
	virtual void run(HWND hwnd);

};
#endif 

