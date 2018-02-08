#pragma once
#include "WZen.h"

#pragma pack(push,1)
struct _TextBlock
{
	DWORD dwIndex;
	DWORD dwSize;
	char Content[1];
};
struct _MoveReqBlock
{
	DWORD dwIndex;//4
	char Name[32];//32
	char NameWarp[32];//32
	DWORD dwLevel;//4
	DWORD dwZen;//4
	DWORD dwGate;//4
};
struct _GateBlock
{
	BYTE Flag;
	BYTE Map;
	BYTE X1, Y1;
	BYTE X2, Y2;
	WORD Target;
	WORD Dir;
	WORD LevelMin;
	WORD LevelMax;
};
struct _ItemBlock
{
	//WORD wIndex;
	char szName[30];
	WORD TwoHandos;
	WORD Level;
	BYTE Slot;
	BYTE Skill;
	BYTE X, Y;
	BYTE DmgMin;
	BYTE DmgMax;
	BYTE DefRate;
	BYTE Def;
	BYTE MagDef;
	BYTE Speed;
	BYTE Walk;
	BYTE Durability;
	BYTE MagicDur;
	BYTE MagPower;
	WORD Str;
	WORD Agi;
	WORD Ene;
	WORD Vit;
	WORD Cmd;
	WORD LevelReq;
	DWORD Value;
	DWORD Zen;
	BYTE Type;
	BYTE Class[6];
	BYTE Resistance[7];
	WORD Unk39;
};
#pragma pack(pop)

bool MapFile(const TCHAR * szFile, DWORD * FileSize);
void UnMapFile();

template <typename CWZType>
class CWZBMDText
{
public:
	CWZBMDText() {
		ZeroMemory(this->vElements, sizeof(this->vElements));
	}
	CWZBMDText(const TCHAR * szFile) {
		ZeroMemory(this->vElements, sizeof(this->vElements));
		this->Load(szFile);
	}
	~CWZBMDText(){
		for (int i = 0; i < this->iCount; i++)
		{
			free(this->vElements[i]);
		}
	}

	virtual bool Load(const TCHAR * szFile) = 0;
	const CWZType * Get(int Number) {
		if (Number < 0 && Number >= this->iCount) return nullptr;
		return this->vElements[Number];
	}
	int Count() { return this->iCount; }
protected:
	CWZType * vElements[65535];
	int iCount;
};

class CWZText : public CWZBMDText<_TextBlock>
{
	TCHAR szLine[1024];
public:
	bool Load(const TCHAR * szFile);
	const TCHAR * String(int Index)
	{
		_tcscpy_s(this->szLine, CTString(this->Get(Index)->Content));
		return (const TCHAR*)this->szLine;
	}
};

class CWZMoveReq : public CWZBMDText<_MoveReqBlock>
{
public:
	bool Load(const TCHAR * szFile);
};

class CWZGate : public CWZBMDText<_GateBlock>
{
public:
	bool Load(const TCHAR * szFile);
};

class CWZItem : public CWZBMDText<_ItemBlock>
{
public:
	bool Load(const TCHAR * szFile);
};

bool file_exists(const TCHAR * szFile);