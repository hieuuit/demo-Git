#include "Image.h"



Image::Image()
{
	_initialized = false;
	_spriteData.width = 2;
	_spriteData.height = 2;
	_spriteData.x = 0.0;
	_spriteData.y = 0.0;
	_spriteData.scale = 1.0;
	_spriteData.angle = 0.0;
	_spriteData.rect.left = 0;
	_spriteData.rect.top = 0;
	_spriteData.rect.right = _spriteData.width;
	_spriteData.rect.bottom = _spriteData.height;
	_spriteData.texture = NULL;
	_spriteData.flipHorizontal = false;
	_spriteData.flipVertical = false;
	_cols = 1;
	_textureManager = NULL;
	_startFrame = 0;
	_endFrame = 0;
	_currentFrame = 0;
	_frameDelay = 1.0;
	_animTimer = 0.0;
	_visible = true;
	_loop = true;
	_animComplete = false;
	_graphics = NULL;
	_colorFilter = GraphicsNS::WHITE;
}


Image::~Image()
{
}

void Image::setCurrentFrame(int currentFrame)
{
	if (currentFrame >= 0)
	{
		_currentFrame = currentFrame;
		_animComplete = false;
		setRect();
	}
}

void Image::setRect()
{
	_spriteData.rect.left = (_currentFrame % _cols) * _spriteData.width;
	_spriteData.rect.right = _spriteData.rect.left + _spriteData.width;
	_spriteData.rect.top = (_currentFrame / _cols) * _spriteData.height;
	_spriteData.rect.bottom = _spriteData.rect.top + _spriteData.height;
}

bool Image::initialize(Graphics * graphics, int width, int height, int ncols, TextureManager * textureManager)
{
	try {
		_graphics = graphics;
		_textureManager = textureManager;

		_spriteData.texture = _textureManager->getTexture();
		if (width == 0)
			width = textureManager->getWidth();
		_spriteData.width = width;
		if (height == 0)
			height = textureManager->getHeight();
		_spriteData.height = height;
		_cols = ncols;
		if (_cols == 0)
			_cols = 1;

		_spriteData.rect.left = (_currentFrame % _cols) * _spriteData.width;
		_spriteData.rect.right = _spriteData.rect.left + _spriteData.width;
		_spriteData.rect.top = (_currentFrame / _cols) * _spriteData.height;
		_spriteData.rect.bottom = _spriteData.rect.top + _spriteData.height;
	}
	catch (...)
	{
		return false;
	}
	_initialized = true;
	return true;
}

void Image::draw(COLOR_ARGB color)
{
	if (!_visible || _graphics == NULL)
		return;

	_spriteData.texture = _textureManager->getTexture();
	if (color == GraphicsNS::FILTER)
		_graphics->drawSprite(_spriteData, _colorFilter);
	else
		_graphics->drawSprite(_spriteData, color);
}

void Image::draw(SpriteData spriteData, COLOR_ARGB color)
{
	if (!_visible || _graphics == NULL)
		return;

	_spriteData.rect = spriteData.rect;
	_spriteData.texture = _textureManager->getTexture();
	if (color == GraphicsNS::FILTER)
		_graphics->drawSprite(_spriteData, _colorFilter);
	else
		_graphics->drawSprite(_spriteData, color);
}

void Image::update(float frameTime)
{
	if (_endFrame - _startFrame > 0)
	{
		_animTimer += frameTime;
		if (_animTimer > _frameDelay)
		{
			_animTimer -= _frameDelay;
			_currentFrame++;
			if (_currentFrame < _startFrame || _currentFrame > _endFrame)
			{
				if (_loop == true)
					_currentFrame = _startFrame;
				else
				{
					_currentFrame = _endFrame;
					_animComplete = true;
				}
			}
			setRect();
		}
	}
}
