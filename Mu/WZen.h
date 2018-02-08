#pragma once

HRESULT WebZenTexture(PDIRECT3DDEVICE9 dev, const TCHAR * File, PDIRECT3DTEXTURE9*Texture, bool pow2=true);
inline void decrypt2(char* buffer, size_t size)
{
	const BYTE xorKeys[3] = { 0xFC, 0xCF, 0xAB };
	for (size_t i = 0; i<size; ++i)
	{
		*buffer ^= xorKeys[i % 3];
		buffer++;
	}
}

inline void decrypt(char* buffer, size_t size)
{
	const BYTE xorKeys[16] = {
		0xd1, 0x73, 0x52, 0xf6,
		0xd2, 0x9a, 0xcb, 0x27,
		0x3e, 0xaf, 0x59, 0x31,
		0x37, 0xb3, 0xe7, 0xa2
	};
	BYTE key = 0x5E;
	for (size_t i = 0; i<size; ++i)
	{
		char encode = *buffer;
		*buffer ^= xorKeys[i % 16];
		*buffer -= key;
		key = encode + 0x3D;
		buffer++;
	}
}

inline void GetFolder(TCHAR * out, int size, const TCHAR*file)
{
	int last = 0;
	for (last = _tcslen(file) - 1; last >= 0; last--)
	{
		if (file[last] == TEXT('/') || file[last] == TEXT('\\')) break;
	}

	ZeroMemory(out, size);
	memcpy(out, file, min(size - 1, last+1)*sizeof(TCHAR));
}

#ifdef UNICODE
#define WritePrivateProfileInt WritePrivateProfileIntW
#else
#define WritePrivateProfileInt WritePrivateProfileIntA
#endif

BOOL WritePrivateProfileIntW(LPCWSTR lpAppName, LPCWSTR lpKeyName, INT Value, LPCWSTR lpFile);
BOOL WritePrivateProfileIntA(LPCSTR lpAppName, LPCSTR lpKeyName, INT Value, LPCSTR lpFile);