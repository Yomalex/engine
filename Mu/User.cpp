#include "stdafx.h"
#include "Model.h"
#include "Mu.h"

CUser Player(0);

const TCHAR * szBase[][2] = {
	{TEXT("./Data/Player/HelmClass%02d.bmd"), TEXT("Helm of Class %d")},
	{TEXT("./Data/Player/ArmorClass%02d.bmd"), TEXT("Armor of Class %d") },
	{TEXT("./Data/Player/GloveClass%02d.bmd"), TEXT("Glove of Class %d")},
	{TEXT("./Data/Player/PantClass%02d.bmd"), TEXT("Pant of Class %d")},
	{TEXT("./Data/Player/BootClass%02d.bmd"), TEXT("Boot of Class %d")},
};

Class g_BaseClases[3][6];
CModel * g_Player;
void UpdatePos(CModel*caller)
{
	Player.Pos = D3DXVECTOR3(Player.Position.x, Player.Position.y, 0);
}
void InitializeClasses()
{
	g_Player = new CModel(TEXT("./Data/Player/Player.bmd"),2);
	g_Player->SetName(TEXT("Player Animations"));
	g_Player->OnAnimationEnd = UpdatePos;
	eng->AddLoadQueueElement(g_Player);

	TCHAR szName[MAX_PATH];
	for (int i = 0; i < 18; i++)
	{
		int c = i % 6;
		int j = i / 6;
		if (!(j == 1 && (c == 4 || c == 5)))
		{
			_stprintf_s(szName, szBase[0][0], (c + 1) + ((j) ? (j + 1) * 100 : 0));
			g_BaseClases[j][c].Head = new CModel(szName);

			_stprintf_s(szName, szBase[1][0], (c + 1) + ((j) ? (j + 1) * 100 : 0));
			g_BaseClases[j][c].BodyUp = new CModel(szName);

			_stprintf_s(szName, szBase[2][0], (c + 1) + ((j) ? (j + 1) * 100 : 0));
			g_BaseClases[j][c].Arms = new CModel(szName);

			_stprintf_s(szName, szBase[3][0], (c + 1) + ((j) ? (j + 1) * 100 : 0));
			g_BaseClases[j][c].BodyDown = new CModel(szName);

			_stprintf_s(szName, szBase[4][0], (c + 1) + ((j) ? (j + 1) * 100 : 0));
			g_BaseClases[j][c].Foots = new CModel(szName);

			_stprintf_s(szName, szBase[0][1], (c + 1) + ((j) ? (j + 1) * 100 : 0));
			g_BaseClases[j][c].Head->SetName(szName);

			_stprintf_s(szName, szBase[1][1], (c + 1) + ((j) ? (j + 1) * 100 : 0));
			g_BaseClases[j][c].BodyUp->SetName(szName);

			_stprintf_s(szName, szBase[2][1], (c + 1) + ((j) ? (j + 1) * 100 : 0));
			g_BaseClases[j][c].Arms->SetName(szName);

			_stprintf_s(szName, szBase[3][1], (c + 1) + ((j) ? (j + 1) * 100 : 0));
			g_BaseClases[j][c].BodyDown->SetName(szName);

			_stprintf_s(szName, szBase[4][1], (c + 1) + ((j) ? (j + 1) * 100 : 0));
			g_BaseClases[j][c].Foots->SetName(szName);

			eng->AddLoadQueueElement(g_BaseClases[j][c].Head);
			eng->AddLoadQueueElement(g_BaseClases[j][c].BodyUp);
			eng->AddLoadQueueElement(g_BaseClases[j][c].Arms);
			eng->AddLoadQueueElement(g_BaseClases[j][c].BodyDown);
			eng->AddLoadQueueElement(g_BaseClases[j][c].Foots);
		}
		else
		{
			ZeroMemory(&g_BaseClases[j][c], sizeof(Class));
		}
	}
}

CUser::CUser(int Class)
{
	Frame = 0;
	this->SetClass(Class);
	ZeroMemory(&this->Equipament, sizeof(this->Equipament));
}


CUser::~CUser()
{
}

void CUser::SetPath(BYTE * inPath, int Len)
{
	this->CurPath = 1;
	this->LenPath = Len;
	ZeroMemory(this->Path, sizeof(this->Path));
	memcpy(this->Path, inPath, sizeof(BYTE)*2*Len);
}

void CUser::SetClass(int Class)
{
	this->BaseClass = (Class & 0x10) >> 4;
	this->ChangeUp = Class & 0x01;
	Base = g_BaseClases[this->ChangeUp][this->BaseClass];
	if (Base.Head != nullptr) Base.Head->SetAnimation(g_Player);
	if (Base.Arms != nullptr) Base.Arms->SetAnimation(g_Player);
	if (Base.BodyDown != nullptr) Base.BodyDown->SetAnimation(g_Player);
	if (Base.BodyUp != nullptr) Base.BodyUp->SetAnimation(g_Player);
	if (Base.Foots != nullptr) Base.Foots->SetAnimation(g_Player);
}

void CUser::SetInventory(CItem * Inv)
{
}

void CUser::SetSlot(int Slot, CItem * Item)
{
}

MDEVENT(PlayerAnimationEnd)
{
	CUser * pthis = (CUser*)g_Player->Param;
	
	pthis->Pos = D3DXVECTOR3(pthis->Position.x, pthis->Position.y, 0);
	pthis->Frame = 0.0f;
}

void CUser::Draw(double fElapsed)
{
	this->Walking(fElapsed);
	g_Player->OnAnimationEnd = PlayerAnimationEnd;
	g_Player->Param = (LPARAM)this;
	g_Player->SetFrame(this->AniNumber,(WORD)this->Frame);
	D3DXMATRIX Trans, Rot;
	D3DXMatrixTranslation(&Trans, this->Pos.x+CELL_SIZE*0.5, this->Pos.y+ CELL_SIZE*0.5, this->Position.z);
	D3DXMatrixRotationZ(&Rot, this->rotation);

	Rot = Rot*Trans;

	eng->PushWorld(&Rot);
	if (Base.Arms != nullptr)		this->Base.Arms->Draw();
	if (Base.BodyDown != nullptr)	this->Base.BodyDown->Draw();
	if (Base.BodyUp != nullptr)		this->Base.BodyUp->Draw();
	if (Base.Foots != nullptr)		this->Base.Foots->Draw();
	if (Base.Head != nullptr)		this->Base.Head->Draw();
	eng->PopWorld();

	this->Frame += 24.0*fElapsed;
}

void CUser::Walking(float fElapsed)
{
	this->bWalking = true;
	if (this->CurPath >= this->LenPath - 1)
	{
		this->Pos = D3DXVECTOR3(this->Position.x, this->Position.y, 0);
		if (this->AniNumber != PA_MALE_STAY) { this->Frame = 0; }
		this->AniNumber = PA_MALE_STAY;
		this->bWalking = false;
		return;
	}
	if (this->AniNumber != PA_MALE_WALKING) { this->Frame = 0; }
	this->AniNumber = PA_MALE_WALKING;
	float dx = (float)(this->Path[this->CurPath][0] - (BYTE)(this->Position.x / CELL_SIZE));
	float dy = (float)(this->Path[this->CurPath][1] - (BYTE)(this->Position.y / CELL_SIZE));
	float m = sqrt(dx*dx + dy*dy);
	if (m != 0.0f) { dx /= m; dy /= m; }

	this->Position.x += dx*fElapsed*4.0* CELL_SIZE;
	this->Position.y += dy*fElapsed*4.0* CELL_SIZE;
	if (
		((BYTE)(this->Position.x / CELL_SIZE) == this->Path[this->CurPath][0]) &&
		((BYTE)(this->Position.y / CELL_SIZE) == this->Path[this->CurPath][1])
		)
	{
		this->CurPath++;
	}
	//Player.SetPosition(X, Y, 0);
	float fCurAngle = atan2(dy, dx) + D3DX_PI / 2.0;
	if (fCurAngle != this->rotation)
	{
		this->Pos = D3DXVECTOR3(this->Position.x, this->Position.y, 0);
		this->rotation = fCurAngle;
		this->Frame = 0.0f;
	}
}
