#pragma once
#include "Model.h"
#define MAX_INDEX 512
#define MAX_TYPE 16

enum _Types
{
	IT_SWORD,
	IT_AXE,
	IT_MACE,
	IT_SPEAR,
	IT_BOW,
	IT_STAFF,
	IT_SHIELD,
	IT_HELM,
	IT_ARMOR,
	IT_PANT,
	IT_GLOVE,
	IT_BOOT,
	IT_WING
};

void InitializeItem();

class CItem
{
	CModel * pModel;
public:
	CItem();
	~CItem();
};

