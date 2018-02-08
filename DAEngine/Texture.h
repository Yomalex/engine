#pragma once
#include "EngineObject.h"

class Texture : public EngineObject
{
protected:
	LPDIRECT3DTEXTURE9 iD3DT9;
	WCHAR TextureFile[260];
public:
	Texture();
	Texture(DAEngine * link, const WCHAR * File) : EngineObject(link)
	{ this->SetFile(File); }
	~Texture();

	void SetFile(const WCHAR * File);

	virtual HRESULT Load();
	virtual HRESULT Render() { return E_NOTIMPL; }
	virtual HRESULT Release();

	//virtual HRESULT OnCreateDevice() { return this->Load(); }
	virtual HRESULT OnResetDevice() { return this->Load(); }
	virtual HRESULT OnLostDevice() { return this->Release(); }

	bool Lock(D3DCOLOR ** Array);
	bool Unlock();

	LPDIRECT3DTEXTURE9 Get() { return this->iD3DT9; }
};

