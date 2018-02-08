#include "stdafx.h"
#include "Game.h"

CGame * Mu;

const TCHAR * szGameFiles[] =
{
	TEXT("./Data/Local/Text.bmd"),
	TEXT("./Data/Local/Movereq.bmd"),
	TEXT("./Data/Gate.bmd"),
};

const TCHAR * szGameConfig[] =
{
	TEXT("./config.ini"),
	TEXT("Video"),
	TEXT("W"),
	TEXT("H"),
	TEXT("FullScreen"),
};

CModel * Faces[MAX_BASIC_CLASS];
int TextFileClass[] = {
	  20,//Dark Wizard
	  21,//Dark Knight
	  22,//Fary Elf
	  23,//Magic Gladiator
	  24,//Dark Lord
	1687,//Summoner

	25,//Soul Master
	26,//Blade Knight
	27,//Muse Elf
	28,//none
	29,//none
	1688,//Blody Summoner

	1668,//Blade Master
	1669,//Grand Master
	1670,//High Elf
	1671,//Duel Master
	1672,//Lord Emperador
	1689,//Dimension Master
};

LRESULT CALLBACK GameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_SETFOCUS:
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Analizar las selecciones de menú:
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Agregar cualquier código de dibujo que use hDC aquí...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BYTE Path[][2] =
{
	{ 247,51 },
	{ 247,24 },
	{ 191,24 },
	{ 191,51 },
};

#define Rad 10.0
void GetCamDir(const BYTE *b0, const BYTE *b1, const float *a, float& cdx, float& cdy, float dx, float dy)
{
	float sy = dx < 0 ? 1 : -1;
	float sx = dy >= 0 ? 1 : -1;
	if (b0[0] == b1[0])
	{
		float x = (b0[0] - a[0]);
		float y = sqrt(Rad*Rad - x*x)*sy;
		float m = sqrt(x*x + y*y);
		cdx = x / m;
		cdy = y / m;
	}
	else if (b0[1] == b1[1])
	{
		float y = b0[1] - a[1];
		float x = sqrt(Rad*Rad - y*y)*sx;
		float m = sqrt(x*x + y*y);
		cdx = x / m;
		cdy = y / m;
	}
}
void ServerAnimation(CEngine * p, float fElapsed)
{
	static float cx = Path[0][0], cy = Path[0][1], speed = 5, dx, dy, cdx, cdy;
	static BYTE dest = 0;
	if (
		((BYTE)cx >= Path[dest][0] && dx>0) ||
		((BYTE)cy >= Path[dest][1] && dy>0) ||
		((BYTE)cx < Path[dest][0] && dx<0) ||
		((BYTE)cy < Path[dest][1] && dy<0)
		)
	{
		cx = Path[dest][0];
		cy = Path[dest][1];
		dest++;
		dest %= 4;
		dx = Path[dest][0] - cx;
		dy = Path[dest][1] - cy;
		float m = sqrt(dx*dx + dy*dy);
		dx /= m;
		dy /= m;
	}
	else if (dx == 0 && dy == 0)
	{
		cx = Path[dest][0];
		cy = Path[dest][1];
		dest++;
		dest %= 4;
		dx = Path[dest][0] - cx;
		dy = Path[dest][1] - cy;
		float m = sqrt(dx*dx + dy*dy);
		dx /= m;
		dy /= m;
	}
	cx += dx*speed*fElapsed;
	cy += dy*speed*fElapsed;
	if ((cx + dx*Rad > Path[dest][0] && dx>0) || (cy + dy*Rad > Path[dest][1] && dy>0) ||
		(cx + dx*Rad < Path[dest][0] && dx<0) || (cy + dy*Rad < Path[dest][1] && dy<0))
	{
		BYTE pDest = (dest + 1) % 4;
		float A[] = { cx,cy };
		GetCamDir(Path[dest], Path[pDest], A, cdx, cdy, dx, dy);
	}
	else
	{
		cdx = dx;
		cdy = dy;
	}
	p->SetCameraPos(cx*CELL_SIZE, cy*CELL_SIZE, 400);
	p->SetCameraDir(cdx, cdy, 0);
}

E_FRAME(OnFrame)
{
	Mu->Draw(fElapsed);
}

E_EVENT(OnLost)
{
	// Device Lost
	Mu->OnLostDev();
}

E_EVENT(OnReset)
{
	// Device Reset
	int W, H;
	p->GetDevInfo(W, H);
	ResetInterface(W, H);
	WritePrivateProfileInt(szGameConfig[1], szGameConfig[2], W, szGameConfig[0]);
	WritePrivateProfileInt(szGameConfig[1], szGameConfig[3], H, szGameConfig[0]);
	WritePrivateProfileInt(szGameConfig[1], szGameConfig[4], p->isFullScreen(), szGameConfig[0]);
	Mu->OnResetDev();
}

E_EVENT(OnCreate)
{
	p->SetFogLinear(0.45, 0.9);
	int w, h;
	p->GetDevInfo(w, h);
	StartInterface(w, h);
	Mu->OnCreateDev();
}

DWORD Crc(const BYTE * pBytes, int len)
{
	DWORD dwCrc = 0;
	for (int i = 0; i < len;  i++)
	{
		dwCrc += *pBytes;
	}

	return dwCrc^len;
}

CGame::CGame(HINSTANCE hInstance) 
{
	this->CurrentTerrain = -1;
	this->ConnectionStatus = CS_NOTLOGGED;
	this->PlayerStatus = PS_DEAD;
	this->Texts.Load(szGameFiles[0]);
	this->MoveReq.Load(szGameFiles[1]);
	this->Gates.Load(szGameFiles[2]);

	int W = GetPrivateProfileInt(szGameConfig[1], szGameConfig[2], 640, szGameConfig[0]);
	int H = GetPrivateProfileInt(szGameConfig[1], szGameConfig[3], 480, szGameConfig[0]);
	bool fs = GetPrivateProfileInt(szGameConfig[1], szGameConfig[4], 0, szGameConfig[0])==1;

	this->EngineD3D = new CEngine(hInstance, TEXT("MU"), W, H, fs, GameWndProc);
	this->Terrains = new CTerrain(74);
	this->TextureManager = new CTexMng();
	this->Loading = new CLoadSplash();

	const TCHAR * szFaces[] =
	{
		TEXT("./Data/Logo/NewFace01.bmd"),
		TEXT("./Data/Logo/NewFace02.bmd"),
		TEXT("./Data/Logo/NewFace03.bmd"),
		TEXT("./Data/Logo/NewFace04.bmd"),
		TEXT("./Data/Logo/NewFace05.bmd"),
		TEXT("./Data/Logo/NewFace06.bmd"),
		//TEXT("./Data/Logo/NewFace01.bmd"),
	};

	for (int i = 0; i < MAX_BASIC_CLASS; i++)
	{
		Faces[i] = new CModel(szFaces[i]);
		Faces[i]->SetName(CTString(this->Texts.Get(TextFileClass[i])->Content));
		this->EngineD3D->AddLoadQueueElement(Faces[i]);
	}

	this->EngineD3D->AddLoadQueueElement(TextureManager);
	this->EngineD3D->AddLoadQueueElement(Terrains);
	this->EngineD3D->UseRightHand();
	this->EngineD3D->SetStatus(Z_Angle, 0);
	this->EngineD3D->SetStatus(X_Angle, 0);
	this->EngineD3D->SetStatus(Z_Speed, 0.15);
	this->EngineD3D->SetStatus(X_Speed, 0.15);
	this->EngineD3D->SetFarNearPlanes(VIEW_TILES*CELL_SIZE*SQRT2*1.5);
	this->EngineD3D->SetCallBack(OnFrame);
	this->EngineD3D->OnCreate = OnCreate;
	this->EngineD3D->OnReset = OnReset;
	//this->EngineD3D->OnLost = OnLost;
	this->EngineD3D->SetLoadScreen(this->Loading);
}

CGame::~CGame()
{
}

bool CGame::Login(const TCHAR * szAccount, const TCHAR * szPassword)
{
	if(this->ConnectionStatus!=CS_NOTLOGGED || _tcslen(szAccount)==FALSE || _tcslen(szPassword)==FALSE) return false;

	this->ConnectionStatus = CS_LOGGED;
	this->Warp(75,0,0);
}

bool CGame::SelectCharacter(int i)
{
	if(i<0||i>4) return false;
	if (this->ConnectionStatus != CS_LOGGED) return false;
	this->WarpGate(4);
	this->ConnectionStatus = CS_PLAYING;
}

int CGame::Run()
{
	return this->EngineD3D->Loop(NULL);
}

void CGame::Warp(int Map, BYTE x, BYTE y)
{
	if (Map != this->CurrentTerrain)
	{
		this->EngineD3D->Release(this->Terrains);
		this->Terrains->SetMap(Map);
		this->CurrentTerrain = Map;
		this->EngineD3D->AddLoadQueueElement(this->Terrains);
		this->EngineD3D->StartLoad();
	}

	this->EngineD3D->SetCameraPos(x*CELL_SIZE, y*CELL_SIZE, 1000);
}

void CGame::WarpGate(int iGate)
{
	const _GateBlock * Gate = this->Gates.Get(iGate);
	this->Warp(
		Gate->Map,
		this->Random(Gate->X1, Gate->X2),
		this->Random(Gate->Y1, Gate->Y2)
		);
}

void CGame::OnCreateDev()
{
	this->TextureManager->Load();
	g_ScrollInfo.Top = this->TextureManager->GetTexture(TEXT("SCROLLTOP"));
	g_ScrollInfo.Bot = this->TextureManager->GetTexture(TEXT("SCROLLBOT"));
	g_ScrollInfo.Mid = this->TextureManager->GetTexture(TEXT("SCROLLMID"));
	g_ScrollInfo.ScrollOn = this->TextureManager->GetTexture(TEXT("SCROLLON"));
	g_ScrollInfo.ScrollOff = this->TextureManager->GetTexture(TEXT("SCROLLOFF"));
}

void CGame::OnLostDev()
{
}

void CGame::OnResetDev()
{
}

void CGame::Draw(float fElapsed)
{
	switch (this->ConnectionStatus)
	{
	case CS_NOTLOGGED:
		this->EngineD3D->SetAmbientLight(D3DCOLOR_XRGB(0, 0, 0));
		ServerAnimation(this->EngineD3D, fElapsed);
		break;
	case CS_LOGGED:
		this->EngineD3D->SetAmbientLight(D3DCOLOR_XRGB(0, 0, 0));
		break;
	case CS_PLAYING:
		this->EngineD3D->SetAmbientLight(D3DXCOLOR(0.5, 0.8, 0.9, 1));
		break;
	}

	this->Terrains->Draw(fElapsed);
}

DWORD CGame::Random(DWORD Min, DWORD Max)
{
	int Delta = (Max-Min);
	if (Delta < 0) return 0;
	return rand() % Delta + Min;
}
