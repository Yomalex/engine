#include "stdafx.h"
#include "Mu.h"

int CurrentMap = 74;
float X, Y;
STATUS Game;
D3DXCOLOR Colors[] =
{
	D3DXCOLOR(0, 0, 0, 1),// Lorencia
	D3DXCOLOR(0, 0, 0, 1),//Dungeon
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// Devias
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// Noria
	D3DXCOLOR(0, 0, 0, 1),// Losttower
	D3DXCOLOR(0.2, 0.32, 0.36, 1),// Exile
	D3DXCOLOR(0.4, 0.6, 0.675, 1),// Arena
	D3DXCOLOR(0.4, 0.6, 0.675, 1),// Atlans
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// Tarkan
	D3DXCOLOR(0.2, 0.32, 0.36, 1),// Exile
	D3DXCOLOR(0.15, 0.2, 0.275, 1),// Icarus
	D3DXCOLOR(0.15, 0.2, 0.275, 1),// ??
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// ??
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),// Valle of Lorencia
	D3DXCOLOR(0, 0, 0, 1),// Loren Deep
	D3DXCOLOR(0, 0, 0, 1),// Empty
	D3DXCOLOR(0, 0, 0, 1),// Aida
	D3DXCOLOR(0, 0, 0, 1),// Crywolf
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),// Kantru
	D3DXCOLOR(0, 0, 0, 1),// Kantru Relics
	D3DXCOLOR(0, 0, 0, 1),// Kantru Tower
	D3DXCOLOR(0, 0, 0, 1),// Gardenia
	D3DXCOLOR(0, 0, 0, 1),// Barracks
	D3DXCOLOR(0, 0, 0, 1),// Refuge
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),// Illusion Temple
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),// Elbeland
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// ??
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// ??
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// ??
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// ??
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// ??
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// Vulcanus
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// Duel Arena
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// ??
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// Event
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// ??
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// ??
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// ??
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// ??
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// ??
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// ??
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// Login
	D3DXCOLOR(0.5, 0.8, 0.9, 1),// CharSelect
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),//Empty
	D3DXCOLOR(0, 0, 0, 1),// Lorencia Market
};

DWORD Random(DWORD dwMin, DWORD dwMax)
{
	int delta = dwMax - dwMin;
	if (delta < 0) delta*=-1;
	if (delta == 0) return dwMin;
	return rand() % delta + dwMin;
}

void WarpGate(int G)
{
	const _GateBlock * pGate = Gate.Get(G);
	Warp(
		pGate->Map+1,
		Random((DWORD)pGate->X1, (DWORD)pGate->X2),
		Random((DWORD)pGate->Y1, (DWORD)pGate->Y2)
		);
}

void Warp(int Map, BYTE x, BYTE y)
{
	if (CurrentMap != Map)
	{
		eng->SetAmbientLight(Colors[Map - 1]);
		CurrentMap = Map;
		eng->Release(Terrains);
		Terrains->SetMap(Map);
		eng->AddLoadQueueElement(Terrains);
		eng->StartLoad();
	}

	X = x*CELL_SIZE;
	Y = y*CELL_SIZE;
}