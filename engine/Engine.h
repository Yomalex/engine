#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <d3d9.h>
#include <DxErr.h>
#include <d3dx9.h>
#include <list>
#include <tchar.h>
#include "TString.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9d.lib")
#pragma comment(lib,"dxerr.lib")
#pragma comment(lib,"legacy_stdio_definitions.lib")

#define BACKBUFFERS 3
#define NEAR_PLANE 1.0f
#define FAR_PLANE 100000.0f

#define SAFE_RELEASE(x) if(x) x->Release(); x=nullptr

#ifdef RHAND
#define D3DXMatrixLookAt D3DXMatrixLookAtRH
#define D3DXMatrixPerspectiveFov D3DXMatrixPerspectiveFovRH
#else
#define D3DXMatrixLookAt D3DXMatrixLookAtLH
#define D3DXMatrixPerspectiveFov D3DXMatrixPerspectiveFovLH
#endif

struct _tagResolution
{
	UINT Format;
	TCHAR szFormat[100];
};

struct _tagDevice
{
	PDIRECT3DDEVICE9 Device;
	D3DCAPS9 DeviceCaps;
	D3DFORMAT Format;
	D3DDEVTYPE Type;
	DWORD VertexProcessing;
	UINT VideoModesCount;
	D3DDISPLAYMODE * VideoModes;
	D3DDISPLAYMODE * CurrentMode;
	D3DADAPTER_IDENTIFIER9 AdapterId;
};

struct _tagStatus
{
	//Timing
	float FramesPerSecond;
	float Elapsed;
	float Time;
	//Eye
	D3DXVECTOR3 Eye_Position;
	D3DXVECTOR3 Eye_ViewAt;
	D3DXVECTOR3 Eye_Up;
	float Zoom;
	// Planes
	float plane_near;
	float plane_far;
	// DrawType
	//bool Right_hand;
	// speeds and angles
	float Z_Angle, Z_Speed;
	float X_Angle, X_Speed;
	float F_Speed;
	float L_Speed;
	// Render Status
	bool BindCamera;
	bool DrawLoading;
	bool Fog;
	bool KillSignal;
	bool Focus;
	bool LoadError;
	bool Initialized;
	bool Draw3D;
	CRITICAL_SECTION CSEngine;
	D3DXCOLOR Ambient, AmbientYBR;
	// LoadQueue
	float QueueSize;
	tm now;

	bool Reseting;
	bool Resizing;
	int VideoMode;
	//
	float ReflectQuality;
	float ShadowsQuality;
};

struct _tagEffect
{
	TCHAR szEffect[100];
	LPD3DXEFFECT lpEffect;
};

class CEngine;

class CEngineElement
{
	CEngine * Engine;
	TCHAR szName[256];
	bool LoadSkip;
public:
	CEngineElement();
	~CEngineElement();

	void SetName(const TCHAR*szObjectName);
	void CanSkip(bool enable);
	bool CanSkip();
	const TCHAR * GetName();
	void AsociateEngine(CEngine*eng);
	CEngine * GetEngine();
	PDIRECT3DDEVICE9 GetDevice();
	virtual bool Load() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;
protected:
	bool Loaded, bDrawing;
};

class CEngineLoad :public CEngineElement
{
public:
	virtual void SetProgress(float progress, const TCHAR * szProgress) = 0;
	virtual void OnLostDevice() = 0;
	virtual void OnResetDevice() = 0;
};

struct HRTimer
{
	LARGE_INTEGER Frec, Counter;
	double freq;
	HRTimer()
	{
		this->Reset();
	}
	void Reset()
	{
		QueryPerformanceFrequency(&Frec);
		freq = double(Frec.QuadPart) / 1000.0;
		QueryPerformanceCounter(&Counter);
	}
	double Consumed()
	{
		LARGE_INTEGER End;
		QueryPerformanceCounter(&End);
		return double(End.QuadPart - Counter.QuadPart) / freq/ 1000.0;
	}
};

struct Elapsed
{
	DWORD dwStartTick;
	Elapsed()
	{
		this->Reset();
	}
	void Reset()
	{
		this->dwStartTick = GetTickCount();
	}
	float Consumed()
	{
		float ret = (float)(GetTickCount() - this->dwStartTick) / 1000.0f;
		//this->Reset();
		return ret;
	}
};

struct Plane
{
	D3DXVECTOR3 Normal;
	D3DXVECTOR3 Origin;
	D3DXPLANE _Plane;
	D3DXMATRIX ReflectMatrix;
	D3DXMATRIX View;
	LPDIRECT3DTEXTURE9 RenderTarget;
};

enum ENG_STATUS
{
	Z_Angle,
	Z_Speed,
	X_Angle,
	X_Speed,
	F_Speed,
	L_Speed
};

//typedef void(CALLBACK*OnFrameCallBack)(CEngine*p);
typedef void(CALLBACK*OnEvent)(CEngine*p);

#define E_EVENT(x) void CALLBACK x(CEngine*p)
//#define E_FRAME(x) void CALLBACK x(CEngine*p,float fElapsed)
#define VEC4TO3(vec) (D3DXVECTOR3(vec.x, vec.y, vec.z)/vec.w)
#define dot(a,b) D3DXVec3Dot(&a,&b)

enum LFMT
{
	LFMT_RGB,
	LFMT_YBR,
};

struct _tagCamera
{
	D3DXVECTOR3 Pos, Look, Up;
};

enum Qualities
{
	Q_UHIGH		= 1 << 0,
	Q_HIGH		= 1 << 1,
	Q_MEDIUM	= 1 << 2,
	Q_LOW		= 1 << 3,
	Q_POOR		= 1 << 4,
};

#include "UI.h"
#include "Text.h"

class CEngine
{
	HANDLE hLog, hWorker, hLoadQueue;
#ifdef _DEBUG
	HANDLE hConsole;
#endif
	DWORD dwWorkerId, dwQueueId;
	PDIRECT3D9 d3d;
	_tagDevice Device;
	_tagStatus Status;

	D3DXMATRIX m_World, m_View, m_Project, m_Ortho, m_Identity, m_ViewOrtho;
	D3DXMATRIX m_pcWVP;

	D3DPRESENT_PARAMETERS PresentParameters;
	LPD3DXFONT font_fps;
	std::list<CEngineElement*> vLoadList;
	std::list<CEngineElement*> vLoadeds;
	CEngineLoad* LoadScreen;
	std::list<D3DXMATRIX> vWorld;
	std::list<_tagEffect> vEffects;
	OnEvent OnFrame;
	LPD3DXSPRITE lpSprite;
	std::list<_tagCamera> vCamera;
	HINSTANCE hInst;
	TCHAR szWindowName[128];
	WNDPROC ExterProc;

	std::list<Plane> lReflections;
	D3DXMATRIX m_rProyection, m_rView;
public:

	OnEvent FrameReflections;
	OnEvent FrameShadows;

	CEngine(HINSTANCE hInstance, const TCHAR * szName, int w, int h, bool fullscreen, OnEvent Create, WNDPROC Extern, HICON hIcon=nullptr);
	~CEngine();

	//bool Initialize(HWND hWnd);
	void Kill(bool force=false);
	void SetFullScreen(bool enable);
	void SetVideoMode(UINT Mode);
	void AddLoadQueueElement(CEngineElement*Element);
	void Release(CEngineElement*Element);
	void SetLoadScreen(CEngineLoad*Screen);
	void Release();
	const _tagStatus * GetStatus();
	void GetDevInfo(int&w, int&h);
	bool isFullScreen() { return this->PresentParameters.Windowed == 0; }
	LPDIRECT3DDEVICE9 GetDevice() { return this->Device.Device; }
	void SetStatus(ENG_STATUS Status, float value);
	void SetFogLinear(float start, float end);
	void SetFogDisable();
	int Loop(HACCEL hAccelTable);
	void StartLoad() { this->Status.DrawLoading=true; }
	void SetCallBack(OnEvent cb);
	void BindCamera(bool bind2mouse) { this->Status.BindCamera = bind2mouse; }
	D3DXCOLOR& GetAmbientLight(LFMT fmt);
	void SetAmbientLight(D3DXCOLOR cRGB);
	void SetCameraPos(float x, float y, float z) { this->Status.Eye_Position = D3DXVECTOR3(x, y, z); this->UpdateMatrix(); }
	void SetCameraDir(float x, float y, float z) { this->Status.Eye_ViewAt = D3DXVECTOR3(x, y, z) + this->Status.Eye_Position; this->UpdateMatrix(); }

	Plane * AddReflectionPlane(D3DXVECTOR3 Origin, D3DXVECTOR3 Normal);
	void FlushReflections();

	// Draw Mode
	D3DXMATRIX& GetWorld();
	D3DXMATRIX& GetWVP();
	D3DXMATRIX& GetWRP(Plane*p);
	void PushWorld(D3DXMATRIX *World=nullptr);
	void PushCamera(D3DXVECTOR3 Pos, D3DXVECTOR3 Look, D3DXVECTOR3 Up);
	void PopWorld();
	void PopCamera();
	void SetWorld(D3DXMATRIX * m);
	void SetOrthoMatrix(bool update = true);
	void SetProjectionMatrix(bool update = true);
	void SetRenderTarget(LPDIRECT3DTEXTURE9 Texture);
	void SetBackBufferTarget();
	//void UseRightHand(bool enable = true);
	void SetFarNearPlanes(float far_plane=FAR_PLANE,float near_plane=NEAR_PLANE);
	LPD3DXSPRITE Sprite() { return this->lpSprite; }
	void Text(const TCHAR * szText, RECT * rect, DWORD Color, DWORD Flags = DT_LEFT | DT_NOCLIP);

	HRESULT LoadEffect(const TCHAR * EffectKey, const TCHAR * EffectFile, LPD3DXEFFECT * ppEffect);

	std::list<_tagResolution> GetResolutionList();
	OnEvent OnCreate;
	OnEvent OnReset;
	CUI * GUI;
	CText *TextManager;
#ifdef UNICODE
#define LOG(x,...) Log(TEXT("Error in %s: %s"),__FUNCTIONW__, __VA_ARGS__)
#else
#define LOG(x,...) Log(TEXT("Error in %s: %s"),__FUNCTION__, __VA_ARGS__)
#endif
#define Dev() Device.Device
	void Log(const TCHAR * szError, ...);
private:
	void ResetWindow();
	void DefaultOnFrame(float elapsed);
	void GetDeviceFormat(D3DDEVTYPE*DevType, D3DFORMAT*Format);
	void CloseThreads();
	void UpdateMatrix();
	LPDIRECT3DSURFACE9 BackBuffer;
	friend DWORD WINAPI thEngineWorker(void * lpEngine);
	friend DWORD WINAPI thEngineLoadQueue(void * lpEngine);
	friend CEngineElement;
	friend LRESULT CALLBACK D3D9WndProc(HWND, UINT, WPARAM, LPARAM);
};

inline void char2wcharW(wchar_t*out, int size, const char*in)
{
	size_t Converted;
	mbstowcs_s(&Converted, out, size, in, size - 1);
}

#endif