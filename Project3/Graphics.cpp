#include "Graphics.h"



void Graphics::initD3Dpp()
{
	try
	{
		ZeroMemory(&_d3dpp, sizeof(_d3dpp));

		_d3dpp.BackBufferHeight = _height;
		_d3dpp.BackBufferWidth = _width;
		_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		_d3dpp.Windowed = !_fullScreen;

		if (_fullScreen)
			_d3dpp.BackBufferFormat = D3DFMT_X8B8G8R8;
		else
			_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

		_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		_d3dpp.BackBufferCount = 1;

		_d3dpp.hDeviceWindow = _hwnd;
	}
	catch (...)
	{
		throw(Error(ErrorNS::FATAL_ERROR, "Error initializing D3D presentation parameters"));
	}
}

HRESULT Graphics::reset()
{
	_result = E_FAIL;
	initD3Dpp();
	_result = _device->Reset(&_d3dpp);
	return _result;
}

Graphics::Graphics()
{
	_width = WIDTH;
	_height = HEIGHT;
	_fullScreen = FULLSCREEN;
	_hwnd = NULL;
	_direct = NULL;
	_device = NULL;

	_backColor = D3DCOLOR_ARGB(255, 0, 0, 0);
}


Graphics::~Graphics()
{
	releaseAll();
}

void Graphics::releaseAll()
{
	safeRelease(_direct);
	safeRelease(_device);
}

void Graphics::initialize(HWND hwnd, int width, int height, bool fullscreen)
{
	_hwnd = hwnd;
	_width = width;
	_height = height;
	_fullScreen = fullscreen;

	_direct = Direct3DCreate9(D3D_SDK_VERSION);
	if (_direct == NULL)
	{
		throw(Error(ErrorNS::FATAL_ERROR, "Error initializing Direct3D"));
	}

	initD3Dpp();

	if (_fullScreen)
	{
		if (isAdapterCompatible())
		{
			_d3dpp.FullScreen_RefreshRateInHz = _pMode.RefreshRate;
		}
		else
			throw(Error(ErrorNS::FATAL_ERROR, "The graphics device does not support the" \
				"specified resolution and/or format."));
	}

	_D3DCAPS9 caps;
	DWORD behavior;
	_result = _direct->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 || caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;


	_result = _direct->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hwnd, behavior, &_d3dpp, &_device);


	if (FAILED(_result))
		throw(Error(ErrorNS::FATAL_ERROR, "Error creating Direct3D device"));

	_result = D3DXCreateSprite(_device, &_sprite);
	if (FAILED(_result))
		throw(Error(ErrorNS::FATAL_ERROR, "Error creating Direct3D sprite"));
}

HRESULT Graphics::showBackbuffer()
{
	_result = E_FAIL;

	//_device->Clear(0, NULL, D3DCLEAR_TARGET, _backColor, 0.0f, 0);

	_result = _device->Present(0, 0, 0, 0);
	return _result;
}

bool Graphics::isAdapterCompatible()
{
	UINT count = _direct->GetAdapterCount();

	for (int i = 0; i < count - 1; i++)
	{
		_direct->EnumAdapterModes(D3DADAPTER_DEFAULT, _d3dpp.BackBufferFormat, i, &_pMode);
		if (_pMode.Width == _d3dpp.BackBufferWidth &&
			_pMode.Height == _d3dpp.BackBufferHeight &&
			_pMode.RefreshRate == _d3dpp.FullScreen_RefreshRateInHz)
			return true;
	}

	return false;
}

void Graphics::changeDisplayMode(GraphicsNS::DISPLAY_MODE mode)
{
	try {
		switch (mode)
		{
		case GraphicsNS::FULLSCREEN:
			if (_fullScreen)
				return;
			_fullScreen = true; break;
		case GraphicsNS::WINDOW:
			if (_fullScreen == false)
				return;
			_fullScreen = false; break;
		default:
			_fullScreen = !_fullScreen;
		}
		reset();
		if (_fullScreen)
		{
			SetWindowLong(_hwnd, GWL_STYLE, WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP);
		}
		else
		{
			SetWindowLong(_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(_hwnd, HWND_TOP, 0, 0, WIDTH, HEIGHT, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);


			RECT clientRect;
			GetClientRect(_hwnd, &clientRect);


			MoveWindow(_hwnd,
				0,
				0,
				WIDTH + (WIDTH - clientRect.right),
				HEIGHT + (HEIGHT - clientRect.bottom),
				TRUE);
		}

	}
	catch (...)
	{
		SetWindowLong(_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(_hwnd, HWND_TOP, 0, 0, WIDTH, HEIGHT, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		RECT clientRect;
		GetClientRect(_hwnd, &clientRect);
		MoveWindow(
			_hwnd,
			0,
			0,
			WIDTH + (WIDTH - clientRect.right),
			HEIGHT + (HEIGHT - clientRect.bottom),
			TRUE);
	}
}

HRESULT Graphics::getDeviceState()
{
	_result = E_FAIL;
	if (_device == NULL)
		return _result;
	_result = _device->TestCooperativeLevel();
	return _result;
}

HRESULT Graphics::beginScene()
{
	_result = E_FAIL;
	if (_device == NULL)
		return _result;
	_device->Clear(0, NULL, D3DCLEAR_TARGET, _backColor, 1.0F, 0);
	_result = _device->BeginScene();
	return _result;
}

HRESULT Graphics::endScene()
{
	_result = E_FAIL;
	if (_device)
		_result = _device->EndScene();
	return _result;
}

HRESULT Graphics::loadTexture(const char * fileName, COLOR_ARGB transColor, UINT & width, UINT & height, LP_TEXTURE & texture)
{
	//Thông tin ảnh
	D3DXIMAGE_INFO imageInfo;
	_result = E_FAIL;
	try
	{
		//nếu đường dẫn NULL thì sẽ trả về lỗi
		if (fileName == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		//Lấy thông tin ảnh có đường dẫn fileName và đổ vào imageInfo, thành công hay không lưu vào _result
		_result = D3DXGetImageInfoFromFile(fileName, &imageInfo);

		//_result lỗi thì thoát
		if (_result != D3D_OK)
		{
			return _result;
		}

		//Tạo texture từ đường dẫn ảnh fileName
		width = imageInfo.Width;
		height = imageInfo.Height;
		_result = D3DXCreateTextureFromFileEx(
			_device,
			fileName,
			width,
			height,
			1,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			transColor,
			&imageInfo,
			NULL,
			&texture
		);
	}
	catch (...)
	{
		throw(Error(ErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
	}

	return _result;
}

void Graphics::drawSprite(const SpriteData & data, COLOR_ARGB color)
{
	if (data.texture == NULL)
	{
		return;
	}
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)(data.width / 2 * data.scale), (float)(data.height / 2 * data.scale));
	D3DXVECTOR2 translate = D3DXVECTOR2((float)data.x, (float)data.y);
	D3DXVECTOR2 scaling(data.scale, data.scale);
	if (data.flipHorizontal)
	{
		scaling.x *= -1;
		spriteCenter.x -= (float)(data.width * data.scale);
		translate.x += (float)(data.width * data.scale);
	}
	if (data.flipVertical)
	{
		scaling.y *= -1;
		spriteCenter.y -= (float)(data.height * data.scale);
		translate.y += (float)(data.height  *data.scale);
	}
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,
		NULL,
		0.0f,
		&scaling,
		&spriteCenter,
		(float)(data.angle),
		&translate
	);
	_sprite->SetTransform(&matrix);
	_sprite->Draw(data.texture, &data.rect, NULL, NULL, color);
}
