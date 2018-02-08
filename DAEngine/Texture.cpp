#include "stdafx.h"
#include "Texture.h"
#include "DAEngine.h"

Texture::Texture()
{
}


Texture::~Texture()
{
	this->Release();
}

void Texture::SetFile(const WCHAR * File)
{
	wcscpy_s(this->TextureFile, File);
}

HRESULT Texture::Load()
{
	//return D3DXCreateTextureFromFileW(this->Engine->Dev(), this->TextureFile, &this->iD3DT9);
	return D3DXCreateTextureFromFileExW(
		this->Engine->Dev(),
		this->TextureFile,
		D3DX_DEFAULT_NONPOW2, // Original Size
		D3DX_DEFAULT_NONPOW2, // Original Size
		D3DX_DEFAULT, // All mip maps
		0, // No usage defined
		D3DFMT_FROM_FILE, // Format from file
		D3DPOOL_MANAGED, // memory managed
		D3DX_DEFAULT, // filter default
		D3DX_DEFAULT, // mip filter default
		0, // no color key
		nullptr, // no used
		nullptr, // no used
		&this->iD3DT9
	);
}

HRESULT Texture::Release()
{
	return this->iD3DT9->Release();
}

bool Texture::Lock(D3DCOLOR ** Array)
{
	D3DLOCKED_RECT lr;
	bool res = this->Engine->SetError(this->iD3DT9->LockRect(0, &lr, nullptr, 0)) == S_OK;
	*Array = (D3DCOLOR*)lr.pBits;
	return res;
}

bool Texture::Unlock()
{
	return this->iD3DT9->UnlockRect(0) == S_OK;
}
