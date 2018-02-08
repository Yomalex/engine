#include "stdafx.h"
#include "Model.h"
#include "WZen.h"

D3DVERTEXELEMENT9 decl[] =
{
	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, 32, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
	{ 0, 36, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
	D3DDECL_END()
};

CModel::CModel():cAni(0),cFrame(0)
{
	this->addFrames = 0;
	this->Loaded = false;
	this->AnimationSource = this;
	memset(this->fxMesh, 0, 128);
}

CModel::CModel(const TCHAR * szFileName, int inflate) :cAni(0), cFrame(0)
{
	this->addFrames = inflate;
	this->Loaded = false;
	this->AnimationSource = this;
	memset(this->fxMesh, 0, 128);
	this->SetFile(szFileName);
}

CModel::~CModel()
{
	this->Release();
}

void CModel::SetFile(const TCHAR * szFileName)
{
	_tcscpy_s(this->szFile, szFileName);
}

bool CModel::Load()
{
	if (this->Loaded) return true;

	this->vBounding[0] = D3DXVECTOR3(INFINITE, INFINITE, INFINITE);
	this->vBounding[1] = -D3DXVECTOR3(INFINITE, INFINITE, INFINITE);

	HANDLE hFile = CreateFile(this->szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	DWORD dwSize = GetFileSize(hFile, nullptr);
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

	char * Buffer = (char*)malloc(dwSize), *pBuf;
	ReadFile(hFile, Buffer, dwSize, &dwSize, nullptr);
	CloseHandle(hFile);
	pBuf = Buffer + 4;

	switch (((BMDSig*)Buffer)->Encrypt)
	{
	case BMD_ENCRY:
		if (*(DWORD*)pBuf > dwSize - 8) { free(Buffer); return false; }
		dwSize = *(DWORD*)pBuf;
		pBuf += 4;
		decrypt(pBuf, dwSize);
		break;
	case BMD_NOENCRY:
		break;
	default: free(Buffer);
		return false;
	}

	BMDHeader * Header=(BMDHeader*)pBuf;
	pBuf += sizeof(BMDHeader);

	std::list<MVertex> Vertex;
	for (int i = 0; i < Header->uMeshCount; i++)
		this->LoadMesh(&pBuf, Vertex);

	for (int i = 0; i < Header->uAnimCount; i++)
		this->LoadAnim(&pBuf);

	for (int i = 0; i < Header->uBoneCount; i++)
		this->LoadBone(&pBuf);

	this->VertexC = Vertex.size();
	this->CPURender = this->vBoneF.size() >= 60;

	this->SetAnimation(this);

	if(Vertex.size()>0){
		HRESULT hr =this->GetDevice()->CreateVertexBuffer(
		Vertex.size() * sizeof(MVertex),
			0,
			0,
			D3DPOOL_MANAGED,
			&this->VBuffer,
			NULL
			);

		if (FAILED(hr))
		{
			this->Release();
			this->GetEngine()->LOG(TEXT("Creating Vertex Buffer for '%s'"), this->GetName());
			return false;
		}

		MVertex * v, *v2;

		if (this->CPURender)
		{
			hr = this->GetDevice()->CreateVertexBuffer(
				Vertex.size() * sizeof(MVertex),
				0,
				0,
				D3DPOOL_MANAGED,
				&this->VBufferC,
				NULL
				);

			if (FAILED(hr))
			{
				this->Release();
				this->GetEngine()->LOG(TEXT("Creating Vertex Buffer for '%s'"), this->GetName());
				return false;
			}
		}
		else
		{
			this->VBufferC = this->VBuffer;
		}

		this->VBuffer->Lock(0, 0, (void**)&v, 0);
		D3DXVECTOR4 tmp;
		while (!Vertex.empty())
		{
			*v = Vertex.front();
			D3DXVec3Transform(&tmp, &v->Pos, &this->GetBoneSpace(v->BoneIndex[0]));
			this->vBounding[0].x = min(this->vBounding[0].x, tmp.x);
			this->vBounding[0].y = min(this->vBounding[0].y, tmp.y);
			this->vBounding[0].z = min(this->vBounding[0].z, tmp.z);

			this->vBounding[1].x = max(this->vBounding[1].x, tmp.x);
			this->vBounding[1].y = max(this->vBounding[1].y, tmp.y);
			this->vBounding[1].z = max(this->vBounding[1].z, tmp.z);
			v++;
			Vertex.pop_front();
		}
		this->VBuffer->Unlock();

		hr = this->GetDevice()->CreateVertexDeclaration(decl, &VDModel);

		if (FAILED(hr))
		{
			this->Release();
			this->GetEngine()->LOG(TEXT("Creating Vertex Definicion for '%s'"), this->GetName());
			return false;
		}

		if (FAILED(this->GetEngine()->LoadEffect(TEXT("ModelShader"), TEXT("Model.fx"), &this->lpEffect)))
		{
			this->Release();
			return false;
		}
	}
	this->Loaded = true;
	return true;
}

void CModel::Draw()
{
	if (!this->Loaded) return;
	this->bDrawing = true;
	if (this->CPURender) this->VertexCPUProcess();

	UINT Bones = min(this->vBoneF.size(), MAX_BONE_CALL);
	UINT Passes;
	for (UINT j = 0; j <Bones; j++)
	{
		this->Frame[j] = this->AnimationSource->GetBoneSpace(j);
	}

	this->lpEffect->SetTechnique("Basic");
	this->lpEffect->SetMatrixArray("mBone", this->Frame, Bones);
	float y = this->GetEngine()->GetAmbientLight(LFMT_YBR).r*0.5;
	this->lpEffect->SetFloat("Ambient", y);
	this->lpEffect->SetMatrix("mW", &this->GetEngine()->GetWorld());
	this->lpEffect->SetMatrix("mWVP", &this->GetEngine()->GetWVP());
	this->lpEffect->SetFloat("Time", this->GetEngine()->GetStatus()->Time);
	this->GetDevice()->SetVertexDeclaration(this->VDModel);
	this->GetDevice()->SetStreamSource(0, this->VBufferC, 0, sizeof(MVertex));
	this->lpEffect->Begin(&Passes, 0);
	for (UINT Pass = 0; Pass < Passes; Pass++)
	{
		this->lpEffect->BeginPass(Pass);
		for (UINT i = 0; i < this->vMesh.size(); i++)
		{
			if (this->vMesh[i].Effect != Pass) continue;
			this->GetDevice()->SetTexture(0, this->vMesh[i].Texture);
			this->GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, this->vMesh[i].Offset, this->vMesh[i].uTri);
		}
		this->lpEffect->EndPass();
	}
	this->lpEffect->End();
	this->bDrawing = false;
	this->ModelElapsed += this->GetEngine()->GetStatus()->Elapsed;
	this->SetFrame(this->ModelElapsed*24.0);
}

void CModel::onFrame(float Elapsed)
{
	this->ModelElapsed += Elapsed;
	this->SetFrame(this->ModelElapsed*24.0);
}

void CModel::Release()
{
	this->Loaded = false;
	while (this->bDrawing) Sleep(10);
	SAFE_RELEASE(this->VDModel);
	SAFE_RELEASE(this->VBuffer);
	if (this->CPURender) SAFE_RELEASE(this->VBufferC);
	for (UINT i = 0; i < this->vMesh.size(); i++)
	{
		//SAFE_RELEASE(this->vMesh[i].VBuffer);
		SAFE_RELEASE(this->vMesh[i].Texture);
	}
	for (UINT i = 0; i < this->vAnim.size(); i++)
	{
		free(this->vAnim[i]);
	}
	this->vMesh.clear();
	this->vAnim.clear();
	this->vBoneF.clear();
}

std::vector<BoneF>& CModel::GetAnimation()
{
	return this->vBoneF;
}

D3DXMATRIX CModel::GetAnimation(int bone)
{
	return this->vBoneF[bone].Animations[this->cAni].vLocal[this->cFrame];
}

void CModel::SetAnimation(CModel * Animat)
{
	this->AnimationSource = Animat;
}

void CModel::SetFrame(WORD aNumber, WORD aFrame)
{
	if (!this->Loaded) return;
	this->cAni = min(aNumber, this->AnimationSource->GetAnimCount());
	/*this->cFrame = aFrame%max(1, this->AnimationSource->GetAnimFrames(this->cAni));
	if (this->OnAnimationEnd && this->cFrame != aFrame)
	{
		this->OnAnimationEnd(this);
	}*/
	ModelElapsed = (float)aFrame / 24.0f;
	//this->lastFrame = aFrame;
	this->SetFrame(aFrame);
}

void CModel::SetFrame(WORD aFrame)
{
	if (!this->Loaded) return;
	WORD maxAni = max(1, this->AnimationSource->GetAnimFrames(this->cAni));
	this->cFrame = aFrame%maxAni;
	if (this->OnAnimationEnd && this->cFrame < this->lastFrame%maxAni)
	{
		this->OnAnimationEnd(this);
	}
	this->lastFrame = aFrame;
}

UINT CModel::GetAnimCount()
{
	return this->vAnim.size();
}

UINT CModel::GetAnimFrames(int ani)
{
	if (this->GetAnimCount() < ani) return 0;
	return (this->vBoneF.size() && this->vBoneF[0].Animations.size())?this->vBoneF[0].Animations[ani].vLocal.size() : this->vAnim[ani]->Count + (this->vAnim[ani]->Count-1)*this->addFrames;
}

void CModel::LoadMesh(char ** ppData, std::list<MVertex>& V)
{
#pragma pack(push,1)
	struct BMDVertexHeader
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
		//float x, y, z;
		D3DXVECTOR3 vNor;
		unsigned long id;
	};
	struct BMDtextuv
	{
		float u, v;
	};
	struct BMDTriangle
	{
		BYTE Count;
		BYTE Trash;
		WORD indexVertex[3];
		BYTE uUnknown2[2];//CD
		WORD indexNormal[3];
		BYTE uUnknown3[2];//CD
		WORD indexUV[3];
		BYTE uUnknown[40];//CDCDCDCD
	};
#pragma pack(pop)

	BMDVertexHeader * MeshInfo = (BMDVertexHeader *)*ppData;
	*ppData += sizeof(BMDVertexHeader);

	BMDPos * Vertex = (BMDPos *)*ppData;
	*ppData += sizeof(BMDPos)*MeshInfo->nPos;

	BMDNor * Normal = (BMDNor*)*ppData;
	*ppData += sizeof(BMDNor)*MeshInfo->nNor;

	D3DXVECTOR2 * UV = (D3DXVECTOR2*)*ppData;
	*ppData += sizeof(D3DXVECTOR2)*MeshInfo->nTex;

	BMDTriangle * pTriangle = (BMDTriangle*)*ppData;
	*ppData += sizeof(BMDTriangle)*MeshInfo->nTri;

	TCHAR szTexture[33];/*
#ifdef UNICODE
	char2wchar(szTexture, 33, *ppData);
#else
	memcpy(szTexture, *ppData, 32);
#endif*/

	_tcscpy_s(szTexture, CTString(*ppData));

	*ppData += 32;

	Mesh mesh;
	UINT v, n, t;
	mesh.Offset = V.size();
	mesh.uTri = MeshInfo->nTri;
	mesh.Effect = this->fxMesh[this->vMesh.size()];
	mesh.bmin = 255;
	mesh.bmax = 0;

	for (UINT i = 0; i < MeshInfo->nTri; i++)
	{
		for (UINT j = 0; j < 3; j++)
		{
			v = pTriangle[i].indexVertex[j];
			n = pTriangle[i].indexNormal[j];
			t = pTriangle[i].indexUV[j];

			/*this->vBounding[0].x = min(this->vBounding[0].x, Vertex[v].vPos.x);
			this->vBounding[0].y = min(this->vBounding[0].y, Vertex[v].vPos.y);
			this->vBounding[0].z = min(this->vBounding[0].z, Vertex[v].vPos.z);

			this->vBounding[1].x = max(this->vBounding[1].x, Vertex[v].vPos.x);
			this->vBounding[1].y = max(this->vBounding[1].y, Vertex[v].vPos.y);
			this->vBounding[1].z = max(this->vBounding[1].z, Vertex[v].vPos.z);*/

			MVertex mv = {
				Vertex[v].vPos,
				Normal[n].vNor,
				UV[t],
				1.0,
				{ Vertex[v].iParent, 0,0,0 }
			};

			mesh.bmin = min(mesh.bmin, Vertex[v].iParent);
			mesh.bmax = max(mesh.bmax, Vertex[v].iParent);

			V.push_back(mv);
		}
	}

	TCHAR szTex[MAX_PATH];
	GetFolder(szTex, MAX_PATH, this->szFile);
	_tcscat_s(szTex, szTexture);
	switch (szTex[_tcslen(szTex)-3])
	{
	case 'B':
	case 'b':
		_tcscpy_s(&szTex[_tcslen(szTex) - 3], 5, TEXT("OZB"));
		break;
	case 'j':
	case 'J':
		_tcscpy_s(&szTex[_tcslen(szTex) - 3], 5, TEXT("OZJ"));
		break;
	case 'T':
	case 't':
		_tcscpy_s(&szTex[_tcslen(szTex) - 3], 5, TEXT("OZT"));
		break;
	}
	WebZenTexture(this->GetDevice(), szTex, &mesh.Texture);
	this->vMesh.push_back(mesh);
}

void CModel::LoadAnim(char ** ppData)
{
	Anim * Info = (Anim*)*ppData;
	*ppData += 3;

	DWORD dwbSize = 0;
	if (Info->Flag)
	{
		dwbSize = sizeof(D3DXVECTOR3)*Info->Count + 3;
		*ppData += sizeof(D3DXVECTOR3)*Info->Count;
	}
	else
	{
		dwbSize = sizeof(D3DXVECTOR3)*Info->Count + 3;
	}
	Anim * anim = (Anim*)malloc(dwbSize);
	memcpy(anim, Info, dwbSize);
	this->vAnim.push_back(anim);
}

void CModel::LoadBone(char ** ppData)
{
	BoneF bonef;
	char flag = **ppData;
	*ppData += 1;
	if (flag)
	{
		bonef.iParent = -1;
		bonef.Animations.clear();
		strcpy_s(bonef.sName, "(null)");
		this->vBoneF.push_back(bonef);
		return;
	}

	Bone * bone = (Bone*)*ppData;
	*ppData += sizeof(Bone);

	D3DXVECTOR3 v3Out;
	D3DXMATRIX Local, mT;
	D3DXQUATERNION q1, q2, x, y, z, slerp;
	memcpy(&bonef, bone, sizeof(Bone));
	for (int i = 0; i < this->vAnim.size(); i++)
	{
		AnimF animf;
		animf.Count = this->vAnim[i]->Count;
		D3DXVECTOR3 *vTrans=(D3DXVECTOR3 *)*ppData;
		*ppData += sizeof(D3DXVECTOR3)*this->vAnim[i]->Count;
		D3DXVECTOR3 *vRotat = (D3DXVECTOR3 *)*ppData;
		*ppData += sizeof(D3DXVECTOR3)*this->vAnim[i]->Count;

		for (int j = 0; j < this->vAnim[i]->Count; j++)
		{
			D3DXQuaternionRotationAxis(&x, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), vRotat[j].x);
			D3DXQuaternionRotationAxis(&y, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), vRotat[j].y);
			D3DXQuaternionRotationAxis(&z, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), vRotat[j].z);
			q1 = x*y*z;
			//D3DXMatrixRotationQuaternion(&mT, &D3DXQUATERNION(q1.x, q1.y, q1.z, q1.w));
			D3DXMatrixRotationQuaternion(&mT, &q1);

			D3DXMatrixIdentity(&Local);
			Local *= mT;
			D3DXMatrixTranslation(&mT, vTrans[j].x, vTrans[j].y, vTrans[j].z);
			Local *= mT;

			animf.vLocal.push_back(Local);
			
			if(j+1<this->vAnim[i]->Count) for (int k = 1; k < this->addFrames+1; k++)
			{
				D3DXMatrixIdentity(&Local);
				//D3DXVec3Lerp(&v3Out, &vRotat[j], &vRotat[j + 1], (float)k / (float)(this->addFrames + 1));
				
				D3DXQuaternionRotationAxis(&x, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), vRotat[j].x);
				D3DXQuaternionRotationAxis(&y, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), vRotat[j].y);
				D3DXQuaternionRotationAxis(&z, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), vRotat[j].z);
				q1 = x*y*z;
				D3DXQuaternionRotationAxis(&x, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), vRotat[j+1].x);
				D3DXQuaternionRotationAxis(&y, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), vRotat[j+1].y);
				D3DXQuaternionRotationAxis(&z, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), vRotat[j+1].z);
				q2 = x*y*z;

				D3DXQuaternionSlerp(&slerp, &q1, &q2, (float)k / (float)(this->addFrames + 1));
				D3DXMatrixRotationQuaternion(&mT, &slerp);
				Local *= mT;

				D3DXVec3Lerp(&v3Out, &vTrans[j], &vTrans[j + 1], (float)k / (float)(this->addFrames + 1));
				D3DXMatrixTranslation(&mT, v3Out.x, v3Out.y, v3Out.z);
				Local *= mT;

				animf.vLocal.push_back(Local);
			}
		}
		bonef.Animations.push_back(animf);
	}
	this->vBoneF.push_back(bonef);
}

D3DXMATRIX CModel::GetBoneSpace(int bone)
{
	D3DXMATRIX Parent;
	D3DXMatrixIdentity(&Parent);
	if (bone >= this->vBoneF.size()) return Parent;

	if (this->vBoneF[bone].iParent >= 0) Parent = this->GetBoneSpace(vBoneF[bone].iParent);
	if (this->AnimationSource->GetAnimation()[bone].Animations.size() == 0) return Parent;

	return this->AnimationSource->GetAnimation(bone)/*.Animations[this->cAni].vLocal[this->cFrame]*/*Parent;
}

void CModel::SetMeshEffect(UINT Mesh, UINT Effect)
{
	this->fxMesh[Mesh] = Effect;
}

#include <omp.h>
void CModel::VertexCPUProcess()
{
	static int LastFrame = -1;
	if (this->cFrame!= LastFrame)
	{
		MVertex * vi, *vo;
		D3DXVECTOR4 tmp;
		this->VBuffer->Lock(0, 0, (void**)&vi, 0);
		this->VBufferC->Lock(0, 0, (void**)&vo, 0);
		UINT bones = min(this->vBoneF.size(), 128);

#pragma omp parallel for
		for (int i = 0; i < bones; i++)
		{
			this->Frame[i] = this->GetBoneSpace(i);
		}

#pragma omp parallel for
		for (int i = 0; i < this->VertexC; i++)
		{
			vo[i] = vi[i];
			if (vi[i].BoneIndex[0] < 60) continue;

			D3DXMATRIX * m = this->Frame + vi[i].BoneIndex[0];

			D3DXVec3Transform(&tmp, &vi[i].Pos, m);
			vo[i].Pos = D3DXVECTOR3(tmp.x, tmp.y, tmp.z);
			tmp = D3DXVECTOR4(vi[i].Normal.x, vi[i].Normal.y, vi[i].Normal.z, 0);
			D3DXVec4Transform(&tmp, &tmp, m);
			vo[i].Normal = D3DXVECTOR3(tmp.x, tmp.y, tmp.z);
		}
		this->VBuffer->Unlock();
		this->VBufferC->Unlock();
		LastFrame = this->cFrame;
	}
}
