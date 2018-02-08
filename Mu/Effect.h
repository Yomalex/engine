#pragma once

#define FX_FVF (D3DFVF_XYZ|D3DFVF_TEX1)
#define MAX_EFFECT 50
#define EFPS 5.0

float Random(float min, float max);

class CEffect :
	public CEngineElement
{
	struct EVertex
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR2 UV;
	};
	struct Particle
	{
		DWORD Life;
		D3DXVECTOR3 Velocity;
		D3DXVECTOR3 Position;
		float Frame;
	};
	struct PointInfo
	{
		D3DXVECTOR3 Pos;
		float size;
		std::vector<Particle> vParticles;
		D3DXVECTOR3 Velocity;
		DWORD LastParticle;
	};
	struct Gfx
	{
		int Type; //0 Frames 1 Unique
		DWORD Life;
		BYTE MaxParticles;
		D3DXVECTOR3 Gravity;
		D3DXVECTOR3 Velocity;
	};
	struct GfxFrames: Gfx
	{
		std::vector<PDIRECT3DTEXTURE9> vFrames;
		std::vector<TCHAR*> vFiles;
	};
	struct  GfxUnique: Gfx
	{
		PDIRECT3DTEXTURE9 lpTexture;
		BYTE Frames;
		float Rows;
		float Cols;
		TCHAR szFile[MAX_PATH];
	};
public:
	CEffect();
	//CEffect(const TCHAR * Effect, float cols, float rows):lpTexture(nullptr) { this->SetEffect(Effect, cols, rows); }
	~CEffect();
	void SetEffect(UINT Fx, const TCHAR * Effect, BYTE cols, BYTE rows);
	void SetEffect(UINT Fx, const TCHAR ** Effect, BYTE Frames);
	void SetVelocity(UINT Fx, D3DXVECTOR3 Vel) { if (Fx < MAX_EFFECT) this->gfx[Fx]->Velocity = Vel; }
	void SetGravity(UINT Fx, D3DXVECTOR3 Grav) { if (Fx < MAX_EFFECT) this->gfx[Fx]->Gravity = Grav; }
	void SetMaxParticles(UINT Fx, BYTE Max) { if (Fx < MAX_EFFECT) this->gfx[Fx]->MaxParticles = Max; }
	void SetLife(UINT Fx, DWORD Time) { if (Fx < MAX_EFFECT) this->gfx[Fx]->Life = Time; }
	bool Load();
	void Draw();
	void Release();
	void AddPoint(UINT Fx, D3DXVECTOR3 Pos, float Size, D3DXVECTOR3 Vel=D3DXVECTOR3(0,0,0));
	void Flush() { for (UINT i = 0; i < MAX_EFFECT; i++) this->vPoints[i].clear(); }
	void Flush(UINT Fx) { if(Fx<MAX_EFFECT) this->vPoints[Fx].clear(); }
private:
	std::list<PointInfo> vPoints[MAX_EFFECT];
	Gfx * gfx[MAX_EFFECT];
	PDIRECT3DVERTEXBUFFER9 VBuffer;
	LPD3DXEFFECT lpEffect;
};

