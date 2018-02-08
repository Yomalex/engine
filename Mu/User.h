#pragma once
#include "Model.h"

struct Class {
	CModel * Head;
	CModel * BodyUp;
	CModel * Arms;
	CModel * Foots;
	CModel * BodyDown;
};

enum _PlayerAnim
{
	PA_NONE,
	PA_MALE_STAY,
	PA_FEMALE_STAY,
	PA_FEMALE_STAY_HUNT,
	PA_MALE_STAY_HUNT_SWORD_SHIELD,
	PA_MALE_STAY_HUNT_TWO_HANDS_SWORD,
	PA_MALE_STAY_HUNT_TWO_HANDS_SPEAR,
	PA_MALE_STAY_HUNT_STAFF_SHIELD,
	PA_FEMALE_STAY_HUNT_BOW,
	PA_FEMALE_STAY_HUNT_CROSSBOW,
	PA_FEMALE_STAY_HUNT_UNK1,
	PA_MALE_STAY_HUNT_FLY_SWORD_SHILD,
	PA_MALE_STAY_HUNT_FLY_STAFF_SHILD,
	PA_MALE_STAY_HUNT_MOUNT,
	PA_MALE_RUNING_HUNT_MOUNT,
	PA_MALE_WALKING,
	PA_FEMALE_WALKING,
	PA_MALE_WALKING_HUNT_SWORD_SHILD,
	PA_MALE_WALKING_HUNT_TWO_HANDS_SWORD,
	PA_MALE_WALKING_HUNT_TWO_HANDS_SPEAR,
	PA_MALE_WALKING_HUNT_STAFF,
	PA_FEMALE_WALKING_HUNT_BOW,
	PA_FEMALE_WALKING_HUNT_CROSSBOW,
	PA_FEMALE_WALKING_HUNT_,
	PA_CHANGEUP,
	PA_MALE_RUNING_SWORD_SHILD,
};

class CUser
{
	struct Class Base;

	struct Equip {
		CItem * Helm;
		CItem * Armor;
		CItem * Gloves;
		CItem * Pants;
		CItem * Boots;
		CItem * Rings[2];
		CItem * Pendant;
		CItem * Wings;
		CItem * Pet;
	} Equipament;

	float rotation;
	float Frame;
	BYTE BaseClass, ChangeUp;
	int CurPath;
	int LenPath;
	BYTE Path[MAX_PATH][2];
	WORD AniNumber;

	bool bWalking;
public:
	D3DXVECTOR3 Position, Pos;

	CUser(int Class);
	~CUser();

	void SetPath(BYTE * inPath, int Len);
	void SetPosition(float x, float y, float z) { this->Position = D3DXVECTOR3(x, y, z); }
	void SetRotation(float angleZ) { rotation = angleZ; }
	void SetClass(int Class);
	void SetInventory(CItem * Inv);
	void SetSlot(int Slot, CItem * Item);
	void Draw(double fElapsed);
private:
	void Walking(float fElapsed);
	friend MDEVENT(PlayerAnimationEnd);
};

void InitializeClasses();
extern CUser Player;