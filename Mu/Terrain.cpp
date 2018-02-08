#include "stdafx.h"
#include "Terrain.h"
#include "WZen.h"

//#define EFFECT_LIST 3

struct _tagFxList
{
	const TCHAR * Name;
	BYTE Cols;
	BYTE Rows;
}
FxList[] = {
	{ TEXT("./Data/Effect/Fire01.ozj"), 4, 1 },
	{ TEXT("./Data/Effect/Fire03.ozj"), 4, 1 },
	{ TEXT("./Data/Effect/Fire05.ozj"), 4, 1 },
	{ nullptr, 0, 0 },
};

const TCHAR * FxFirehik[3] = 
{
	TEXT("./Data/Effect/firehik01.OZJ"),
	TEXT("./Data/Effect/firehik02.OZJ"),
	TEXT("./Data/Effect/firehik03.OZJ"),
};

const TCHAR* szTerrainMaterial[15][2] = {
	{ TEXT("TileGrass01.ozj"), TEXT("Grass1") },
	{ TEXT("TileGrass02.ozj"), TEXT("Grass2") },
	{ TEXT("TileGround01.ozj"), TEXT("Ground1") },
	{ TEXT("TileGround02.ozj"), TEXT("Ground2") },
	{ TEXT("TileGround03.ozj"), TEXT("Ground3") },
	{ TEXT("TileWater01.ozj"), TEXT("Water1") },
	{ TEXT("TileWood01.ozj"), TEXT("Wood1") },
	{ TEXT("TileRock01.ozj"), TEXT("Rock1") },
	{ TEXT("TileRock02.ozj"), TEXT("Rock2") },
	{ TEXT("TileRock03.ozj"), TEXT("Rock3") },
	{ TEXT("TileRock04.ozj"), TEXT("Rock4") },
	{ TEXT("TileRock05.ozj"), TEXT("Rock5") },
	{ TEXT("TileGrass03.ozj"), TEXT("Grass2") },
	{ TEXT("TileRock06.ozj"), TEXT("Rock6") },
	{ TEXT("TileRock07.ozj"), TEXT("Rock7") }/*,
	{ TEXT("TileRock08.ozj"), TEXT("Rock8") }//Base Texture*/
};

const TCHAR* szTerrainObject[] =
{
 TEXT("Tree01"),
 TEXT("Tree02"),
 TEXT("Tree03"),
 TEXT("Tree04"),
 TEXT("Tree05"),
 TEXT("Tree06"),
 TEXT("Tree07"),
 TEXT("Tree08"),
 TEXT("Tree09"),
 TEXT("Tree10"),
 TEXT("Tree11"),//10
 TEXT("Tree12"),
 TEXT("Tree13"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("Grass01"),//20
 TEXT("Grass02"),
 TEXT("Grass03"),
 TEXT("Grass04"),
 TEXT("Grass05"),
 TEXT("Grass06"),
 TEXT("Grass07"),
 TEXT("Grass08"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("Stone01"),//30
 TEXT("Stone02"),
 TEXT("Stone03"),
 TEXT("Stone04"),
 TEXT("Stone05"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("StoneStatue01"),//40
 TEXT("StoneStatue02"),
 TEXT("StoneStatue03"),
 TEXT("SteelStatue01"),
 TEXT("Tomb01"),
 TEXT("Tomb02"),
 TEXT("Tomb03"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("FireLight01"),//50
 TEXT("FireLight02"),
 TEXT("BonFire01"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("DoungeonGate01"),
 TEXT("MerchantAnimal01"),
 TEXT("MerchantAnimal02"),
 TEXT("TreasureDrum01"),
 TEXT("TreasureChest01"),
 TEXT("Ship01"),//60
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("SteelWall01"),
 TEXT("SteelWall02"),
 TEXT("SteelWall03"),
 TEXT("SteelDoor01"),
 TEXT("StoneWall01"),
 TEXT("StoneWall02"),//70
 TEXT("StoneWall03"),
 TEXT("StoneWall04"),
 TEXT("StoneWall05"),
 TEXT("StoneWall06"),
 TEXT("StoneMuWall01"),
 TEXT("StoneMuWall02"),
 TEXT("StoneMuWall03"),
 TEXT("StoneMuWall04"),
 TEXT("NULL"),
 TEXT("Bridge01"),//80
 TEXT("Fence01"),
 TEXT("Fence02"),
 TEXT("Fence03"),
 TEXT("Fence04"),
 TEXT("BridgeStone01"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("StreetLight01"),//90
 TEXT("Cannon01"),
 TEXT("Cannon02"),
 TEXT("Cannon03"),
 TEXT("NULL"),
 TEXT("Curtain01"),
 TEXT("Sign01"),
 TEXT("Sign02"),
 TEXT("Carriage01"),
 TEXT("Carriage02"),
 TEXT("Carriage03"),//100
 TEXT("Carriage04"),
 TEXT("Straw01"),
 TEXT("Straw02"),
 TEXT("NULL"),
 TEXT("Waterspout01"),//105
 TEXT("Well01"),
 TEXT("Well02"),
 TEXT("Well03"),
 TEXT("Well04"),
 TEXT("Hanging01"),//110
 TEXT("Stair01"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("House01"),
 TEXT("House02"),
 TEXT("House03"),
 TEXT("House04"),
 TEXT("House05"),
 TEXT("Tent01"),//120
 TEXT("HouseWall01"),
 TEXT("HouseWall02"),
 TEXT("HouseWall03"),
 TEXT("HouseWall04"),
 TEXT("HouseWall05"),
 TEXT("HouseWall06"),
 TEXT("HouseEtc01"),
 TEXT("HouseEtc02"),
 TEXT("HouseEtc03"),
 TEXT("Light01"),//130
 TEXT("Light02"),
 TEXT("Light03"),
 TEXT("PoseBox01"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("Furniture01"),
 TEXT("Furniture02"),
 TEXT("Furniture03"),
 TEXT("Furniture04"),
 TEXT("Furniture05"),
 TEXT("Furniture06"),
 TEXT("Furniture07"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("NULL"),
 TEXT("Candle01"),
 TEXT("Beer01"),
 TEXT("Beer02"),
 TEXT("Beer03")
};

CTerrain::CTerrain(int m):Map(m),VBTerrain(nullptr),IBTerrain(nullptr),VDTerrain(nullptr)
{
	this->Loaded = false;
	this->FxMng = new CEffect();
	this->SetName(TEXT("Terrain"));
	for (int i = 0; i < 15; i++) this->TXTerrain[i] = nullptr;
	for (int i = 0; i < MAX_MODELS; i++) this->Model[i] = nullptr;
}

CTerrain::~CTerrain()
{
	this->Release();
	delete this->FxMng;
}

void CTerrain::SetMap(int iMap)
{
	this->Map = iMap;
}

bool CTerrain::Load()
{
	if (this->Loaded) return true;
	for (UINT i = 0; i < 10; i++)
	{
		if (FxList[i].Name == nullptr) break;
		this->FxMng->SetEffect(i, FxList[i].Name, FxList[i].Cols, FxList[i].Rows);
	}

	this->FxMng->SetVelocity(0, D3DXVECTOR3(14, 14, 80));
	this->FxMng->SetMaxParticles(0, 4);
	this->FxMng->SetLife(0, 800);

	this->FxMng->SetEffect(10,FxFirehik,3);
	this->FxMng->SetVelocity(10, D3DXVECTOR3(14, 14, 80));
	this->FxMng->SetMaxParticles(10, 3);
	this->FxMng->SetLife(10, 600);

	this->FxMng->SetEffect(11, TEXT("./Data/Effect/smoke01.OZJ"), 1, 1);
	this->FxMng->SetVelocity(11, D3DXVECTOR3(40, 40, 40));
	this->FxMng->SetMaxParticles(11, 20);
	this->FxMng->SetLife(11, 3000);

	this->FxMng->SetEffect(12, TEXT("./Data/Effect/waterFall2.OZJ"), 1, 1);
	this->FxMng->SetVelocity(12, D3DXVECTOR3(0, 0, 800));
	this->FxMng->SetGravity(12, D3DXVECTOR3(0, 0, -980*2));
	this->FxMng->SetMaxParticles(12, 40);
	this->FxMng->SetLife(12, 500);

	this->GetEngine()->AddLoadQueueElement(this->FxMng);
	TCHAR szFile[MAX_PATH];

	_stprintf_s(szFile, TEXT("./Data/World%d/EncTerrain%d.obj"), this->Map, this->Map);
	if (!this->LoadObjects(szFile)) { this->Release(); return false; }

	_stprintf_s(szFile, TEXT("./Data/World%d/TerrainHeight.OZB"), this->Map);
	if (!this->LoadHeight(szFile)) { this->Release(); return false; }

	_stprintf_s(szFile, TEXT("./Data/World%d/TerrainLight.OZJ"), this->Map);
	if (!this->LoadLight(szFile)) { this->Release(); return false; }

	_stprintf_s(szFile, TEXT("./Data/World%d/EncTerrain%d.map"), this->Map, this->Map);
	if (!this->LoadLayers(szFile)) { this->Release(); return false; }

	_stprintf_s(szFile, TEXT("./Data/World%d/EncTerrain%d.att"), this->Map, this->Map);
	if (!this->LoadAttributes(szFile)) { this->Release(); return false; }

	for (int i = 0; i < 15; i++)
	{
		_stprintf_s(szFile, TEXT("./Data/World%d/%s"), this->Map, szTerrainMaterial[i][0]);
		this->hr = WebZenTexture(this->GetDevice(), szFile, &this->TXTerrain[i]);
		if (FAILED(hr))
		{
			this->GetEngine()->Log(TEXT("Can't load material %s"), szTerrainMaterial[i][0]);
		}
	}

	this->hr = this->GetDevice()->CreateVertexBuffer(
		sizeof(TVertex) * 4 * 255 * 255,
		0,
		0,
		D3DPOOL_MANAGED,
		&this->VBTerrain,
		NULL
		);

	if (FAILED(hr))
	{
		this->Release();
		this->GetEngine()->Log(TEXT("Can't create VertexBuffer: %08x"), hr);
		return false;
	}

	TVertex * pVertex;
	this->VBTerrain->Lock(0, 0, (void**)&pVertex, 0);

	D3DXVECTOR3 XAxis, YAxis, Normal;
	D3DCOLOR Color;
	float UVMap[] = { 0.0f, 0.5f, 1.0f, 0.5f };
	bool WaterPlane = false;
	WORD ID;
	for (int y = 0; y < 256; y++)
	{
		for (int x = 0; x < 256; x++)
		{
			pVertex->Pos = D3DXVECTOR3(x*CELL_SIZE+50.0,y*CELL_SIZE+50.0,this->GetHeight(x,y));
			if (x>0 && x < 255)//XAxis
			{
				XAxis = D3DXVECTOR3(CELL_SIZE, 0, this->GetHeight(x + 1, y) - this->GetHeight(x - 1, y));
			}
			else if (x>0)
			{
				XAxis = D3DXVECTOR3(CELL_SIZE, 0, this->GetHeight(x, y) - this->GetHeight(x - 1, y));
			}
			else
			{
				XAxis = D3DXVECTOR3(CELL_SIZE, 0, this->GetHeight(x + 1, y) - this->GetHeight(x, y));
			}
			if (y > 0 && y < 255)
			{
				YAxis = D3DXVECTOR3(0, CELL_SIZE, this->GetHeight(x, y + 1) - this->GetHeight(x, y - 1));
			}
			else if (y>0)
			{
				YAxis = D3DXVECTOR3(0, CELL_SIZE, this->GetHeight(x, y) - this->GetHeight(x, y - 1));
			}
			else
			{
				YAxis = D3DXVECTOR3(0, CELL_SIZE, this->GetHeight(x + 1, y) - this->GetHeight(x, y));
			}
			D3DXVec3Cross(
				&Normal,
				&XAxis,
				&YAxis
				);
			
			D3DXVec3Normalize(&pVertex->Normal, &Normal);
			ID = x + y * 256;
			this->Normals[ID] = pVertex->Normal;
			Color = this->Light[ID];
			pVertex->Color = (Color &0x00FFFFFF)|(this->Layer[2][ID]<<24);
			pVertex->Texture = D3DXVECTOR2((float)x/2.0f, (float)y / 2.0f);
			pVertex->TextureNumber.x = this->Layer[0][ID];
			pVertex->TextureNumber.y = this->Layer[1][ID];
			pVertex->TextureNumber.z = (this->Attributes[ID] & ATTRIBUTE_UNVISIBLE) ? 0 : 1;
			if (!WaterPlane && this->Layer[0][ID]==5 && !(this->Attributes[ID] & ATTRIBUTE_UNVISIBLE))
			{
				WaterPlane = true;
				this->RPlane = this->GetEngine()->AddReflectionPlane(D3DXVECTOR3(x*CELL_SIZE, y*CELL_SIZE, this->GetHeight(x, y)), this->Normals[ID]);
			}
			pVertex++;
		}
	}

	this->VBTerrain->Unlock();

	WORD Indices[] = {0,256,257,257,1,0};
	this->hr = this->GetDevice()->CreateIndexBuffer(
		sizeof(Indices),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&this->IBTerrain,
		NULL
		);

	if (FAILED(hr))
	{
		this->Release();
		this->GetEngine()->Log(TEXT("Can't create IndexBuffer: %08x"), hr);
		return false;
	}
	
	void * pVoid;
	this->IBTerrain->Lock(0, 0, &pVoid, 0);
	memcpy(pVoid, Indices, sizeof(Indices));
	this->IBTerrain->Unlock();

	D3DVERTEXELEMENT9 delc[] =
	{
		{ 0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION },
		{ 0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL },
		{ 0,24,D3DDECLTYPE_D3DCOLOR,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_COLOR },
		{ 0,28,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0,36,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 1 },
		D3DDECL_END()
	};
	this->hr = this->GetDevice()->CreateVertexDeclaration(delc, &this->VDTerrain);

	if (FAILED(hr))
	{
		this->Release();
		this->GetEngine()->Log(TEXT("Can't create VertexDeclaration: %08x"), hr);
		return false;
	}

	this->hr = this->GetEngine()->LoadEffect(TEXT("TerrainShander"), TEXT("Terrain.fx"), &this->lpEffect);
	if (FAILED(hr))
	{
		this->Release();
		this->GetEngine()->Log(TEXT("Can't load Fx: %08x"), hr);
		return false;
	}

	this->lpEffect->SetTechnique("Basic");

	this->Loaded = true;

	return true;
}

void CTerrain::Draw()
{
	if (!this->Loaded) return;
	this->bDrawing = true;
	D3DXVECTOR3 tmp1, tmp2;

	int CCellX = this->Center.x / CELL_SIZE;
	int CCellY = this->Center.y / CELL_SIZE;

	int sx = max(CCellX - VIEW_TILES / 2, 0);
	int sy = max(CCellY - VIEW_TILES / 2, 0);
	int ex = min(CCellX + VIEW_TILES / 2, 255);
	int ey = min(CCellY + VIEW_TILES / 2, 255);

	this->GetDevice()->SetStreamSource(0, this->VBTerrain, 0, sizeof(TVertex));
	this->GetDevice()->SetIndices(this->IBTerrain);
	this->GetDevice()->SetVertexDeclaration(this->VDTerrain);

	WORD Index = 0;
	UINT Passes;
	this->lpEffect->SetFloat("Ambient", this->GetEngine()->GetAmbientLight(LFMT_YBR).r*0.5);
	this->lpEffect->SetFloat("Time", this->GetEngine()->GetStatus()->Time);
	this->lpEffect->SetMatrix("mWVP", &this->GetEngine()->GetWVP());
	this->lpEffect->SetMatrix("mW", &this->GetEngine()->GetWorld());
	if (this->RPlane)
	{
		this->lpEffect->SetMatrix("mWRP", &this->GetEngine()->GetWRP(this->RPlane));
		this->lpEffect->SetTexture("ReflMapTexture", this->RPlane->RenderTarget);
	}


	for(UINT i = 0; i < 8; i++)
		this->GetDevice()->SetTexture(i, this->TXTerrain[i]);

	this->lpEffect->Begin(&Passes, 0);

	WORD wID = 0;
	if (this->Map != 11)
		for (UINT i = 0; i < Passes; i++)
		{
			this->lpEffect->BeginPass(i);
			for (int y = sy; y < ey; y++)
			{
				for (int x = sx; x < ex; x++)
				{
					Index = x + y * 256;
					this->GetDevice()
						->DrawIndexedPrimitive(
							D3DPT_TRIANGLELIST, Index, 0, 4, 0, 2);
					if (this->Layer[0][Index] == 5) wID = Index;
				}
			}
			this->lpEffect->EndPass();
		}
	this->lpEffect->End();

	/*if (this->RPlane)
	{
		this->RPlane->Normal = this->Normals[wID];
		this->RPlane->Origin = D3DXVECTOR3(0, 0, ((float)this->Heights[wID]) * CELL_SCALE*CELL_SIZE);
		D3DXPlaneFromPointNormal(&this->RPlane->_Plane, &this->RPlane->Origin, &this->RPlane->Normal);
	}*/

	//this->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	this->GetEngine()->PushWorld();
	tmp2 = D3DXVECTOR3(Center.x, Center.y, 0);
	for (auto&it : this->vObjects)
	{
		tmp1 = it.Position;
		tmp1.z = 0;
		tmp1 -= tmp2;
		if (D3DXVec3Length(&tmp1) > RADIUS ||
			!this->Model[it.Model]) continue;
		this->GetEngine()->SetWorld(&it.mWorld);
		this->Model[it.Model]->Draw();
		//it.DrawTime += this->GetEngine()->GetStatus()->Elapsed;
	}
	this->GetEngine()->PopWorld();
	//this->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//this->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	this->FxMng->Draw();
	this->bDrawing = false;
}

bool CTerrain::LoadLayers(const TCHAR * szFile)
{
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwSize;
	if (hFile == INVALID_HANDLE_VALUE) return false;
	if (GetFileSize(hFile, &dwSize) != MAP_FILE_SIZE) { this->GetEngine()->LOG("File no match"); CloseHandle(hFile); return false; }
	ReadFile(hFile, this->Layer, MAP_FILE_SIZE, &dwSize, NULL);
	CloseHandle(hFile);
	decrypt((char*)this->Layer, MAP_FILE_SIZE);
	return true;
}

bool CTerrain::LoadAttributes(const TCHAR * szFile)
{
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return false;
	DWORD dwSize = GetFileSize(hFile, nullptr);
	char*Buffer;
	Buffer = (char*)malloc(dwSize);
	ReadFile(hFile, Buffer, dwSize, &dwSize, NULL);
	CloseHandle(hFile);

	decrypt(Buffer, dwSize);
	decrypt2(Buffer, dwSize);

	if (dwSize == ATT_FILE_65KB_SIZE)
	{
		for (int i = 0; i < TERRAIN_SIZE; i++)
		{
			this->Attributes[i] = Buffer[4 + i];
		}
	}
	else
	{
		memcpy(this->Attributes, Buffer+4, dwSize-4);
	}
	free(Buffer);
	return true;
}

bool CTerrain::LoadHeight(const TCHAR * szFile)
{
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return false;

	DWORD dwRead;

#pragma pack(push,1)
	struct hbmp
	{
		BYTE Magic[2];//2
		DWORD Size;//4
		WORD Reserved[2];//4
		DWORD DataOffset;//4
		DWORD HeadSize;//4
		DWORD W;//4
		DWORD H;//4
		WORD Planes;//2
		WORD PixelSize;//2
		DWORD Compression;//4
		DWORD DataSize;//4
	} header;
#pragma pack(pop)

	SetFilePointer(hFile, 4, nullptr, FILE_BEGIN);
	ReadFile(hFile, &header, sizeof(header), &dwRead, NULL);
	SetFilePointer(hFile, header.DataOffset+4, nullptr, FILE_BEGIN);
	//BYTE * Data = (BYTE*)malloc(header.DataSize);
	//ReadFile(hFile, Data, header.DataSize, &dwRead, NULL);
	//CloseHandle(hFile);

	BYTE Data[4];
	for (UINT i = 0, j=0; i < header.DataSize && j < TERRAIN_SIZE; i+=header.PixelSize/8, j++)
	{
		ReadFile(hFile, Data, header.PixelSize/8, &dwRead, NULL);
		this->Heights[j] = Data[0];
	}

	CloseHandle(hFile);
	//free(Data);

	return true;
}

bool CTerrain::LoadLight(const TCHAR * szFile)
{
	PDIRECT3DTEXTURE9 LightMap;
	D3DLOCKED_RECT rect;

	if (FAILED(WebZenTexture(this->GetDevice(), szFile, &LightMap))) return false;

	LightMap->LockRect(0, &rect, nullptr, D3DLOCK_READONLY);
	DWORD * dwPixel = static_cast<DWORD*>(rect.pBits);
	for (int i = 0; i < TERRAIN_SIZE; i++)
	{
		this->Light[i] = D3DXCOLOR(dwPixel[i]);
	}
	LightMap->UnlockRect(0);
	SAFE_RELEASE(LightMap);

	return true;
}

bool CTerrain::LoadObjects(const TCHAR * szFile)
{
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return false;

	DWORD dwSize = GetFileSize(hFile, nullptr);
	char * Buffer = static_cast<char*>(malloc(dwSize));
	ReadFile(hFile, Buffer, dwSize, &dwSize, NULL);
	CloseHandle(hFile);

	decrypt(Buffer, dwSize);
	DWORD dwObjectCount = (dwSize-4)/sizeof(TerrainObjectInfo);
	TerrainObjectInfo * ObjectArray = (TerrainObjectInfo*)(Buffer+4);
	TerrainObjectInfo Empty = { 0 };
	TerrainObject Object;
	D3DXMATRIX mT;
	WORD Index;
	TCHAR szName[MAX_PATH];

	for (UINT i = 0; i < dwObjectCount; i++)
	{
		if (
			!memcmp(&Empty, ObjectArray + i, sizeof(TerrainObjectInfo)) ||
			ObjectArray[i].Index < 0
			) continue;

		Object.Position = ObjectArray[i].Position;
		Object.Position.x += 2.0f*CELL_SIZE;
		ObjectArray[i].Rotation *= D3DX_PI / 180.0f;

		Index = (WORD)(Object.Position.x / CELL_SIZE + Object.Position.y / CELL_SIZE * 256.0);
		Object.Diffuse = this->Light[Index];

		D3DXMatrixScaling(&mT, ObjectArray[i].Size, ObjectArray[i].Size, ObjectArray[i].Size);
		Object.mWorld = mT;
		D3DXMatrixRotationYawPitchRoll(&mT, ObjectArray[i].Rotation.y, ObjectArray[i].Rotation.x, ObjectArray[i].Rotation.z);
		Object.mWorld *= mT;
		D3DXMatrixTranslation(&mT, Object.Position.x, Object.Position.y, Object.Position.z);
		Object.mWorld *= mT;

		if (this->Map == 1)
		{
			if (ObjectArray[i].Index >= 154) continue;
			_stprintf_s(szName, TEXT("./Data/Object%d/%s.bmd"), this->Map, szTerrainObject[ObjectArray[i].Index]);
		}
		else
		{
			_stprintf_s(szName, TEXT("./Data/Object%d/Object%02d.bmd"), this->Map, ObjectArray[i].Index + 1);
		}

		WORD ID = ObjectArray[i].Index;
		int Fx = -1;
		switch (MAKE_OBJID(this->Map, ID))
		{
			case MAKE_OBJID(75, 79):
			case MAKE_OBJID(74, 79):
				Fx=10;
				break;
			case MAKE_OBJID(1, 132):
			case MAKE_OBJID(1, 130):// Smoke
				Fx = 11;
					break;
			case MAKE_OBJID(1,131):// WaterSpout
				Fx = 12;
				break;
		}
		if (Fx>=0)
		{
			this->FxMng->AddPoint(Fx, Object.Position, ObjectArray[i].Size);
		}else
		if (this->Model[ID] == nullptr)
		{
			this->Model[ID] = new WZModel(szName);
			_stprintf_s(szName, TEXT("Terrain %d BMD Model %d"), this->Map, ObjectArray[i].Index + 1);
			this->Model[ID]->SetName(szName);
			switch (MAKE_OBJID(this->Map,ID))
			{
			case MAKE_OBJID(3, 19):
			case MAKE_OBJID(3, 78):
			case MAKE_OBJID(3, 92):
			case MAKE_OBJID(3, 93):
			case MAKE_OBJID(4, 9):
			case MAKE_OBJID(4, 37):
			case MAKE_OBJID(4, 39):
			case MAKE_OBJID(4, 41):
			case MAKE_OBJID(8, 23):
			case MAKE_OBJID(8, 38):
			case MAKE_OBJID(8, 40):
			case MAKE_OBJID(9, 02):
			case MAKE_OBJID(9, 12):
			case MAKE_OBJID(9, 13):
			case MAKE_OBJID(9, 81)://Cristal
			case MAKE_OBJID(9, 82)://Luz
			case MAKE_OBJID(9, 87):
			//case MAKE_OBJID(52, 20):
			//case MAKE_OBJID(52, 21):
			case MAKE_OBJID(52, 56):
			case MAKE_OBJID(52, 78)://Cristal
			case MAKE_OBJID(52, 79)://Cristal
			case MAKE_OBJID(52, 85):
			//case MAKE_OBJID(52, 122):
			//case MAKE_OBJID(52, 123):
			//case MAKE_OBJID(52, 124):
			case MAKE_OBJID(52, 125):
			case MAKE_OBJID(52, 128):
			case MAKE_OBJID(52, 131):
			case MAKE_OBJID(52, 132):
			case MAKE_OBJID(74, 9):
			case MAKE_OBJID(74, 152):// Transparencia
			case MAKE_OBJID(74, 15):// Transparencia
			case MAKE_OBJID(74, 23):// Transparencia
			case MAKE_OBJID(74, 76):// Transparencia
			case MAKE_OBJID(74, 77):// Transparencia
			case MAKE_OBJID(74, 81):// Agua
				this->Model[ID]->SetMeshEffect(0, ME_TRANSPARENT);
				break;
			case MAKE_OBJID(52, 110):
				this->Model[ID]->SetMeshEffect(0, ME_TRANSPARENT);
				this->Model[ID]->SetMeshEffect(1, ME_TRANSPARENT);
				this->Model[ID]->SetMeshEffect(2, ME_TRANSPARENT);
				break;
			case MAKE_OBJID(5, 18):
			case MAKE_OBJID(5, 23):
			case MAKE_OBJID(8, 32):
			case MAKE_OBJID(8, 35):
				this->Model[ID]->SetMeshEffect(1, ME_TRANSPARENT);
				break;
			case MAKE_OBJID(1, 98):
				this->Model[ID]->SetMeshEffect(2, ME_TRANSPARENT);
				break;
			case MAKE_OBJID(52, 02):
			case MAKE_OBJID(52, 53):
			case MAKE_OBJID(52, 55):
			case MAKE_OBJID(52, 126):
				this->Model[ID]->SetMeshEffect(0, ME_WATER);
				break;
			case MAKE_OBJID(1, 105):// Water
				this->Model[ID]->SetMeshEffect(3, ME_WATER);
				break;
			case MAKE_OBJID(1, 119)://
				this->Model[ID]->SetMeshEffect(2, ME_WATER);
				break;
			case MAKE_OBJID(1, 122):
			case MAKE_OBJID(74, 92):// Transparencia
				this->Model[ID]->SetMeshEffect(4, ME_TRANSPARENT);
				break;
			case MAKE_OBJID(74, 149):// Cristal
			case MAKE_OBJID(74, 150):// Cristal
			case MAKE_OBJID(74, 151):// Cristal
			case MAKE_OBJID(74, 127):// Luz
				this->Model[ID]->SetMeshEffect(0, ME_LIGHT);
				break;
			case MAKE_OBJID(9, 07)://fire
				this->Model[ID]->SetMeshEffect(0, ME_FIRE);
				break;
			case MAKE_OBJID(9, 72)://Lava
			case MAKE_OBJID(9, 73)://Lava
			case MAKE_OBJID(9, 75)://Lava
			case MAKE_OBJID(9, 79)://Lava
				this->Model[ID]->SetMeshEffect(0, ME_LAVA);
				break;
			case MAKE_OBJID(11, 6):
			case MAKE_OBJID(11, 7):
			case MAKE_OBJID(11, 8):
			case MAKE_OBJID(11, 12):
			case MAKE_OBJID(11, 13):
			case MAKE_OBJID(11, 14):
			case MAKE_OBJID(11, 15):
			case MAKE_OBJID(11, 10):
			case MAKE_OBJID(52, 130):
				this->Model[ID]->SetMeshEffect(0, ME_CLOUD);
				this->Model[ID]->SetMeshEffect(1, ME_CLOUD);
				break;
			}
			this->GetEngine()->AddLoadQueueElement(this->Model[ID]);
		}

		switch (MAKE_OBJID(this->Map, ID))
		{
		case MAKE_OBJID(1, 50):
			this->FxMng->AddPoint(10, Object.Position+D3DXVECTOR3(0,0,150), ObjectArray[i].Size);
			break;
		case MAKE_OBJID(1, 51):
		case MAKE_OBJID(1, 52):
			this->FxMng->AddPoint(0, Object.Position + D3DXVECTOR3(0, 0, 50), ObjectArray[i].Size);
			break;
			case MAKE_OBJID(1, 133):
				continue;
		}

		Object.DrawTime = 0;
		Object.Model = ID;
		this->vObjects.push_back(Object);
	}
	
	return true;
}

void CTerrain::Release()
{
	if (!this->Loaded) return;
	this->Loaded = false;
	while (this->bDrawing) Sleep(10);
	for (UINT i = 0; i < MAX_MODELS; i++)
	{
		this->GetEngine()->Release(this->Model[i]);
		this->Model[i] = nullptr;
	}
	for (int i = 0; i < 15; i++)
	{
		SAFE_RELEASE(this->TXTerrain[i]);
	}
	this->FxMng->Flush();
	SAFE_RELEASE(this->IBTerrain);
	SAFE_RELEASE(this->VBTerrain);
	SAFE_RELEASE(this->VDTerrain);
	//SAFE_RELEASE(this->lpEffect);
	this->vObjects.clear();
	this->GetEngine()->Release(this->FxMng);
	this->GetEngine()->FlushReflections();
	this->RPlane = nullptr;
}

float CTerrain::GetHeight(int x, int y)
{
	return ((float)this->Heights[x + y * 256]) * CELL_SCALE*CELL_SIZE;
}

BOOL CTerrain::TestRay(D3DXVECTOR3 *vin)
{
	//D3DXVECTOR3 Org(this->GetEngine()->GetStatus()->Eye_Position);
	int w, h;
	float fn = this->GetEngine()->GetStatus()->plane_far - this->GetEngine()->GetStatus()->plane_near;
	this->GetEngine()->GetDevInfo(w, h);
	vin->x /= (float)w/2.0;
	vin->y /= (float)h/2.0;
	vin->x -= 1.0f;
	vin->y -= 1.0f;
	vin->y *= -1.0f;
	D3DXVECTOR3 p1(vin->x, vin->y, 1), p2(vin->x, vin->y, 0);
	D3DXMATRIX ivView/*, View*/;
	//this->GetEngine()->GetDevice()->GetTransform(D3DTS_VIEW, &View);
	FLOAT det; 
	D3DXMatrixInverse(&ivView, &det, &this->GetEngine()->GetWVP());
	//p1 *= this->GetEngine()->GetStatus()->plane_far;
	//p2 *= this->GetEngine()->GetStatus()->plane_near;
	D3DXVECTOR4 v4;
	D3DXVec3Transform(&v4, &p1, &ivView);
	p1 = D3DXVECTOR3(v4.x, v4.y, v4.z)/ v4.w;
	D3DXVec3Transform(&v4, &p2, &ivView);
	p2 = D3DXVECTOR3(v4.x, v4.y, v4.z) / v4.w;
	//D3DXVec3Normalize(vin, &(p1-p2));

	D3DXPLANE ref;
	D3DXVECTOR3 IntersecPoint;

	int sx = max(this->Center.x / CELL_SIZE - CELL_SIZE / 2, 0);
	int sy = max(this->Center.y / CELL_SIZE - CELL_SIZE / 2, 0);
	int ex = min(this->Center.x / CELL_SIZE + CELL_SIZE / 2, 256);
	int ey = min(this->Center.y / CELL_SIZE + CELL_SIZE / 2, 256);
	for (int j = sy; j < ey; j++)
	{
		for (int i = sx; i < ex; i++)
		{
			int index = j * 256 + i;
			D3DXPlaneFromPointNormal(&ref,
				&D3DXVECTOR3(i*CELL_SIZE, j*CELL_SIZE, this->Heights[index]),
				&this->Normals[index]);

			D3DXPlaneIntersectLine(
				&IntersecPoint,
				&ref,
				&p1,
				&p2
			);

			if (
				j == (int)(IntersecPoint.y / CELL_SIZE) &&
				i == (int)(IntersecPoint.x / CELL_SIZE)
				)
			{
				*vin = IntersecPoint;
				return TRUE;
			}
		}
	}

	return FALSE;

	//for (int i = 0; i < TERRAIN_SIZE; i++)
	//{
	//	int x = i % 256;
	//	int y = i / 256;
	//	float z = ((float)this->Heights[i]) * CELL_SCALE*CELL_SIZE;

	//	D3DXVECTOR3 dira = D3DXVECTOR3(
	//		x*CELL_SIZE + CELL_SIZE / 2.0f,
	//		y*CELL_SIZE + CELL_SIZE / 2.0f,
	//		z
	//		) - p2;

	//	//D3DXVec3Normalize(&dira, &dira);
	//	float factor = (D3DXVec3Dot(&dira, &this->Normals[i])/ D3DXVec3Dot(vin, &this->Normals[i]));
	//	D3DXVECTOR3 Dest = (*vin)*factor + p2;

	//	//if (Dest.z<=z+10.0 && Dest.z>=z-10.0)
	//	if (
	//		(Dest.x + CELL_SIZE / 2.0f >= x*CELL_SIZE && Dest.x - CELL_SIZE / 2.0f <= x*CELL_SIZE) &&
	//		(Dest.y + CELL_SIZE / 2.0f >= y*CELL_SIZE && Dest.y - CELL_SIZE / 2.0f <= y*CELL_SIZE)
	//		)
	//	{
	//		*vin = Dest;
	//		return TRUE;
	//	}
	//}
	//return FALSE;
}

BOOL CTerrain::GetPoint(D3DXVECTOR3 * vin)
{
	D3DXPLANE plPN;

	int x = vin->x / CELL_SIZE;
	int y = vin->y / CELL_SIZE;
	int index = y * 256 + x;

	x *= CELL_SIZE;
	y *= CELL_SIZE;

	if (index < 0 || index > 65535)
		return FALSE;

	float z = this->Heights[index];
	z *= CELL_SCALE*CELL_SIZE;

	D3DXPlaneFromPointNormal(&plPN,
		&D3DXVECTOR3(x, y, z),
		&this->Normals[index]
	);

	D3DXVECTOR3 PointOverPlane;
	D3DXVECTOR3 pStart = *vin;
	D3DXVECTOR3 pEnd = *vin;
	pStart.z = 10000;
	pEnd.z = -10000;

	D3DXPlaneIntersectLine(&PointOverPlane,
		&plPN,
		&pStart,
		&pEnd
	);

	*vin = PointOverPlane;
	return TRUE;

	/*vin->z = 1000;
	for (int i = 0; i < TERRAIN_SIZE; i++)
	{
		int x = i % 256;
		int y = i / 256;
		float z = ((float)this->Heights[i]) * CELL_SCALE*CELL_SIZE;
		

		D3DXVECTOR3 dira = D3DXVECTOR3(
			x*CELL_SIZE + CELL_SIZE / 2.0f,
			y*CELL_SIZE + CELL_SIZE / 2.0f,
			z
			) - *vin;

		float factor = (D3DXVec3Dot(&dira, &this->Normals[i]) / D3DXVec3Dot(&D3DXVECTOR3(0,0,-1), &this->Normals[i]));
		D3DXVECTOR3 Dest = D3DXVECTOR3(0, 0, -1)*factor + *vin;

		//if (Dest.z <= z + 10.0 && Dest.z >= z - 10.0)
		if(
			(Dest.x + CELL_SIZE / 2.0f >= x*CELL_SIZE && Dest.x - CELL_SIZE / 2.0f <= x*CELL_SIZE) &&
			(Dest.y + CELL_SIZE / 2.0f >= y*CELL_SIZE && Dest.y - CELL_SIZE / 2.0f <= y*CELL_SIZE)
		)
		{
			*vin = Dest;
			return TRUE;
		}
	}
	return FALSE;*/
}
