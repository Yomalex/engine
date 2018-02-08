#pragma once
#include "EngineObject.h"
class Effect :
	public EngineObject
{
public:
	Effect(DAEngine * link);
	~Effect();

	HRESULT Load();
	HRESULT Render() { return E_NOTIMPL; }
	HRESULT Release();
};

