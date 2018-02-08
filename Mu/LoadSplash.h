#pragma once

#define LSFVF (D3DFVF_XYZ|D3DFVF_TEX0)
class CLoadSplash :
	public CEngineLoad
{
	struct VertexLS
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR2 uv;
	};

	float progress;
	TCHAR szText[256];
	PDIRECT3DVERTEXDECLARATION9 VDGeometry;
	LPDIRECT3DVERTEXBUFFER9 VBGeometry;
	LPDIRECT3DINDEXBUFFER9 IBGeometry;
	LPDIRECT3DTEXTURE9 Texturing[12];
	D3DXMATRIX mTrans;
	LPD3DXFONT lpFont;
	bool Loaded;
	int w;
	int h;
public:
	CLoadSplash();
	~CLoadSplash();


	// heredado
	bool Load();
	void Draw();
	void Release();
	void SetProgress(float progress, const TCHAR * szProgress);
	void OnLostDevice();
	void OnResetDevice();
	void SetUpGeometry();
private:
	VertexLS Geometry[48];
};

