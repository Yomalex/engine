#pragma once
#include <vector>
#define BMD_NOENCRY	0x0a
#define BMD_ENCRY	0x0c

#define MAX_BONE_CALL 60

struct AnimF
{
	int Count;
	std::vector<D3DXMATRIX> vLocal;
};
struct Bone
{
	char sName[32];
	short iParent;
};
struct BoneF :Bone
{
	std::vector<AnimF> Animations;
};

enum _MeshEffect
{
	ME_NONE,
	ME_TRANSPARENT,
	ME_LIGHT,
	ME_WATER,
	ME_FIRE,
	ME_LAVA,
	ME_CLOUD
};

#define MFVF (D3DFVF_XYZ)
class CModel;
typedef void(*cbModel)(CModel*caller);
#define MDEVENT(x) void x(CModel*caller)
class CModel :
	public CEngineElement
{
	struct MVertex
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Normal;
		D3DXVECTOR2 UV;
		float w;
		BYTE BoneIndex[4];
	};
	struct Mesh
	{
		DWORD Offset;
		DWORD uTri;
		UINT Effect;
		PDIRECT3DTEXTURE9 Texture;
		BYTE bmin, bmax;
	};
	struct Anim
	{
		WORD Count;
		bool Flag;
		D3DXVECTOR3 Vectors[1];
	};
	WORD cAni, cFrame;
	WORD lastFrame;
	float ModelElapsed;
	int addFrames;
public:
	CModel();
	CModel(const TCHAR * szFileName, int inflate=0);
	~CModel();

	cbModel OnAnimationEnd;
	LPARAM Param;

	void SetFile(const TCHAR * szFileName);

	bool Load();
	void Draw();
	void onFrame(float Elapsed);
	void Release();
	std::vector<BoneF>& GetAnimation();
	D3DXMATRIX GetAnimation(int Bone);
	void SetAnimation(CModel*Animation);
	void SetFrame(WORD aNumber, WORD aFrame);
	void SetFrame(WORD aFrame);
	UINT GetAnimCount();
	UINT GetAnimFrames(int ani);
	D3DXMATRIX GetBoneSpace(int bone);
	void SetMeshEffect(UINT Mesh, UINT Effect);

	D3DXVECTOR3 vBounding[2];
private:
	void VertexCPUProcess();
	//void VertexGPUProcess();

	CModel * AnimationSource;
	TCHAR szFile[MAX_PATH];
	BYTE fxMesh[128];
	bool CPURender;
	int VertexC;
	//D3DXVECTOR3 * Animation;
	std::vector<Mesh> vMesh;
	std::vector<Anim*> vAnim;
	std::vector<BoneF> vBoneF;
	void LoadMesh(char**ppData, std::list<MVertex>& V);
	void LoadAnim(char**ppData);
	void LoadBone(char**ppData);
	PDIRECT3DVERTEXDECLARATION9 VDModel;
	LPD3DXEFFECT lpEffect;
	D3DXMATRIX Frame[128];
	PDIRECT3DVERTEXBUFFER9 VBuffer;
	PDIRECT3DVERTEXBUFFER9 VBufferC;
};

