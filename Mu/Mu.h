#pragma once

#include "resource.h"
#include "Terrain.h"
#include "LoadSplash.h"
#include "Effect.h"
#include "TexMng.h"
#include "WZText.h"
#include "Interface.h"
#include "GameMain.h"
#include "Item.h"
#include "User.h"
#include "PathFinding.h"

extern CEngine *eng;
extern CUI * Interface;
extern CTerrain * Terrains;
extern CTexMng *TextureManager;
extern CModel * Faces[7];
extern STATUS Game;
extern CWZText Text;
extern CWZMoveReq MoveReq;
extern CWZGate Gate;
extern CWZItem Item;
extern CUser Player;

E_EVENT(OnFrame_NOTLOGGED);
E_EVENT(OnFrame_LOGGED);
E_EVENT(OnFrame_PLAYING);