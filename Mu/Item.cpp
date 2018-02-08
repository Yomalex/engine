#include "stdafx.h"
#include "Mu.h"

int ItemCount[MAX_TYPE];

CModel * mItems[MAX_INDEX*MAX_TYPE];
//
const TCHAR * szItems[MAX_TYPE] =
{
	TEXT("./Data/Item/Sword%02d.bmd"),
	TEXT("./Data/Item/Axe%02d.bmd"),
	TEXT("./Data/Item/Mace%02d.bmd"),
	TEXT("./Data/Item/Spear%02d.bmd"),
	TEXT("./Data/Item/Bow%02d.bmd"),
	TEXT("./Data/Item/Staff%02d.bmd"),
	TEXT("./Data/Item/Shield%02d.bmd"),
	TEXT("./Data/Player/HelmMale%02d.bmd"),
	TEXT("./Data/Player/ArmorMale%02d.bmd"),
	TEXT("./Data/Player/PantMale%02d.bmd"),
	TEXT("./Data/Player/GloveMale%02d.bmd"),
	TEXT("./Data/Player/BootMale%02d.bmd"),
	TEXT("./Data/Item/Wing%02d.bmd"),
	TEXT("./Data/Item/"),
};

void InitializeItem()
{
	ItemCount[IT_SWORD] = 82;
	ItemCount[IT_AXE] = 29;
	ItemCount[IT_MACE] = 46;
	ItemCount[IT_SPEAR] = 31;
	ItemCount[IT_BOW] = 33;
	ItemCount[IT_STAFF] = 43;
	ItemCount[IT_SHIELD] = 52;
	ItemCount[IT_HELM] = 113;
	ItemCount[IT_ARMOR] = 113;
	ItemCount[IT_PANT] = 113;
	ItemCount[IT_GLOVE] = 113;
	ItemCount[IT_BOOT] = 113;
	ItemCount[IT_WING] = 16;

	TCHAR szFile[MAX_PATH];
	UINT Number;
	for (int type = 0; type < MAX_TYPE; type++)
	{
		for (int index = 0; index < ItemCount[type]; index++)
		{
			_stprintf_s(szFile, szItems[type], index+1);
			Number = type*MAX_INDEX + index;

			mItems[Number] = nullptr;
			if (Item.Get(Number)->X == 0||!file_exists(szFile)) continue;

			mItems[Number] = new CModel(szFile);
			mItems[Number]->SetName(CTString(Item.Get(Number)->szName));
			mItems[Number]->AsociateEngine(eng);
		}
	}

	Number = 0;
}

CItem::CItem()
{
}


CItem::~CItem()
{
}
