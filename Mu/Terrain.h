#pragma once
#include "Model.h"
#include "WZModel.h"
#include "Effect.h"
#define CELL_SCALE 0.015f
#define CELL_SIZE 100.0f
#define VIEW_TILES 64
#define SQRT2 1.414213562
#define RADIUS CELL_SIZE*VIEW_TILES*0.5

#define BASE 256
#define TERRAIN_SIZE 65536
#define MAP_FILE_SIZE 196610//65536*3+2
#define ATT_FILE_129KB_SIZE 131076//65536*2+4
#define ATT_FILE_65KB_SIZE 65540//65536+4
#define HEIGHT_HEAD_SIZE 1082
#define HEIGHT_BUFFER_SIZE 65536
#define OZJ_HEAD_SIZE 24
#define LIGHT_MAP_SIZE 65536

#define ATTRIBUTE_HUNTZ			0x00
#define ATTRIBUTE_SAFE          (0x01<<0)
#define ATTRIBUTE_BREAK         (0x01<<2)
#define ATTRIBUTE_UNVISIBLE     (0x01<<3)
#define ATTRIBUTE_GRASS			0x08
#define MAX_MODELS 256

#define MAKE_OBJID(map,objid) (map*MAX_MODELS+objid)

class CTerrain :
	public CEngineElement
{
#pragma pack(push,1)
	struct TerrainObjectInfo
	{
		short Index;
		D3DXVECTOR3 Position;//float x, y, z;//Position
		D3DXVECTOR3 Rotation;//float rx, ry, rz;//Rotation
		float Size;
	};
#pragma pack(pop)
	struct TerrainObject
	{
		WORD Model;
		D3DXVECTOR3 Position;
		D3DXMATRIX mWorld;
		D3DXCOLOR Diffuse;
		float DrawTime;
	};
	struct TVertex
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Normal;
		D3DCOLOR Color;
		D3DXVECTOR2 Texture;
		D3DXVECTOR3 TextureNumber;
	};
	HRESULT hr;
	Plane * RPlane;
public:
	D3DXVECTOR3 Center;

	CTerrain(int Map);
	~CTerrain();

	void SetMap(int iMap);
	bool Load();
	void Draw();
	void Release();
	float GetHeight(int x, int y);
	BOOL TestRay(D3DXVECTOR3 *vin);
	BOOL GetPoint(D3DXVECTOR3 *vin);

	WORD Attributes[TERRAIN_SIZE];
private:
	BYTE Layer[3][TERRAIN_SIZE];
	BYTE Heights[TERRAIN_SIZE];
	D3DXCOLOR Light[TERRAIN_SIZE];
	D3DXVECTOR3 Normals[TERRAIN_SIZE];
	int Map;
	std::list<TerrainObject> vObjects;
	PDIRECT3DVERTEXBUFFER9 VBTerrain;
	PDIRECT3DINDEXBUFFER9 IBTerrain;
	PDIRECT3DTEXTURE9 TXTerrain[15];
	PDIRECT3DVERTEXDECLARATION9 VDTerrain;
	LPD3DXEFFECT lpEffect;
	WZModel * Model[MAX_MODELS];
	CEffect * FxMng;

	bool LoadLayers(const TCHAR * szFile);
	bool LoadAttributes(const TCHAR * szFile);
	bool LoadHeight(const TCHAR * szFile);
	bool LoadLight(const TCHAR * szFile);
	bool LoadObjects(const TCHAR * szFile);
};

