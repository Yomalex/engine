#include "stdafx.h"
#include "LoadSplash.h"
#include "WZen.h"

D3DVERTEXELEMENT9 LoadSplash[] =
{
	{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION},
	{0,12,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD},
	D3DDECL_END()
};

const TCHAR* szLoadScreen[] =
{
	TEXT(".\\Data\\Interface\\New_lo_back_01.ozj"),
	TEXT(".\\Data\\Interface\\New_lo_back_02.ozj"),
	TEXT(".\\Data\\Interface\\lo_back_s5_im01.ozj"),
	TEXT(".\\Data\\Interface\\lo_back_s5_im02.ozj"),
	TEXT(".\\Data\\Interface\\lo_back_s5_im03.ozj"),
	TEXT(".\\Data\\Interface\\lo_back_s5_im04.ozj"),
	TEXT(".\\Data\\Interface\\lo_back_s5_im05.ozj"),
	TEXT(".\\Data\\Interface\\lo_back_s5_im06.ozj"),
	TEXT(".\\Data\\Interface\\lo_back_s5_03.ozj"),
	TEXT(".\\Data\\Interface\\lo_back_s5_04.ozj"),
	TEXT(".\\Data\\Interface\\lo_lo.ozj"),
	TEXT(".\\Data\\Interface\\New_lo_mu_logo.ozt"),
	nullptr
};

CLoadSplash::CLoadSplash()
	:Loaded(false), VBGeometry(nullptr), IBGeometry(nullptr), progress(0.0)
{
	for (int i = 0; i < 12; i++)
	{
		this->Texturing[i] = nullptr;
	}
}

CLoadSplash::~CLoadSplash()
{
	this->Release();
}

bool CLoadSplash::Load()
{
	if (this->Loaded) return true;

	HRESULT hr;

	hr = this->GetDevice()->CreateVertexBuffer(
		sizeof(Geometry),
		0,
		0,
		D3DPOOL_MANAGED,
		&this->VBGeometry,
		NULL
		);

	if (FAILED(hr))
	{
		this->GetEngine()->Log(TEXT("Error %s"), DXGetErrorDescription(hr));
		this->GetEngine()->Kill(true);
		return false;
	}

	this->Loaded = true;

	this->GetDevice()->CreateIndexBuffer(
		12,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&this->IBGeometry,
		NULL
		);

	this->SetUpGeometry();

	this->GetDevice()->CreateVertexDeclaration(LoadSplash, &this->VDGeometry);

	for (int i = 0; i < 12; i++)
	{
		WebZenTexture(this->GetDevice(), szLoadScreen[i], &this->Texturing[i]);
	}
	this->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	this->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	this->GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	this->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	return true;
}

void CLoadSplash::Draw()
{
	if (!this->Loaded) return;
	this->GetDevice()->SetStreamSource(0, this->VBGeometry, 0, sizeof(CLoadSplash::VertexLS));
	this->GetDevice()->SetIndices(this->IBGeometry);
	this->GetDevice()->SetVertexDeclaration(this->VDGeometry);
	
	D3DXMATRIX mWorld, mScaling;
	D3DXMatrixIdentity(&mWorld);
	this->GetDevice()->SetTransform(D3DTS_WORLD, &mWorld);
	for (int i = 0; i < 12; i++)
	{
		if (i == 10)continue;
		this->GetDevice()->SetTexture(0, this->Texturing[i]);
		this->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, i*4, 0, 4, 0, 2);
	}
	D3DXMatrixScaling(&mScaling, this->progress, 1, 1);
	D3DXMatrixTranslation(&mTrans, 0.0875*this->w, 0, 0);
	mWorld = mScaling*mTrans;
	this->GetDevice()->SetTransform(D3DTS_WORLD, &mWorld);
	this->GetDevice()->SetTexture(0, this->Texturing[10]);
	this->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 40, 0, 4, 0, 2);
	D3DXMatrixIdentity(&mWorld);
	this->GetDevice()->SetTransform(D3DTS_WORLD, &mWorld);
	RECT r = {this->w,0.936f*this->h,100,30};

	this->GetEngine()->Text(this->szText, &r, D3DXCOLOR(1, 1, 1, 1), DT_CENTER|DT_NOCLIP);
}

void CLoadSplash::Release()
{
	SAFE_RELEASE(this->VBGeometry);
	SAFE_RELEASE(this->IBGeometry);
	SAFE_RELEASE(this->VDGeometry);
	for (int i = 0; i < 12; i++)
	{
		SAFE_RELEASE(this->Texturing[i]);
	}
	this->Loaded = false;
}

void CLoadSplash::SetProgress(float prog, const TCHAR * szProgress)
{
	this->progress = prog;
	_tcscpy_s(this->szText, szProgress);
}

void CLoadSplash::OnLostDevice()
{
	// Empty
}

void CLoadSplash::OnResetDevice()
{
	this->SetUpGeometry();
}

void CLoadSplash::SetUpGeometry()
{
	if (!this->Loaded) return;
	void *Vertex;
	//int h;
	this->GetEngine()->GetDevInfo(this->w, this->h);

	CLoadSplash::VertexLS tGeometry[] = {
		{ D3DXVECTOR3(0.0,0.0,0),D3DXVECTOR2(0,0) },//0
		{ D3DXVECTOR3(0.0,0.1f*h,0),D3DXVECTOR2(0,1) },//1
		{ D3DXVECTOR3(0.5f*w,0.1f*h,0),D3DXVECTOR2(1,1) },//2
		{ D3DXVECTOR3(0.5f*w,0,0),D3DXVECTOR2(1,0) },//3
		//------------------------------------------------------
		{ D3DXVECTOR3(0.5f*w,0,0),D3DXVECTOR2(0,0) },//0
		{ D3DXVECTOR3(0.5f*w,0.1f*h,0),D3DXVECTOR2(0,1) },//1
		{ D3DXVECTOR3(1.0f*w,0.1f*h,0),D3DXVECTOR2(1,1) },//2
		{ D3DXVECTOR3(1.0f*w,0,0),D3DXVECTOR2(1,0) },//3
		//------------------------------------------------------
		//------------------------------------------------------
		{ D3DXVECTOR3(0,0.1f*h,0),D3DXVECTOR2(0,0) },//0
		{ D3DXVECTOR3(0,0.6f*h,0),D3DXVECTOR2(0,1) },//1
		{ D3DXVECTOR3(0.4f*w,0.6f*h,0),D3DXVECTOR2(1,1) },//2
		{ D3DXVECTOR3(0.4f*w,0.1f*h,0),D3DXVECTOR2(1,0) },//3
		//------------------------------------------------------
		{ D3DXVECTOR3(0.4f*w,0.1f*h,0),D3DXVECTOR2(0,0) },//0
		{ D3DXVECTOR3(0.4f*w,0.6f*h,0),D3DXVECTOR2(0,1) },//1
		{ D3DXVECTOR3(0.8f*w,0.6f*h,0),D3DXVECTOR2(1,1) },//2
		{ D3DXVECTOR3(0.8f*w,0.1f*h,0),D3DXVECTOR2(1,0) },//3
		//------------------------------------------------------
		{ D3DXVECTOR3(0.8f*w,0.1f*h,0),D3DXVECTOR2(0,0) },//0
		{ D3DXVECTOR3(0.8f*w,0.6f*h,0),D3DXVECTOR2(0,1) },//1
		{ D3DXVECTOR3(w,0.6f*h,0),D3DXVECTOR2(1,1) },//2
		{ D3DXVECTOR3(w,0.1f*h,0),D3DXVECTOR2(1,0) },//3
		//------------------------------------------------------
		//------------------------------------------------------
		{ D3DXVECTOR3(0,0.6f*h,0),D3DXVECTOR2(0,0) },//0
		{ D3DXVECTOR3(0,0.9f*h,0),D3DXVECTOR2(0,1) },//1
		{ D3DXVECTOR3(0.4f*w,0.9f*h,0),D3DXVECTOR2(1,1) },//2
		{ D3DXVECTOR3(0.4f*w,0.6f*h,0),D3DXVECTOR2(1,0) },//3
		//------------------------------------------------------
		{ D3DXVECTOR3(0.4f*w,0.6f*h,0),D3DXVECTOR2(0,0) },//0
		{ D3DXVECTOR3(0.4f*w,0.9f*h,0),D3DXVECTOR2(0,1) },//1
		{ D3DXVECTOR3(0.8f*w,0.9f*h,0),D3DXVECTOR2(1,1) },//2
		{ D3DXVECTOR3(0.8f*w,0.6f*h,0),D3DXVECTOR2(1,0) },//3
		//------------------------------------------------------
		{ D3DXVECTOR3(0.8f*w,0.6f*h,0),D3DXVECTOR2(0,0) },//0
		{ D3DXVECTOR3(0.8f*w,0.9f*h,0),D3DXVECTOR2(0,1) },//1
		{ D3DXVECTOR3(w,0.9f*h,0),D3DXVECTOR2(1,1) },//2
		{ D3DXVECTOR3(w,0.6f*h,0),D3DXVECTOR2(1,0) },//3
		//------------------------------------------------------
		//------------------------------------------------------
		{ D3DXVECTOR3(0,0.9f*h,0),D3DXVECTOR2(0,0) },//0
		{ D3DXVECTOR3(0,h,0),D3DXVECTOR2(0,1) },//1
		{ D3DXVECTOR3(0.5f*w,h,0),D3DXVECTOR2(1,1) },//2
		{ D3DXVECTOR3(0.5f*w,0.9f*h,0),D3DXVECTOR2(1,0) },//3
		//------------------------------------------------------
		{ D3DXVECTOR3(0.5f*w,0.9f*h,0),D3DXVECTOR2(0,0) },//0
		{ D3DXVECTOR3(0.5f*w,h,0),D3DXVECTOR2(0,1) },//1
		{ D3DXVECTOR3(w,h,0),D3DXVECTOR2(1,1) },//2
		{ D3DXVECTOR3(w,0.9f*h,0),D3DXVECTOR2(1,0) },//3
		//------------------------------------------------------
		//------------------------------------------------------
		{ D3DXVECTOR3(0,0.936f*h,0),D3DXVECTOR2(0,0) },//0
		{ D3DXVECTOR3(0,0.958f*h,0),D3DXVECTOR2(0,1) },//1
		{ D3DXVECTOR3(0.825*w,0.958f*h,0),D3DXVECTOR2(1,1) },//2
		{ D3DXVECTOR3(0.825*w,0.936f*h,0),D3DXVECTOR2(1,0) },//3
		//------------------------------------------------------
		//------------------------------------------------------
		{ D3DXVECTOR3(0.45f*w,0.45f*h,0),D3DXVECTOR2(0,0) },//0
		{ D3DXVECTOR3(0.45f*w,0.55f*h,0),D3DXVECTOR2(0,1) },//1
		{ D3DXVECTOR3(0.55f*w,0.55f*h,0),D3DXVECTOR2(1,1) },//2
		{ D3DXVECTOR3(0.55f*w,0.45f*h,0),D3DXVECTOR2(1,0) },//3
	};

	memcpy(Geometry, tGeometry, sizeof(Geometry));

	WORD Index[6] = { 0,1,2,2,3,0 };

	this->VBGeometry->Lock(0, 0, &Vertex, 0);
	memcpy(Vertex, Geometry, sizeof(Geometry));
	this->VBGeometry->Unlock();

	this->IBGeometry->Lock(0, 0, &Vertex, 0);
	memcpy(Vertex, Index, sizeof(Index));
	this->IBGeometry->Unlock();
}
