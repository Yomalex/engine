#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <stdexcept>
#include "XMUtil.h"
#pragma comment(lib,"d3d11.lib")
class CEngine;

class CEObject
{
protected:
	bool Initialized;
public:
	static CEngine* GameEngine;

	virtual void OnInitialize() = 0;
	virtual void OnUpdate(double elapsed) = 0;
	virtual void OnFrame() = 0;
	virtual void OnDispose() = 0;
};

