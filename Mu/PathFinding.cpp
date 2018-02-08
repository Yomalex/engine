#include "stdafx.h"
#include "Mu.h"


CPathFinding::CPathFinding(WORD * Tiles, BYTE StartX, BYTE StartY, BYTE EndX, BYTE EndY)
{
	this->btEndX = EndX;
	this->btEndY = EndY;
	this->TilesAtt = Tiles;
	_PathTile Home = { nullptr, StartX, StartY, 0, 0, 0, 0 };
	ZeroMemory(this->vPath, sizeof(this->vPath));
	this->vPath[0] = new _PathTile();
	memcpy(this->vPath[0], &Home, sizeof(_PathTile));
	this->SubTiles(this->vPath[0]);
}


CPathFinding::~CPathFinding()
{
	for (UINT i = 0; i < MAX_PATH; i++)
	{
		if (this->vPath[i] == nullptr) break;
		delete this->vPath[i];
	}
}

bool CPathFinding::SubTiles(_PathTile * Start)
{
	int F, G, H, minF=25555;
	Start->bClose = 1;
	for (int iY = max(Start->Y - 1, 0); iY <= min(Start->Y + 1, 255); iY++)
	{
		for (int iX = max(Start->X - 1, 0); iX <= min(Start->X + 1, 255); iX++)
		{
			if (this->TilesAtt[iY * 256 + iX] & (~ATTRIBUTE_SAFE)) continue;

			H = (abs(this->btEndX - iX) + abs(this->btEndY - iY))*10;
			G = Start->G + (iX == Start->X || iY == Start->Y) ? 10 : 14;
			F = G + H;
			minF = min(minF, F);
			_PathTile SubTile = { Start, iX, iY, F, G, H, 0 };

			if (H == 0)
			{
				this->GeneratePath(&SubTile);
				return true;
			}

			bool exist = false;
			UINT i;
			for (i = 0; i < MAX_TILES; i++)
			{
				if (this->vPath[i] == nullptr) break;
				if (this->vPath[i]->X == iX && this->vPath[i]->Y == iY)
				{
					exist = true;
					break;
				}
			}
			if (!exist)
			{
				this->vPath[i] = new _PathTile();
				memcpy(this->vPath[i], &SubTile, sizeof(_PathTile));
			}
		}
	}

	for (UINT i = 0; i < MAX_TILES; i++)
	{
		if (this->vPath[i] == nullptr) break;
		if (this->vPath[i]->bClose || minF<this->vPath[i]->F) continue;
		if(this->SubTiles(this->vPath[i]))
			return true;
	}
	return false;
}

void CPathFinding::GeneratePath(_PathTile * End)
{
	std::list<_PathTile> vFinal;
	do
	{
		vFinal.push_front(*End);
		_PathTile * next = End->parent;
		End = next;
	} while (End != nullptr);

	int l = 0;
	this->PathLen = vFinal.size();
	while(!vFinal.empty())
	{
		_PathTile p = vFinal.front();
		this->Path[l][0] = p.X;
		this->Path[l][1] = p.Y;
		l++;
		vFinal.pop_front();
	}
}
