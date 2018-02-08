#pragma once
class CGame;
#include "WZText.h"
#include "Terrain.h"
#include "TexMng.h"
#include "LoadSplash.h"
#include "Interface.h"
#include "Console.h"
#include "Client.h"

#define MAX_BASIC_CLASS 6

class CGame
{
	enum _enumConnection
	{
		CS_NOTLOGGED,
		CS_LOGGED,
		CS_PLAYING
	};

	enum _enumPlayer
	{
		PS_DEAD,
		PS_ALIVE,
		PS_WARPING,
	};

	struct _SVList
	{
		TCHAR szName[100];
		DWORD dwIP;
		WORD wPort;
		DWORD dwMax;
		DWORD dwLoad;
	};
	struct _SVGroup
	{
		TCHAR szName[100];
		std::vector<_SVList> vServers;
	};

public:
	CGame(HINSTANCE hInstance);
	~CGame();


	bool Login(const TCHAR * szAccount, const TCHAR * szPassword);
	bool SelectCharacter(int i);
	int Run();
	void Warp(int Map, BYTE x, BYTE y);
	void WarpGate(int Gate);
	void Discconect(int mode);
	CUI * GUI() { return this->EngineD3D->GUI; }
	LPDIRECT3DTEXTURE9 GetTexture(const TCHAR * Name) { return this->TextureManager->GetTexture(Name); }

	CEngine * GetEngine() { return this->EngineD3D; }
private:
	void OnCreateDev();
	void OnLostDev();
	void OnResetDev();
	void Draw(float fElapsed);

	DWORD Random(DWORD Min, DWORD Max);
	CEngine * EngineD3D;
	//CConsole Console;
	CWZText Texts;
	CWZGate Gates;
	CWZMoveReq MoveReq;
	CTerrain * Terrains;
	CTexMng * TextureManager;
	CLoadSplash * Loading;

	// Game
	_enumConnection ConnectionStatus;
	_enumPlayer PlayerStatus;
	int CurrentTerrain;

	std::vector<_SVGroup> vGroups;

	//firendly
	friend E_FRAME(OnFrame);
	friend E_EVENT(OnLost);
	friend E_EVENT(OnReset);
	friend E_EVENT(OnCreate);
	friend LRESULT CALLBACK GameWndProc(HWND, UINT, WPARAM, LPARAM);
	friend CConsole;
	friend CInterface;
};

extern CGame * Mu;

