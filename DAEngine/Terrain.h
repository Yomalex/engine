#pragma once

class Terrain : public EngineObject
{
public:
	Terrain(DAEngine * link);
	~Terrain();

	//virtual HRESULT Load() = 0;
	HRESULT Render() { return E_NOTIMPL; }//
										  //virtual HRESULT Release() = 0;
};