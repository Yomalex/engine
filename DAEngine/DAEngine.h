#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3dx9.lib")

#include <map>
#include <string>
#include <list>

using namespace std;

#include "EngineObject.h"
#include "Camera.h"
#include "Terrain.h"

class DAEngine
{
	IDirect3D9 * iD3D9;
	IDirect3DDevice9 * iD3DD9;

	D3DCAPS9 m_D3DCaps;
	D3DDEVTYPE m_D3DDType;
	D3DPRESENT_PARAMETERS m_D3DPP;

	map<wstring, D3DDISPLAYMODE> m_mVideoModes;

	wstring m_Resolution;

	HRESULT m_LastError;
	wstring m_sLastError;


	list<EngineObject*> m_RQSolid;
	list<EngineObject*> m_RQTranslucid;
	list<EngineObject*> m_RQGUI;
	list<EngineObject*> m_RQOffScreen;
	list<EngineObject*> m_Load;
	list<EngineObject*> m_Loaded;
	map<wstring, EngineObject*> m_FX;

	D3DXMATRIX mat_ProjOrtho;
	D3DXMATRIX mat_ProjPersp;

	D3DXMATRIX * lpmat_World, *lpmat_View, *lpmat_Proj;
	list<D3DXMATRIX> Transforms;
public:
	int m_iMaxFrameRate;
	double m_fTime;

	Camera * _3DCamera;
	Camera * _2DCamera;

	DAEngine();
	~DAEngine();

	static void SetLeftHand();
	static void SetRightHand();

	bool CreateDevice();
	void WindowMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	int Loop();

	HRESULT SetError(HRESULT hrError);
	EngineObject * Load(EngineObject*obj);

	// Aply World Transform
	HRESULT World(D3DXMATRIX * mat);

	// Rollback World Transform
	HRESULT World();
	D3DXMATRIX* CurWorld();

	// Aply view Transform
	HRESULT View(D3DXMATRIX * mat);

	// Aply proj transform
	HRESULT Projection(D3DXMATRIX * mat);

	void PerspectiveFov(float FieldOfView, float aspect, float fnear, float ffar);
	void OrthogonalOffCenter(float l, float r, float t, float b, float fnear, float ffar);

	IDirect3DDevice9 * Dev() { return this->iD3DD9; }
};

// Redefine D3DXMatrix

// Build a lookat matrix.
extern D3DXMATRIX* (WINAPI *D3DXMatrixLookAt)
(D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye, CONST D3DXVECTOR3 *pAt,
	CONST D3DXVECTOR3 *pUp);

// Build a perspective matrix
extern D3DXMATRIX* (WINAPI *D3DXMatrixPerspective)
(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf);

extern D3DXMATRIX* (WINAPI *D3DXMatrixPerspectiveFov)
(D3DXMATRIX *pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf);

extern D3DXMATRIX* (WINAPI *D3DXMatrixPerspectiveOffCenter)
(D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
	FLOAT zf);

extern D3DXMATRIX* (WINAPI *D3DXMatrixOrtho)
(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf);

extern D3DXMATRIX* (WINAPI *D3DXMatrixOrthoOffCenter)
(D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
	FLOAT zf);