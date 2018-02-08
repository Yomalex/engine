#include "stdafx.h"
#include "WZen.h"

HRESULT WebZenTexture(PDIRECT3DDEVICE9 dev, const TCHAR * File, PDIRECT3DTEXTURE9 * Texture, bool pow2)
{
	DWORD dwJump = 0;
	*Texture = nullptr;
	switch (File[_tcslen(File)-1])
	{
	case 'J':
	case 'j': dwJump = 0x18; break;
	case 'T':
	case 't': dwJump = 0x04; break;
	case 'B':
	case 'b': dwJump = 0x04; break;
	default: return D3DERR_INVALIDCALL;
	}

	HANDLE hFile = CreateFile(File, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return D3DERR_NOTFOUND;
	}
	DWORD dwSize = GetFileSize(hFile, nullptr)-dwJump, dwReturned;
	BYTE * pData = static_cast<BYTE*>(malloc(dwSize));

	SetFilePointer(hFile, dwJump, nullptr, FILE_BEGIN);
	ReadFile(hFile, pData, dwSize, &dwReturned, nullptr);
	CloseHandle(hFile);
	HRESULT hr;
	if(pow2) hr = D3DXCreateTextureFromFileInMemoryEx(dev, pData, dwSize, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, Texture);
	else hr = D3DXCreateTextureFromFileInMemoryEx(dev, pData, dwSize, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,0,NULL,NULL,Texture);
	free(pData);

	return hr;
}

BOOL WritePrivateProfileIntW(LPCWSTR lpAppName, LPCWSTR lpKeyName, INT Value, LPCWSTR lpFile)
{
	WCHAR szValue[128];
	swprintf_s(szValue, L"%d", Value);
	return WritePrivateProfileStringW(lpAppName, lpKeyName, szValue, lpFile);
}

BOOL WritePrivateProfileIntA(LPCSTR lpAppName, LPCSTR lpKeyName, INT Value, LPCSTR lpFile)
{
	char szValue[128];
	sprintf_s(szValue, "%d", Value);
	return WritePrivateProfileStringA(lpAppName, lpKeyName, szValue, lpFile);
}
