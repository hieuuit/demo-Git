#include "TextureManager.h"



TextureManager::TextureManager()
{
	_texture = NULL;
	_width = 0;
	_height = 0;
	_filename = NULL;
	_graphics = NULL;
	_initialized = false;
}


TextureManager::~TextureManager()
{
	SAFE_RELEASE(_texture);
}

void TextureManager::onLostDevice()
{
	if (!_initialized)
		return;
	SAFE_RELEASE(_texture);
}

void TextureManager::onResetDevice()
{
	if (!_initialized)
		return;
	_graphics->loadTexture(_filename, GraphicsNS::TRANSCOLOR, _width, _height, _texture);
}

bool TextureManager::initialize(Graphics * graphics, const char * fileName)
{
	try {
		_graphics = graphics;
		_filename = fileName;
		_result = graphics->loadTexture(_filename, GraphicsNS::TRANSCOLOR, _width, _height, _texture);
		if (FAILED(_result))
		{
			SAFE_RELEASE(_texture);
			return false;
		}
	}
	catch (...) { return false; }
	_initialized = true;
	return true;
}
