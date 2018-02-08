#pragma once

#define MAX_KEY 100

class CTexMng :
	public CEngineElement
{
	struct _tagTxInfo
	{
		TCHAR szKey[MAX_KEY];
		TCHAR szPath[MAX_PATH];
		LPDIRECT3DTEXTURE9 lpResource;
	};
public:
	CTexMng();
	~CTexMng();

	void Add(const TCHAR * szKey, const TCHAR * szFile);
	LPDIRECT3DTEXTURE9 GetTexture(const TCHAR * szKey);
	bool Load();
	void Draw();
	void Release();

	LPDIRECT3DTEXTURE9 operator ()(const TCHAR*szTextureKey)
	{
		return this->GetTexture(szTextureKey);
	}

private:
	std::list<_tagTxInfo> lResources;
};

