#pragma once
#include <list>
#include <map>
#define BMD_NOENCRY	0x0a
#define BMD_ENCRY	0x0c


#pragma pack(push,1)
struct  BMDSig
{
	char Signature[3]; //BMD
	BYTE Encrypt; //Encriptado
};
struct BMDHeader
{
	char strFile[32];//Nombre del Archivo
	WORD uMeshCount;//Caras de malla
	WORD uBoneCount;//Huesos
	WORD uAnimCount;//Animaciones
};

// Mesh Info
struct BMDMeshHeader
{
	WORD nPos;
	WORD nNor;
	WORD nTex;
	WORD nTri;
	WORD id;
};
struct BMDPos
{
	int iParent;
	D3DXVECTOR3 vPos;
};
struct BMDNor
{
	int iParent;
	D3DXVECTOR3 vNor;
	DWORD id;
};
struct BMDtextuv
{
	//float u, v;
	D3DXVECTOR2 UV;
};
struct BMDTriangle
{
	BYTE Count;			//0
	BYTE Padding1;		//1
	WORD indexVertex[3];//2-8
	BYTE Padding2[2];	//9-10
	WORD indexNormal[3];//11-17
	BYTE Padding3[2];	//18-20
	WORD indexUV[3];	//21-27
	BYTE Padding4[40];	//28-68
	// Size: 0x45
};

// Animation Info
struct BMDAnimationHeader
{
	WORD Count;
	bool Flag;
	//D3DXVECTOR3 Vectors[1];
};

// Bone Info
struct BMDBone
{
	char szName[32];
	short iParent;
};
#pragma pack(pop)

struct BMDMeshVertex
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
	float w;
	BYTE Bone[4];
};
struct BMDMesh
{
	DWORD Index;
	DWORD Effect;
	DWORD Offset;
	DWORD PrimitiveCount;
	BYTE FirstBone;
	BYTE LastBone;
	char TextureName[32];
	PDIRECT3DTEXTURE9 Texture;
};
//struct BMDFrame
//{
//	std::vector<D3DXMATRIX> Bone;
//};
//struct  BMDAnimation
//{
//	std::vector<std::vector<D3DXMATRIX>> Frames
//};

class WZModel:
	public CEngineElement
{
	BMDSig Signature;
	BMDHeader FileInfo;

	DWORD FileSize;
	DWORD EncryptSize;

	BYTE fxMesh[126];
	std::vector<BMDMesh> MeshList;
	std::vector<BMDBone> BoneList;
	std::vector<std::vector<std::vector<D3DXMATRIX>>> AnimationList;
	DWORD VertexCount;

	PDIRECT3DVERTEXBUFFER9 Vertex;
	PDIRECT3DVERTEXBUFFER9 VertexDraw;
	LPD3DXEFFECT lpEffect;

	short LastFrame;
	BYTE Animation;
	double StartTime;

	D3DXMATRIX Frame[126];

	TCHAR szPath[128];

	WZModel * Source;
public:
	//properties
	bool bValid;
	DWORD Error;
	D3DXVECTOR3 Bounding;
	D3DXVECTOR3 Center;

	// Functions
	WZModel(const TCHAR * szFileName);
	~WZModel();

	void SetMeshEffect(UINT Mesh, UINT Effect) { fxMesh[Mesh] = Effect; }
	void AttachAnimations(WZModel * mod) { Source = mod; }
	void SetAnimation(BYTE Number);

	virtual bool Load();
	virtual void Draw();
	virtual void Release();

private:
	void CPUHelp();
};

