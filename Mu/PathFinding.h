#pragma once

#define MAX_TILES 256*256

struct _PathTile
{
	_PathTile * parent;
	BYTE X, Y;
	BYTE F, G, H;
	BOOL bClose;
};

class CPathFinding
{
	_PathTile * vPath[MAX_TILES];
	BYTE btEndX, btEndY;
	WORD *TilesAtt;
	BYTE Path[MAX_PATH][2];
	int PathLen;
public:
	CPathFinding(WORD * Tiles, BYTE StartX, BYTE StartY, BYTE EndX, BYTE EndY);
	~CPathFinding();
	void GetPath(BYTE * outPath, int * Len)
	{
		memcpy(outPath, this->Path, sizeof(this->Path));
		memcpy(Len, &this->PathLen, sizeof(int));
	}
private:
	bool SubTiles(_PathTile * Start);
	void GeneratePath(_PathTile * End);
};

