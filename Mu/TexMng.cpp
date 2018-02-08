#include "stdafx.h"
#include "TexMng.h"
#include "WZen.h"

CTexMng::CTexMng()
{
}


CTexMng::~CTexMng()
{
	this->Release();
}

void CTexMng::Add(const TCHAR * szKey, const TCHAR * szFile)
{
	_tagTxInfo tx;
	_tcscpy_s(tx.szKey, szKey);
	_tcscpy_s(tx.szPath, szFile);
	tx.lpResource = nullptr;
	this->lResources.push_back(tx);
}

LPDIRECT3DTEXTURE9 CTexMng::GetTexture(const TCHAR * szKey)
{
	int len = _tcslen(szKey);
	DWORD dwLongCmp = *(DWORD*)szKey;
	WORD dwShortCmp = *(WORD*)szKey;
	for (std::list<_tagTxInfo>::iterator it = this->lResources.begin(); it != this->lResources.end(); it++)
	{
		if (
			(len >= 4 && dwLongCmp == *(DWORD*)it->szKey && _tcscmp(it->szKey, szKey) == 0)||
			(len >= 2 && dwShortCmp == *(WORD*)it->szKey && _tcscmp(it->szKey, szKey) == 0)
			)
		{
			return it->lpResource;
		}
	}
	return nullptr;
}

bool CTexMng::Load()
{
	if (this->Loaded) return true;
	for (std::list<_tagTxInfo>::iterator it = this->lResources.begin(); it != this->lResources.end(); it++)
	{
		if (FAILED(WebZenTexture(this->GetDevice(), it->szPath, &it->lpResource, false)))
		{
			this->Release();
			return false;
		}
	}
	this->Loaded = true;
	return true;
}

void CTexMng::Draw()
{
	// empty
}

void CTexMng::Release()
{
	if (!this->Loaded) return;
	for (std::list<_tagTxInfo>::iterator it = this->lResources.begin(); it != this->lResources.end(); it++)
	{
		SAFE_RELEASE(it->lpResource);
	}
	this->Loaded = false;
}
