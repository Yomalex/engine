#include "stdafx.h"
#include "WZModel.h"
#include "WZen.h"

//Helper function
template<typename _T>
_T * ReadStream(PBYTE& pFile, _T& Dest)
{
	static DWORD dwSize;
	dwSize = sizeof(Dest);

	memcpy(&Dest, pFile, dwSize);
	pFile += dwSize;

	return &Dest;
}
template<typename _T>
_T * LinkStream(PBYTE& pFile, int repeat=1)
{
	_T * ret = (_T*)pFile;
	pFile += sizeof(_T)*repeat;
	return ret;
}

D3DVERTEXELEMENT9 WZModelDecl[] =
{
	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, 32, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
	{ 0, 36, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
	D3DDECL_END()
};

LPDIRECT3DVERTEXDECLARATION9 WZModelVDecl = nullptr;

D3DXMATRIX BuildLocal(D3DXVECTOR3& vTrans, D3DXVECTOR3& vRot)
{
	static D3DXQUATERNION q1, q2, x, y, z, slerp;
	static D3DXMATRIX mT, mR;
	D3DXQuaternionRotationAxis(&x, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), vRot.x);
	D3DXQuaternionRotationAxis(&y, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), vRot.y);
	D3DXQuaternionRotationAxis(&z, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), vRot.z);
	q1 = x*y*z;

	//D3DXMatrixIdentity(&Local);
	D3DXMatrixRotationQuaternion(&mR, &q1);
	D3DXMatrixTranslation(&mT, vTrans.x, vTrans.y, vTrans.z);

	return mR*mT;
}

WZModel::WZModel(const TCHAR * szFileName)
{
	this->bValid = false;
	this->LastFrame = -1;
	ZeroMemory(this->fxMesh, sizeof(this->fxMesh));

	_tcscpy_s(szPath, szFileName);
}


WZModel::~WZModel()
{
}

void WZModel::SetAnimation(BYTE Number)
{
	if (Number >= this->Source->AnimationList.size())
		return;

	this->StartTime = this->GetEngine()->GetStatus()->Time;

	Animation = Number;
}

#include <omp.h>
void WZModel::CPUHelp()
{
	BMDMeshVertex *src, *dest;
	this->Vertex->Lock(0, 0, (void**)&src, 0);
	this->VertexDraw->Lock(0, 0, (void**)&dest, 0);
	D3DXVECTOR4 tmp;

#pragma omp parallel for
	for (UINT i =0; i<VertexCount; i++)
	{
		if (src[i].Bone[0] < 60 || 
			src[i].Bone[0] > this->BoneList.size()) 
			continue;

		D3DXMATRIX * m = this->Frame + src[i].Bone[0];

		D3DXVec3Transform(&tmp, &src[i].Position, m);
		dest[i].Position = D3DXVECTOR3(tmp.x, tmp.y, tmp.z);
		
		D3DXVec3Transform(&tmp, &src[i].Normal, m);
		dest[i].Normal = D3DXVECTOR3(tmp.x, tmp.y, tmp.z);
	}
	this->Vertex->Unlock();
	this->VertexDraw->Unlock();
}

bool WZModel::Load()
{
	HANDLE hBMD = CreateFile(szPath, GENERIC_READ, 
		FILE_SHARE_READ | FILE_SHARE_DELETE,
		nullptr, OPEN_EXISTING, 0, NULL);

	if (hBMD == INVALID_HANDLE_VALUE)
	{
		this->Error = GetLastError();
		return false;
	}

	// Load file in ram
	this->FileSize = GetFileSize(hBMD, nullptr);
	BYTE * File = (BYTE*)malloc(this->FileSize);
	BYTE * Heap = File;
	ReadFile(hBMD, File, this->FileSize, &this->FileSize, nullptr);
	CloseHandle(hBMD);

	this->EncryptSize = 0;

	ReadStream(File, this->Signature);

	if (Signature.Encrypt == BMD_ENCRY)
	{
		ReadStream(File, this->EncryptSize);
		if (this->EncryptSize + 8 > this->FileSize)
		{
			this->Error = 0;
			return false;
		}
		decrypt((char*)File, this->EncryptSize);
	}
	else if (Signature.Encrypt != BMD_NOENCRY)
	{
		return false;
	}

	ReadStream(File, this->FileInfo);

	BMDMeshHeader MeshInfo;
	BMDPos * MeshPositions;
	BMDNor * MeshNormals;
	D3DXVECTOR2 * MeshUVs;
	BMDTriangle * MeshTriangles;
	//char szTexture[32];

	BMDMesh WZMeshInfo = { 0 };
	BMDMeshVertex vertex;
	std::list<BMDMeshVertex> ListOfVertex;

	for (int i = 0; i < this->FileInfo.uMeshCount; i++)
	{
		ReadStream(File, MeshInfo);
		MeshPositions = LinkStream<BMDPos>(File, MeshInfo.nPos);
		MeshNormals = LinkStream<BMDNor>(File, MeshInfo.nNor);
		MeshUVs = LinkStream<D3DXVECTOR2>(File, MeshInfo.nTex);
		MeshTriangles = LinkStream<BMDTriangle>(File, MeshInfo.nTri);
		ReadStream(File, WZMeshInfo.TextureName);

		WZMeshInfo.Index = i;
		WZMeshInfo.Offset += WZMeshInfo.PrimitiveCount*3;
		WZMeshInfo.PrimitiveCount = MeshInfo.nTri;
		WZMeshInfo.FirstBone = 255;
		WZMeshInfo.LastBone = 0;
		WZMeshInfo.Effect = fxMesh[i];

		for (UINT i = 0; i < MeshInfo.nTri; i++)
		{
			BMDTriangle& t = *MeshTriangles;
			MeshTriangles++;

			for (UINT j = 0; j < 3; j++)
			{
				vertex.Position = MeshPositions[t.indexVertex[j]].vPos;
				vertex.Bone[0] = MeshPositions[t.indexVertex[j]].iParent;
				vertex.Normal = MeshNormals[t.indexNormal[j]].vNor;
				vertex.UV = MeshUVs[t.indexUV[j]];
				vertex.w = 1.0;

				WZMeshInfo.FirstBone = min(WZMeshInfo.FirstBone, vertex.Bone[0]);
				WZMeshInfo.LastBone = max(WZMeshInfo.LastBone, vertex.Bone[0]);

				ListOfVertex.push_back(vertex);
			}
		}

		this->MeshList.push_back(WZMeshInfo);
	}

	BMDAnimationHeader Animation;
	std::list<BMDAnimationHeader> lAnimationList;
	for (int i = 0; i < this->FileInfo.uAnimCount; i++)
	{
		ReadStream(File, Animation);
		lAnimationList.push_back(Animation);

		if (!Animation.Flag)
			continue;

		// Skip all vectors
		LinkStream<D3DXVECTOR3>(File, Animation.Count);
	}

	BYTE EmptyBone;
	BMDBone bone;

	// bones -> Animations -> Frames/localmatrix
	std::vector<std::vector<std::vector<D3DXMATRIX>>> bones;
	for (int i = 0; i < this->FileInfo.uBoneCount; i++)
	{
		ReadStream(File, EmptyBone);
		std::vector<std::vector<D3DXMATRIX>> boneAnimations;
		boneAnimations.clear();

		if (EmptyBone)
		{
			bone.iParent = -1;
			strcpy_s(bone.szName, "(null)");
		}
		else
		{
			ReadStream(File, bone);

			for (auto&it : lAnimationList)
			{
				D3DXVECTOR3 * vTrans = LinkStream<D3DXVECTOR3>(File, it.Count);
				D3DXVECTOR3 * vRotat = LinkStream<D3DXVECTOR3>(File, it.Count);

				std::vector<D3DXMATRIX> boneFrames;
				for (UINT j = 0; j<it.Count; j++)
				{
					D3DXMATRIX Local = BuildLocal(vTrans[j], vRotat[j]);
					boneFrames.push_back(Local);
				}
				boneAnimations.push_back(boneFrames);
			}
		}

		this->BoneList.push_back(bone);
		bones.push_back(boneAnimations);
	}

	free(Heap);
	File = nullptr;

	// Precalcular
	int iAni = 0;
	// AnimationLst->Animation->Frames->Bones
	for (auto&ani : lAnimationList)
	{
		std::vector<std::vector<D3DXMATRIX>> Animation;
		for (int i = 0; i<ani.Count; i++)
		{
			std::vector<D3DXMATRIX> CurrentFrame;
			int index = 0;
			for (auto&bon : this->BoneList)
			{
				//Bone
				D3DXMATRIX output;
				D3DXMatrixIdentity(&output);
				if (bones[index].size() > iAni)
				{
					output *= bones[index][iAni][i];
				}

				if (bon.iParent != -1)
				{
					output *= CurrentFrame[bon.iParent];
				}

				CurrentFrame.push_back(output);
				index++;
			}
			Animation.push_back(CurrentFrame);
		}
		AnimationList.push_back(Animation);
		iAni++;
	}

	D3DXVECTOR3 bMin(10000, 1000, 1000), bMax(-1000, -1000, -1000);
	if (!ListOfVertex.empty())
	{
		VertexCount = ListOfVertex.size();

		HRESULT hr = this->GetDevice()->CreateVertexBuffer(
			ListOfVertex.size() * sizeof(BMDMeshVertex),
			0,
			0,
			D3DPOOL_MANAGED,
			&this->Vertex,
			NULL
		);

		if (FAILED(hr))
		{
			this->Release();
			this->GetEngine()->Log(
				TEXT("Creating Vertex Buffer for '%s'"), this->GetName());
			return false;
		}

		this->VertexDraw = this->Vertex;

		if (this->BoneList.size() >= 60)
		{
			HRESULT hr = this->GetDevice()->CreateVertexBuffer(
				ListOfVertex.size() * sizeof(BMDMeshVertex),
				0,
				0,
				D3DPOOL_MANAGED,
				&this->VertexDraw,
				NULL
			);

			if (FAILED(hr))
			{
				this->Release();
				this->GetEngine()->Log(
					TEXT("Creating Vertex Buffer for '%s'"), this->GetName());
				return false;
			}
		}


		D3DXVECTOR4 tmp;
		BMDMeshVertex * v, *v2;
		if (this->BoneList.size() >= 60)
		{
			this->Vertex->Lock(0, 0, (void**)&v, 0);
			this->VertexDraw->Lock(0, 0, (void**)&v2, 0);
			while (!ListOfVertex.empty())
			{
				*v = ListOfVertex.front();
				*v2 = *v;
				ListOfVertex.pop_front();

				if (this->AnimationList[0][0].size() > v->Bone[0])
					D3DXVec3Transform(&tmp, &v->Position,
						&this->AnimationList[0][0][v->Bone[0]]);
				else
					this->GetEngine()->Log(
						TEXT("Error Bone out of range (%s:%d)"), 
						this->szPath, v->Bone[0]);

				v++;

				bMin.x = min(bMin.x, tmp.x);
				bMin.y = min(bMin.y, tmp.y);
				bMin.z = min(bMin.z, tmp.z);

				bMax.x = max(bMax.x, tmp.x);
				bMax.y = max(bMax.y, tmp.y);
				bMax.z = max(bMax.z, tmp.z);
			}
			this->Vertex->Unlock();
			this->VertexDraw->Unlock();
		}
		else
		{
			this->Vertex->Lock(0, 0, (void**)&v, 0);
			while (!ListOfVertex.empty())
			{
				*v = ListOfVertex.front();
				ListOfVertex.pop_front();

				if (this->AnimationList[0][0].size() > v->Bone[0])
					D3DXVec3Transform(&tmp, &v->Position,
						&this->AnimationList[0][0][v->Bone[0]]);
				else
					this->GetEngine()->Log(
						TEXT("Error Bone out of range (%s:%d)"), 
						this->szPath, v->Bone[0]);

				v++;

				bMin.x = min(bMin.x, tmp.x);
				bMin.y = min(bMin.y, tmp.y);
				bMin.z = min(bMin.z, tmp.z);

				bMax.x = max(bMax.x, tmp.x);
				bMax.y = max(bMax.y, tmp.y);
				bMax.z = max(bMax.z, tmp.z);
			}
			this->Vertex->Unlock();
		}

		if (WZModelVDecl == nullptr)
		{
			hr = this->GetDevice()
				->CreateVertexDeclaration(WZModelDecl, &WZModelVDecl);

			if (FAILED(hr))
			{
				this->Release();
				this->GetEngine()->LOG(
					TEXT("Creating Vertex Definicion for '%s'"), this->GetName());
				return false;
			}
		}

		if (FAILED(this->GetEngine()
			->LoadEffect(TEXT("ModelShader"), TEXT("Model.fx"), &this->lpEffect)))
		{
			this->Release();
			return false;
		}
	}

	this->bValid = true;
	this->LastFrame = -1;
	this->StartTime = -1;

	for (auto&mesh:this->MeshList)
	{
		TCHAR szTex[MAX_PATH];
		GetFolder(szTex, MAX_PATH, szPath);
		_tcscat_s(szTex, CTString(mesh.TextureName));
		switch (szTex[_tcslen(szTex) - 3])
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
	}
	
	this->Source = this;

	return true;
}

void WZModel::Draw()
{
	if (!this->bValid) return;
	const double& Time = this->GetEngine()->GetStatus()->Time;
	if (this->StartTime == -1)
	{
		this->StartTime = Time;
	}

	DWORD Drawin = (Time - this->StartTime)*22.0f; // fps to frame
	Drawin %= this->Source->AnimationList[this->Animation].size();
	UINT Bones = min(this->BoneList.size(), 60);

	if (this->LastFrame != Drawin)
	{
		this->LastFrame = Drawin;
		auto pFrame = this->Frame;
		auto&it = 
			this->Source->AnimationList[this->Animation][this->LastFrame];
		for(int i = 0; i < Bones; i++)
		{
			*pFrame = it[i];
			pFrame++;
		}

		if (Bones == 60)
			this->CPUHelp();
	}

	this->GetDevice()->SetVertexDeclaration(WZModelVDecl);
	this->GetDevice()->SetStreamSource(0, 
		this->VertexDraw, 0, sizeof(BMDMeshVertex));

	// Inicializando Shader
	this->lpEffect->SetTechnique("Basic");
	this->lpEffect->SetMatrixArray("mBone", this->Frame, Bones);
	float y = this->GetEngine()->GetAmbientLight(LFMT_YBR).r*0.5;
	this->lpEffect->SetFloat("Ambient", y);
	this->lpEffect->SetMatrix("mW", &this->GetEngine()->GetWorld());
	this->lpEffect->SetMatrix("mWVP", &this->GetEngine()->GetWVP());
	this->lpEffect->SetFloat("Time", Time);

	UINT Passes;
	this->lpEffect->Begin(&Passes, 0);
	for (UINT Pass = 0; Pass < Passes; Pass++)
	{
		this->lpEffect->BeginPass(Pass);
		for (auto&mesh : this->MeshList)
		{
			if (fxMesh[mesh.Index] != Pass) continue;
			this->GetDevice()->SetTexture(0, mesh.Texture);
			this->GetDevice()->DrawPrimitive(
				D3DPT_TRIANGLELIST, mesh.Offset, mesh.PrimitiveCount);
		}
		this->lpEffect->EndPass();
	}
	this->lpEffect->End();
}

void WZModel::Release()
{
	SAFE_RELEASE(this->Vertex);
	if(this->BoneList.size()>=60)
		SAFE_RELEASE(this->VertexDraw);
}
