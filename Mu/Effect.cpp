#include "stdafx.h"
#include "Effect.h"
#include "WZen.h"


CEffect::CEffect()
{
	this->VBuffer = nullptr;
	ZeroMemory(this->gfx, sizeof(this->gfx));
	this->lpEffect = nullptr;
	this->SetName(TEXT("Effect Manager"));
}

CEffect::~CEffect()
{
	for (UINT i = 0; i < MAX_EFFECT; i++)
	{
		if (this->gfx[i] == nullptr) continue;
		delete this->gfx[i];
	}
	this->Release();
}

void CEffect::SetEffect(UINT Fx, const TCHAR * Effect, BYTE cols, BYTE rows)
{
	if (Fx >= MAX_EFFECT) return;
	if (this->gfx[Fx] != nullptr) delete this->gfx[Fx];
	this->gfx[Fx] = new GfxUnique();
	GfxUnique * pUnique = (GfxUnique *)this->gfx[Fx];
	pUnique->Frames = rows*cols;
	pUnique->Cols = cols;
	pUnique->Rows = rows;
	pUnique->Life = INFINITE;
	pUnique->MaxParticles = 1;
	pUnique->Gravity = D3DXVECTOR3(0, 0, 0);
	pUnique->Velocity = D3DXVECTOR3(0, 0, 0);
	pUnique->Type = 1;
	_tcscpy_s(pUnique->szFile, Effect);
}

void CEffect::SetEffect(UINT Fx, const TCHAR ** Effect, BYTE Frames)
{
	if (Fx >= MAX_EFFECT) return;
	
	if (this->gfx[Fx] != nullptr) delete this->gfx[Fx];
	this->gfx[Fx] = new GfxFrames();
	GfxFrames * pFrames = (GfxFrames *)this->gfx[Fx];
	pFrames->Life = INFINITE;
	pFrames->MaxParticles = 1;
	pFrames->Gravity = D3DXVECTOR3(0, 0, 0);
	pFrames->Velocity = D3DXVECTOR3(0, 0, 0);
	pFrames->Type = 0;
	pFrames->vFrames.clear();


	for (int i = 0; i < Frames; i++)
	{
		int ilen = _tcslen(Effect[i]) + 1;
		TCHAR* str= (TCHAR*)malloc(ilen*sizeof(TCHAR));
		_tcscpy_s(str,ilen, Effect[i]);
		pFrames->vFiles.push_back(str);
	}
}

bool CEffect::Load()
{
	if (this->Loaded) return true;
	HRESULT hr;

	hr = this->GetDevice()->CreateVertexBuffer(
		6 * sizeof(CEffect::EVertex),
		D3DUSAGE_WRITEONLY,
		FX_FVF,
		D3DPOOL_MANAGED,
		&this->VBuffer,
		NULL
		);	

	EVertex * s;

	if (FAILED(hr)) return false;
	this->VBuffer->Lock(0, 0, (void**)&s, 0);
	EVertex EGeometry[]=
	{
		{D3DXVECTOR3(0, -50, 50),D3DXVECTOR2(0, 0)},
		{D3DXVECTOR3(0, 50, 50),D3DXVECTOR2(1, 0)},
		{D3DXVECTOR3(0, 50, -50),D3DXVECTOR2(1, 1)},
		{D3DXVECTOR3(0, 50, -50),D3DXVECTOR2(1, 1)},
		{D3DXVECTOR3(0, -50, -50),D3DXVECTOR2(0, 1)},
		{D3DXVECTOR3(0, -50, 50),D3DXVECTOR2(0, 0) },
	};	
	memcpy(s, EGeometry, sizeof(EGeometry));
	this->VBuffer->Unlock();

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (this->gfx[i] == nullptr) continue;
		if (this->gfx[i]->Type==0)
		{
			GfxFrames *pGfx = (GfxFrames *)this->gfx[i];
			for (int j = 0; j < pGfx->vFiles.size(); j++)
			{
				LPDIRECT3DTEXTURE9 lpTexturing;
				WebZenTexture(this->GetDevice(), pGfx->vFiles[j], &lpTexturing);
				pGfx->vFrames.push_back(lpTexturing);
			}
		}
		else
		{
			GfxUnique *pGfx = (GfxUnique *)this->gfx[i];
			WebZenTexture(this->GetDevice(), pGfx->szFile, &pGfx->lpTexture);
		}
	}

	this->GetEngine()->LoadEffect(TEXT("GameFx"), TEXT("Effects.fx"), &this->lpEffect);

	this->Loaded = SUCCEEDED(hr);
	if(!this->Loaded) this->Release();
	return this->Loaded;
}

void CEffect::Draw()
{
	if (!this->Loaded) return;
	if (!this->lpEffect) return;
	this->bDrawing = true;

	D3DSURFACE_DESC d3dsd;
	D3DXMATRIX mS, mZ, mR1, mT;
	float thetaZ, thetaX;
	D3DXVECTOR3 Cam = this->GetEngine()->GetStatus()->Eye_Position;
	UINT Passes;

	this->lpEffect->SetFloat("Time", this->GetEngine()->GetStatus()->Time);
	this->GetDevice()->SetStreamSource(0,this->VBuffer,0,sizeof(EVertex));
	this->GetDevice()->SetFVF(FX_FVF);
	this->GetEngine()->PushWorld();

			
	this->lpEffect->Begin(&Passes, 0);
	for (UINT k = 0; k < MAX_EFFECT; k++)
	{
		if (this->gfx[k] == nullptr) continue;
		if (this->gfx[k]->Type == 0)
		{
			GfxFrames * pF = (GfxFrames *)this->gfx[k];
			pF->vFrames[0]->GetLevelDesc(0, &d3dsd);
			float a = (float)d3dsd.Width / (float)d3dsd.Height;
			for (std::list<PointInfo>::iterator it = this->vPoints[k].begin(); it != this->vPoints[k].end(); it++)
			{
				thetaZ = atan2(it->Pos.y - Cam.y, it->Pos.x - Cam.x)-D3DX_PI;
				D3DXMatrixScaling(&mS, it->size*a, it->size, 1);
				D3DXMatrixRotationZ(&mZ, thetaZ);
				//for (UINT Pass = 0; Pass < Passes; Pass++)
				{
					this->lpEffect->BeginPass(0);
					for (std::vector<Particle>::iterator pt = it->vParticles.begin(); pt != it->vParticles.end(); )
					{
						if (pt->Life == 0)
						{
							pt = it->vParticles.erase(pt);
							continue;
						}
						D3DXMatrixTranslation(&mT, pt->Position.x, pt->Position.y, pt->Position.z+25);
						this->GetDevice()->SetTexture(0, pF->vFrames[(int)pt->Frame]);

						this->GetEngine()->SetWorld(&(mS*mZ*mT));
						this->lpEffect->SetFloat("Life", (float)pt->Life / (float)pF->Life);
						this->lpEffect->SetMatrix("mWVP", &this->GetEngine()->GetWVP());
						this->lpEffect->CommitChanges();
						this->GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

						pt->Frame += EFPS*this->GetEngine()->GetStatus()->Elapsed;
						if (pt->Frame >= pF->vFrames.size())
							pt->Frame -= pF->vFrames.size();

						if (pt->Life > (int)(this->GetEngine()->GetStatus()->Elapsed*1000.0))
							pt->Life -= (int)(this->GetEngine()->GetStatus()->Elapsed*1000.0);
						else
							pt->Life = 0;

						pt->Position += pt->Velocity*this->GetEngine()->GetStatus()->Elapsed;
						pt->Velocity += pF->Gravity*this->GetEngine()->GetStatus()->Elapsed;
						pt++;
					}
					this->lpEffect->EndPass();
				}
				if (it->LastParticle < GetTickCount())
				{
					it->LastParticle = GetTickCount() + rand() % pF->Life + 100;
					Particle p;
					p.Frame = rand()% pF->vFrames.size();
					p.Life = pF->Life;
					p.Position = it->Pos;
					float mod = sqrt(it->Velocity.x*it->Velocity.x + it->Velocity.y*it->Velocity.y + it->Velocity.z*it->Velocity.z)*0.1;
					p.Velocity = D3DXVECTOR3(
						Random(-mod, mod) + it->Velocity.x,
						Random(-mod, mod) + it->Velocity.y,
						Random(-mod, mod) + it->Velocity.z
						);
					if(it->vParticles.size()<pF->MaxParticles) it->vParticles.push_back(p);
				}
			}
		}
		else
		{
			GfxUnique * pU = (GfxUnique *)this->gfx[k];
			this->lpEffect->SetFloat("rows", pU->Rows);
			this->lpEffect->SetFloat("cols", pU->Cols);
			this->GetDevice()->SetTexture(0, pU->lpTexture);
			pU->lpTexture->GetLevelDesc(0, &d3dsd);
			float a = (float)d3dsd.Width / (float)d3dsd.Height;
			for (std::list<PointInfo>::iterator it = this->vPoints[k].begin(); it != this->vPoints[k].end(); it++)
			{
				thetaZ = atan2(it->Pos.y - Cam.y, it->Pos.x - Cam.x) - D3DX_PI;
				D3DXMatrixScaling(&mS, it->size*a, it->size, 1);
				D3DXMatrixRotationZ(&mZ, thetaZ);
				//for (UINT Pass = 0; Pass < Passes; Pass++)
				{
					this->lpEffect->BeginPass(1);
					for (std::vector<Particle>::iterator pt = it->vParticles.begin(); pt != it->vParticles.end(); )
					{
						if (pt->Life == 0)
						{
							pt = it->vParticles.erase(pt);
							continue;
						}
						D3DXMatrixTranslation(&mT, pt->Position.x, pt->Position.y, pt->Position.z + 25);

						this->GetEngine()->SetWorld(&(mS*mZ*mT));
						this->lpEffect->SetFloat("Life", (float)pt->Life / (float)pU->Life);
						this->lpEffect->SetMatrix("mWVP",&this->GetEngine()->GetWVP());
						this->lpEffect->SetFloat("row", (float)((int)pt->Frame / (int)pU->Cols));
						this->lpEffect->SetFloat("col", (float)((int)pt->Frame % (int)pU->Cols));
						this->lpEffect->CommitChanges();
						this->GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

						pt->Frame += EFPS*this->GetEngine()->GetStatus()->Elapsed;

						if (pt->Frame >= pU->Frames) pt->Frame = 0;
						if (pt->Life > (int)(this->GetEngine()->GetStatus()->Elapsed*1000.0))
							pt->Life -= (int)(this->GetEngine()->GetStatus()->Elapsed*1000.0);
						else
							pt->Life = 0;

						pt->Position += pt->Velocity*this->GetEngine()->GetStatus()->Elapsed;
						pt->Velocity += pU->Gravity*this->GetEngine()->GetStatus()->Elapsed;
						pt++;
					}
					this->lpEffect->EndPass();
				}
				if (it->LastParticle < GetTickCount())
				{
					it->LastParticle = GetTickCount() + rand() % pU->Life + 100;
					Particle p;
					p.Frame = rand() % pU->Frames;
					p.Life = pU->Life;
					p.Position = it->Pos;
					float mod = sqrt(it->Velocity.x*it->Velocity.x + it->Velocity.y*it->Velocity.y + it->Velocity.z*it->Velocity.z)*0.1;
					p.Velocity = D3DXVECTOR3(
						Random(-mod, mod) + it->Velocity.x,
						Random(-mod, mod) + it->Velocity.y,
						Random(-mod, mod) + it->Velocity.z
						);
					if (it->vParticles.size()<pU->MaxParticles) it->vParticles.push_back(p);
				}
			}
		}
	}
	this->lpEffect->End();
	this->GetEngine()->PopWorld();
	this->bDrawing = false;
}

void CEffect::Release()
{
	this->Loaded = false;
	while (this->bDrawing) Sleep(10);
	this->Flush();
	SAFE_RELEASE(this->VBuffer);
	for (UINT k = 0; k < MAX_EFFECT; k++) {
		if (this->gfx[k] == nullptr) continue;
		if (this->gfx[k]->Type == 1)
		{
			SAFE_RELEASE(((GfxUnique*)this->gfx[k])->lpTexture);
		}
		else
		{
			GfxFrames * p = (GfxFrames*)this->gfx[k];
			int iframes = p->vFrames.size();
			for (int i = 0; i < iframes; i++)
			{
				free(p->vFiles[i]);
				SAFE_RELEASE(p->vFrames[i]);
			}
		}			
		
	}
}

void CEffect::AddPoint(UINT Fx, D3DXVECTOR3 Pos, float Size, D3DXVECTOR3 Vel)
{
	PointInfo p;
	p.Pos = Pos;
	p.size = Size;
	p.vParticles.clear();
	p.Velocity = (Vel!=D3DXVECTOR3(0,0,0))?Vel:this->gfx[Fx]->Velocity;
	p.LastParticle = 0;
	if(Fx<MAX_EFFECT) this->vPoints[Fx].push_back(p);
}

float Random(float fmin, float fmax)
{
	return (fmax - fmin)*((float)rand()/(float)RAND_MAX)+fmin;
}
