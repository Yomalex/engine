#include "stdafx.h"
#include "Engine.h"
#include <time.h>

CEngine * g_Engine;
const TCHAR * EngineClass = TEXT("D3D9Engine");

DWORD WINAPI thEngineLoadQueue(void * lpEngine)
{
	CEngine *eng = (CEngine*)lpEngine;
	Elapsed ElementTime;
	float total, count;
	while (!eng->Status.Initialized) Sleep(100);
	while (!eng->Status.KillSignal)
	{
		total = eng->Status.QueueSize;		
		count = 0;
		if (!eng->vLoadList.empty()) {
			if(eng->Status.DrawLoading)
			{
				if (eng->LoadScreen == nullptr)
				{
					eng->Log(TEXT("LoadScreen not defined"));
					eng->Status.DrawLoading = false;
				}
				else
				{
					eng->Log(TEXT("Load Screen Enable"));
					eng->LoadScreen->AsociateEngine(eng);
					eng->LoadScreen->Load();
					EnterCriticalSection(&eng->Status.CSEngine);
				}
			}
			while (!eng->vLoadList.empty())
			{
				CEngineElement* ee = eng->vLoadList.front();
				ElementTime.Reset();
				count += 1.0f;
				if (eng->Status.DrawLoading)
				{
					eng->LoadScreen->SetProgress(count / total, ee->GetName());
					eng->Dev()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0, 0, 0, 1), 1.0f, 0);
					eng->Dev()->BeginScene();
					eng->SetOrthoMatrix();
					eng->LoadScreen->Draw();
					eng->Dev()->EndScene();
					eng->Dev()->Present(nullptr, nullptr, NULL, nullptr);
				}
				if (!ee->Load())
				{
					eng->vLoadList.pop_front();
					eng->Status.LoadError = true;
					eng->LOG(TEXT("'%s' can't be loaded"), ee->GetName());
					continue;
				}
				total = eng->Status.QueueSize;
				eng->Log(TEXT("Loaded %s in %f seg %d/%d"), ee->GetName(), ElementTime.Consumed(), (int)count, (int)total);
				eng->vLoadeds.push_back(ee);
				eng->vLoadList.pop_front();
			}

			total = eng->Status.QueueSize = 0;

			if (eng->Status.DrawLoading)
			{
				LeaveCriticalSection(&eng->Status.CSEngine);
				eng->Status.DrawLoading = false;
				eng->LoadScreen->Release();
				//eng->LoadScreen = nullptr;
			}
		}
		count = 0;
		Sleep(250);
	}
	return 0x00;
}

DWORD WINAPI thEngineWorker(void * lpEngine)
{
	CEngine *eng = (CEngine*)lpEngine;

	float fElapsed,fNeed=1.0f/((float)eng->Device.CurrentMode->RefreshRate);

	while (!eng->Status.Initialized) Sleep(100);
	std::list<CEngineElement*>::iterator it;

	// Load Mode
	while (!eng->vLoadList.empty())
	{
		Sleep(100);
	}

	// Draw Mode
	Elapsed Draw;
	Sleep(100);
	while ( !eng->Status.KillSignal)
	{
		fElapsed = Draw.Consumed();
		if (fElapsed < fNeed)
		{
			Sleep((fNeed - fElapsed)*1000.0f);
			continue;
		}
		Draw.Reset();

		// Time status save
		eng->Status.Elapsed = fElapsed;
		eng->Status.FramesPerSecond = 1.0f / fElapsed;
		eng->Status.Time += fElapsed;

		//render work
		eng->Dev()->Clear(1, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(1, 1, 1, 1), 1, 0);
		eng->Dev()->BeginScene();
		eng->SetProjectionMatrix();
		/*for (it = eng->vLoadeds.begin(); it != eng->vLoadeds.end(); it++)
		{
			if (eng->Status.KillSignal) break;
			(*it)->Draw(fElapsed);
		}*/

		for (auto&it : eng->vLoadeds)
		{
			it->Draw();
		}
		eng->Dev()->EndScene();
		eng->Dev()->Present(nullptr, nullptr, NULL, nullptr);
		eng->UpdateMatrix();
	}

	return 0;
}

const TCHAR * Vendor(DWORD dwVendor)
{
	switch (dwVendor)
	{
	case 0x10DE: return TEXT("nVidea");
	case 0x1002:
	case 0x1022: return TEXT("AMD");
	case 0x163C:
	case 0x8086:
	case 0x8087: return TEXT("Intel");
	}
	return TEXT("UNKNOWN");
}

LRESULT CALLBACK D3D9WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (g_Engine->GUI->WndProtocol(message, wParam, lParam))
	{
		return TRUE;
	}
	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_CLOSE:
		g_Engine->Kill();
		return 0;
	}

	if(g_Engine->ExterProc(hWnd, message, wParam, lParam))
		return DefWindowProc(hWnd, message, wParam, lParam);
	else
		return 0;
}

CEngine::CEngine(HINSTANCE hInstance, const TCHAR * szName, int w, int h, bool fullscreen, OnEvent Create, WNDPROC Extern, HICON hIcon)
{
	g_Engine = this;
	this->hInst = hInstance;
	_tcscpy_s(this->szWindowName, szName);
	this->Status.Initialized = false;
	this->Status.LoadError = false;
	this->Status.KillSignal = false;
	this->Status.DrawLoading = true;
	this->Status.QueueSize = 0.0f;
	this->Status.ReflectQuality = Q_MEDIUM;
	this->Status.ShadowsQuality = Q_LOW;
	InitializeCriticalSection(&this->Status.CSEngine);
	this->OnFrame = nullptr;
	this->OnCreate = Create;
	this->TextManager = new CText(this);
	this->GUI = new CUI(this);

#ifdef _DEBUG
	AllocConsole();
	this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

	TCHAR szLog[MAX_PATH];
	time_t ttime;
	time(&ttime);
	tm ltime;
	localtime_s(&this->Status.now, &ttime);
	this->Status.now.tm_year += 1900;
	this->Status.now.tm_mon += 1;
	_stprintf_s(
		szLog, 
		TEXT("Eng_%d%d%d.log"), 
		this->Status.now.tm_year,
		this->Status.now.tm_mon,
		this->Status.now.tm_mday
		);
	this->hLog = CreateFile(szLog, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
#ifdef UNICODE
	BYTE Unicode[] = { 0xff,0xfe };
	DWORD dwWrited;
	WriteFile(this->hLog, Unicode, 2, &dwWrited, nullptr);
#endif

	this->Status.Eye_Position = D3DXVECTOR3(1000, 1000, 500);
	this->Status.Eye_ViewAt = D3DXVECTOR3(1500, 1500, 0);
	this->Status.Eye_Up = D3DXVECTOR3(0, 0, 1);
	this->Status.Zoom = 1;
	//this->SetFarNearPlanes();
	this->Status.plane_far = FAR_PLANE;
	this->Status.plane_near = NEAR_PLANE;
	this->d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (this->d3d == nullptr)
	{
		this->Log(TEXT("Error %d: Loading D3D"), GetLastError());
		return;
	}

	this->d3d->GetAdapterIdentifier(D3DADAPTER_DEFAULT, D3DENUM_WHQL_LEVEL, &this->Device.AdapterId);

	this->Log(TEXT("Video Info"));
	this->Log(TEXT("Vendor: %s"), Vendor(this->Device.AdapterId.VendorId));
	this->Log(TEXT("Description: %s"), (const TCHAR*)CTString(this->Device.AdapterId.Description));
	this->Log(TEXT("Driver: %s"), (const TCHAR*)CTString(this->Device.AdapterId.Driver));

	this->Device.Device = nullptr;
	this->GetDeviceFormat(&this->Device.Type, &this->Device.Format);
	this->d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, this->Device.Type, &this->Device.DeviceCaps);

	D3DDISPLAYMODE tmpdm,*pMode;
	int Count = this->d3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, this->Device.Format);
	this->Device.CurrentMode = this->Device.VideoModes;

	std::list<D3DDISPLAYMODE> vModes;

	for (UINT i = 0; i < Count; i++)
	{
		this->d3d->EnumAdapterModes(D3DADAPTER_DEFAULT, this->Device.Format, i, &tmpdm);
		if (tmpdm.Width<640 || tmpdm.Height<480) continue;
		vModes.push_back(tmpdm);
	}

	this->Device.VideoModesCount = vModes.size();
	pMode = this->Device.VideoModes = (D3DDISPLAYMODE*)malloc(this->Device.VideoModesCount*sizeof(D3DDISPLAYMODE));

	while(!vModes.empty())
	{
		memcpy(pMode,&vModes.front(),sizeof(D3DDISPLAYMODE));
		vModes.pop_front();
		pMode++;
	}
	
	this->Status.VideoMode = -1;
	for (UINT i = 0; i < this->Device.VideoModesCount; i++)
	{
		if (this->Device.VideoModes[i].Width != w || this->Device.VideoModes[i].Height != h) continue;
		this->Device.CurrentMode = this->Device.VideoModes + i;
		this->Status.VideoMode = i;
		break;
	}

	if(this->Status.VideoMode == -1)
	for (UINT i = 0; i < this->Device.VideoModesCount; i++)
	{
		if (this->Device.VideoModes[i].Width < 640) continue;
		this->Device.CurrentMode = this->Device.VideoModes + i;
		this->Status.VideoMode = i;
		break;
	}
	if (this->Device.DeviceCaps.DevCaps& D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		this->Device.VertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		this->Log(TEXT("Device Support HLT"));
	}
	else
	{
		this->Device.VertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	//this->Device.VertexProcessing |= ((this->Device.DeviceCaps.DevCaps & D3DDEVCAPS_PUREDEVICE) ? D3DCREATE_PUREDEVICE : 0);

	ZeroMemory(&this->PresentParameters, sizeof(D3DPRESENT_PARAMETERS));
	this->PresentParameters.BackBufferCount = BACKBUFFERS;
	this->PresentParameters.BackBufferFormat = this->Device.CurrentMode->Format;
	this->PresentParameters.BackBufferWidth = this->Device.CurrentMode->Width;
	this->PresentParameters.BackBufferHeight = this->Device.CurrentMode->Height;
	this->PresentParameters.Windowed = (fullscreen)?FALSE:TRUE;
	this->PresentParameters.FullScreen_RefreshRateInHz = (this->PresentParameters.Windowed)?0:this->Device.CurrentMode->RefreshRate;
	this->PresentParameters.hDeviceWindow = NULL;
	this->PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	this->PresentParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
	this->PresentParameters.EnableAutoDepthStencil = TRUE;
	this->PresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//D3DPRESENT_INTERVAL_DEFAULT;// 
	
	this->hLoadQueue = CreateThread(nullptr, 0, thEngineLoadQueue, this, 0, &this->dwQueueId);

	D3DXMatrixIdentity(&this->m_World);
	this->OnReset = nullptr;
	this->UpdateMatrix();

	WNDCLASSEX D3DWnd = {0};
	D3DWnd.cbSize = sizeof(WNDCLASSEX);
	D3DWnd.lpszClassName = EngineClass;
	D3DWnd.lpfnWndProc = D3D9WndProc;
	D3DWnd.hInstance = this->hInst;
	D3DWnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	D3DWnd.hCursor = LoadCursor(nullptr, IDC_ARROW);
	D3DWnd.lpszMenuName = NULL;
	D3DWnd.hIcon = hIcon;
	D3DWnd.hIconSm = hIcon;
	D3DWnd.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&D3DWnd);

	this->ExterProc = Extern;

	RECT wr = { 0, 0, this->PresentParameters.BackBufferWidth, this->PresentParameters.BackBufferHeight };    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size
	HWND hWnd = CreateWindow(EngineClass, this->szWindowName, WS_OVERLAPPEDWINDOW, 0, 0, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, this->hInst, NULL);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	this->PresentParameters.hDeviceWindow = hWnd;

	HRESULT hr = this->d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		this->Device.Type,
		this->PresentParameters.hDeviceWindow,
		this->Device.VertexProcessing,
		&this->PresentParameters,
		&this->Device.Device
		);

	if (FAILED(hr))
	{
		this->Log(TEXT("Can't Create D3DDevice %08x"), hr);
	}

	D3DXCreateFont(
		this->Dev(),
		16,
		0,
		FW_NORMAL,
		1,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		TEXT("Arial"),
		&this->font_fps);

	this->Status.Initialized = true;

	this->Dev()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->Dev()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	this->Dev()->SetRenderState(D3DRS_LIGHTING, FALSE);
	this->Dev()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->Dev()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DXCreateSprite(this->Dev(), &this->lpSprite);
	this->GUI->Initialize();
	if (this->OnCreate) this->OnCreate(this);
}

CEngine::~CEngine()
{
	this->CloseThreads();
	this->Release();
	CloseHandle(this->hLog);
	delete this->GUI;
	delete this->TextManager;
	free(this->Device.VideoModes);
	this->Device.VideoModesCount = 0;
	SAFE_RELEASE(this->Device.Device);
	SAFE_RELEASE(this->d3d);
}

/*bool CEngine::Initialize(HWND hWnd)
{
	if (this->Status.Initialized) return true;

	this->Status.LoadError = false;
	this->PresentParameters.hDeviceWindow = hWnd;
	HRESULT hr = this->d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		this->Device.Type,
		this->PresentParameters.hDeviceWindow,
		this->Device.VertexProcessing,
		&this->PresentParameters,
		&this->Device.Device
		);

	if (FAILED(hr))
	{
		this->Log(TEXT("Can't Create D3DDevice %08x"), hr);
		return false;
	}

	D3DXCreateFont(
		this->Dev(),
		16,
		0,
		FW_NORMAL,
		1,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		TEXT("Arial"),
		&this->font_fps);

	this->Status.Initialized = true;
	/*while(!this->vLoadList.empty() && this->Status.LoadError==false)
	{
		if (this->Status.KillSignal) return false;
		Sleep(1000);
	}/
	this->Dev()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->Dev()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	this->Dev()->SetRenderState(D3DRS_LIGHTING, FALSE);
	this->Dev()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->Dev()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	RECT wr = { 0, 0, this->PresentParameters.BackBufferWidth, this->PresentParameters.BackBufferHeight };    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size
	SetWindowPos(hWnd, NULL, 0, 0, wr.right - wr.left, wr.bottom - wr.top, SWP_NOMOVE | SWP_NOACTIVATE);

	D3DXCreateSprite(this->Dev(), &this->lpSprite);
	this->GUI->Initialize();
	if(this->OnCreate) this->OnCreate(this);

	return !this->Status.LoadError;
}*/

void CEngine::Kill(bool force)
{
	this->Status.KillSignal = true;
	if (force)
	{
		this->Log(TEXT("Force kill called"));
		this->CloseThreads();
		return;
	}

	this->Log(TEXT("kill called"));
}

void CEngine::SetFullScreen(bool enable)
{
	if (this->PresentParameters.Windowed != (BOOL)enable) return;

	this->PresentParameters.Windowed = !enable;
	this->PresentParameters.BackBufferFormat = (this->PresentParameters.Windowed) ? D3DFMT_UNKNOWN : this->Device.CurrentMode->Format;
	this->PresentParameters.FullScreen_RefreshRateInHz = (this->PresentParameters.Windowed) ? 0 : this->Device.CurrentMode->RefreshRate;

	this->Status.Reseting = true;
}

void CEngine::SetVideoMode(UINT Mode)
{
	if (this->Device.VideoModesCount <= Mode) return;
	if (this->Device.CurrentMode == this->Device.VideoModes + Mode) return;
	this->Status.VideoMode = Mode;

	this->Device.CurrentMode = this->Device.VideoModes + Mode;

	HWND hWnd = this->PresentParameters.hDeviceWindow;
	BOOL bWnd = this->PresentParameters.Windowed;

	ZeroMemory(&this->PresentParameters, sizeof(D3DPRESENT_PARAMETERS));
	this->PresentParameters.BackBufferCount = BACKBUFFERS;
	this->PresentParameters.BackBufferFormat = this->Device.CurrentMode->Format;
	this->PresentParameters.BackBufferWidth = this->Device.CurrentMode->Width;
	this->PresentParameters.BackBufferHeight = this->Device.CurrentMode->Height;
	this->PresentParameters.Windowed = bWnd;
	this->PresentParameters.FullScreen_RefreshRateInHz = (this->PresentParameters.Windowed) ? 0 : this->Device.CurrentMode->RefreshRate;
	this->PresentParameters.hDeviceWindow = hWnd;
	this->PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	this->PresentParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
	this->PresentParameters.EnableAutoDepthStencil = TRUE;
	this->PresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//D3DPRESENT_INTERVAL_DEFAULT;// 

	RECT wr = { 0, 0, this->PresentParameters.BackBufferWidth, this->PresentParameters.BackBufferHeight };    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size
	SetWindowPos(this->PresentParameters.hDeviceWindow, NULL, 0, 0, wr.right - wr.left, wr.bottom - wr.top, SWP_NOMOVE | SWP_NOACTIVATE);

	this->Status.Reseting = true;
}

void CEngine::AddLoadQueueElement(CEngineElement * Element)
{
	Element->AsociateEngine(this);
	this->vLoadList.push_back(Element);
	this->Status.QueueSize++;
}

void CEngine::Release(CEngineElement * Element)
{
	if (Element == nullptr) return;
	EnterCriticalSection(&this->Status.CSEngine);
	for (std::list<CEngineElement*>::iterator it = this->vLoadeds.begin(); it != this->vLoadeds.end(); it++)
	{
		if (*it != Element) continue;
		this->Log(TEXT("Released '%s'"), (*it)->GetName());
		(*it)->Release();
		it = this->vLoadeds.erase(it);
		goto LeaveRelease;
	}
	for (std::list<CEngineElement*>::iterator it = this->vLoadList.begin(); it != this->vLoadList.end(); it++)
	{
		if (*it != Element) continue;
		this->Log(TEXT("Released '%s'"), (*it)->GetName());
		(*it)->Release();
		it=this->vLoadList.erase(it);
		return;
	}
LeaveRelease:
	LeaveCriticalSection(&this->Status.CSEngine);
}

void CEngine::SetLoadScreen(CEngineLoad * Screen)
{
	if (this->LoadScreen) this->LoadScreen->Release();
	this->LoadScreen = Screen;
	this->LoadScreen->AsociateEngine(this);
	if (this->Dev()) this->LoadScreen->Load();
}

void CEngine::Release()
{
	EnterCriticalSection(&this->Status.CSEngine);
	for (std::list<CEngineElement*>::iterator it = this->vLoadeds.begin(); it != this->vLoadeds.end(); it++)
	{
		this->Log(TEXT("Released Loaded '%s'"), (*it)->GetName());
		(*it)->Release();
	}
	for (std::list<_tagEffect>::iterator it = this->vEffects.begin(); it != this->vEffects.end(); it++)
	{
		SAFE_RELEASE(it->lpEffect);
		this->Log(TEXT("Released Fx:%s"), it->szEffect);
	}

	this->Status.Initialized = false;
	this->Log(TEXT("Released Engine"));
	SAFE_RELEASE(this->font_fps);
	SAFE_RELEASE(this->lpSprite);
	LeaveCriticalSection(&this->Status.CSEngine);
}

const _tagStatus * CEngine::GetStatus()
{
	return &this->Status;
}

void CEngine::GetDevInfo(int & w, int & h)
{
	w = this->Device.CurrentMode->Width;
	h = this->Device.CurrentMode->Height;
}

void CEngine::SetStatus(ENG_STATUS St, float value)
{
	switch (St)
	{
	case Z_Angle:this->Status.Z_Angle = value; break;
	case Z_Speed:this->Status.Z_Speed = value; break;
	case X_Angle:this->Status.X_Angle = value; break;
	case X_Speed:this->Status.X_Speed = value; break;
	case F_Speed:this->Status.F_Speed = value; break;
	case L_Speed:this->Status.L_Speed = value; break;
	}
}

void CEngine::SetFogLinear(float start, float end)
{
	this->Status.Fog = true;
	this->Dev()->SetRenderState(D3DRS_FOGENABLE, TRUE);
	this->Dev()->SetRenderState(D3DRS_FOGCOLOR, this->Status.Ambient);
	/*if(this->Device.DeviceCaps.RasterCaps&D3DPRASTERCAPS_FOGRANGE)
		this->Dev()->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	else*/
		this->Dev()->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	if (end <= 2.0f)//Porsentual Fog
	{
		start *= this->Status.plane_far - this->Status.plane_near;
		end *= this->Status.plane_far - this->Status.plane_near;
	}
	this->Dev()->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&start));
	this->Dev()->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&end));
}

void CEngine::SetFogDisable()
{
	this->Status.Fog = false;
	this->Dev()->SetRenderState(D3DRS_FOGENABLE, FALSE);
}

int CEngine::Loop(HACCEL hAccelTable)
{
	MSG msg;
	Elapsed Draw2;
	HRTimer Draw, SubDraw;
	float fElapsed, fNeed=1.0f/60.0f;
	int old=0;
	time_t ttime;
	TCHAR Frames[100];
	RECT r = { 0,0,100,40 };

	D3DXVECTOR3 CurrentCamera, Dir;
	D3DXVECTOR4 Out;

	UINT DrawSR = 4;
	
	while(!this->Status.KillSignal)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				this->Status.Focus = this->PresentParameters.hDeviceWindow == GetForegroundWindow();
				DispatchMessage(&msg);
			}
		}
		if (msg.message == WM_QUIT) break;

		if (this->Status.Reseting)
		{
			this->font_fps->OnLostDevice();
			this->lpSprite->OnLostDevice();
			this->GUI->OnLostDevice();
			this->TextManager->OnLostDevice();
			if (this->LoadScreen != nullptr) this->LoadScreen->OnLostDevice();
			for (std::list<_tagEffect>::iterator it = this->vEffects.begin(); it != this->vEffects.end(); it++)
			{
				it->lpEffect->OnLostDevice();
			}
			HRESULT hr = this->Device.Device->Reset(&this->PresentParameters);
			if (FAILED(hr))
			{
				this->Log(TEXT("Error 0x%08X (%s) on reseting device"), hr, DXGetErrorDescription(hr));				
				this->Kill(true);
				return hr;
			}

			for (std::list<_tagEffect>::iterator it = this->vEffects.begin(); it != this->vEffects.end(); it++)
			{
				it->lpEffect->OnResetDevice();
			}
			if (this->LoadScreen != nullptr) this->LoadScreen->OnResetDevice();
			this->GUI->OnResetDevice();
			this->TextManager->OnResetDevice();
			this->font_fps->OnResetDevice();
			this->lpSprite->OnResetDevice();
			this->UpdateMatrix();
			this->Status.Reseting = false;
			this->Status.Resizing = true;

			if (this->lReflections.size() > 0)
			{
				for (std::list<Plane>::iterator it = this->lReflections.begin(); it != this->lReflections.end(); it++)
				{
					SAFE_RELEASE(it->RenderTarget);
					//D3DXMatrixReflect(&it->ReflectMatrix, &it->_Plane);
					D3DXCreateTexture(
						this->Dev(),
						this->Device.CurrentMode->Width / this->Status.ReflectQuality,
						this->Device.CurrentMode->Height / this->Status.ReflectQuality,
						1,
						D3DUSAGE_RENDERTARGET,
						D3DFMT_A8R8G8B8,
						D3DPOOL_DEFAULT,
						&it->RenderTarget
					);
				}
			}
			
			if (this->OnReset) this->OnReset(this);
			continue;
		}

		if (this->Status.Resizing) this->ResetWindow();

		fElapsed = Draw.Consumed();
		if (this->Status.DrawLoading)
		{
			Sleep(10);
			continue;
		}
		/*if (!(DrawSR--))
		{
			LPDIRECT3DSURFACE9 lpSource, lpTargetSurface;
			if (this->lReflections.size() > 0)
			{
				D3DXMATRIX mRView, pCopy, vCopy;
				this->Dev()->GetRenderTarget(0, &lpSource);
				//this->Dev()->SetTransform(D3DTS_PROJECTION, &this->m_rProyection);
				pCopy = this->m_Project;
				vCopy = this->m_View;
				this->m_Project = this->m_rProyection;

				for (std::list<Plane>::iterator it = this->lReflections.begin(); it != this->lReflections.end(); it++)
				{
					it->RenderTarget->GetSurfaceLevel(0, &lpTargetSurface);

					CurrentCamera = this->Status.Eye_Position;
					Dir = /*this->Status.Eye_ViewAt/it->Origin - CurrentCamera;

					Dir = Dir - 2.0*dot(Dir, it->Normal)*it->Normal;
					CurrentCamera = it->Origin - Dir;

					Dir = this->Status.Eye_ViewAt - this->Status.Eye_Position;
					Dir = Dir - 2.0*dot(Dir, it->Normal)*it->Normal;
					Dir = CurrentCamera + Dir;

					/*if(this->Status.Right_hand)
						D3DXMatrixLookAtRH(&it->View, &CurrentCamera, &Dir, &this->Status.Eye_Up);
					else/
						D3DXMatrixLookAt(&it->View, &CurrentCamera, &Dir, &this->Status.Eye_Up);


					this->Dev()->SetTransform(D3DTS_VIEW, &(this->m_View = it->View));

					this->Dev()->SetRenderTarget(0, lpTargetSurface);
					this->Dev()->Clear(1, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, this->Status.Ambient, 1, 0);
					this->Dev()->BeginScene();
					this->FrameReflections(this);
					this->Dev()->EndScene();
					this->Dev()->SetRenderTarget(0, lpSource);

					SAFE_RELEASE(lpTargetSurface);

					//D3DXSaveTextureToFile(TEXT("reflect.bmp"), D3DXIMAGE_FILEFORMAT::D3DXIFF_BMP, it->RenderTarget, nullptr);
				}

				this->m_Project = pCopy;
				this->m_View = vCopy;
				//this->Dev()->SetRenderTarget(0, lpSource);
				SAFE_RELEASE(lpSource);
			}
			//this->FrameShadows(this, SubDraw.Consumed());
			SubDraw.Reset();
			DrawSR = 1;
		}*/
		if (fElapsed < fNeed)
		{
			//if(fElapsed < fNeed) Sleep((fNeed - fElapsed) * 1000.0);
			continue;
		}
		Draw.Reset();
		EnterCriticalSection(&this->Status.CSEngine);

		// Time status save
		this->Status.Elapsed = fElapsed;
		this->Status.FramesPerSecond = 1.0f / fElapsed;
		this->Status.Time += fElapsed;

		if(old!=(int)this->Status.Time)
		{
			time(&ttime);
			localtime_s(&this->Status.now, &ttime);
			old = (int)this->Status.Time;
		}

		//render work
		this->GUI->OffScreenDraw();
		this->Dev()->Clear(1, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, this->Status.Ambient, 1, 0);
		this->Dev()->BeginScene();
		this->SetProjectionMatrix();
		float frame;
		Draw2.Reset();
		if(this->OnFrame) this->OnFrame(this);
		else this->DefaultOnFrame(fElapsed);
		this->GUI->Draw();
		frame = Draw2.Consumed();
		/*if((int)(this->Status.Time*1000.0f)%500==0)*/ _stprintf_s(Frames, TEXT("%.1f %f"), this->Status.FramesPerSecond, frame);
		this->GUI->Draw();
		this->Text(Frames, &r, 0xffffffff);
		this->Dev()->EndScene();
		this->Dev()->Present(nullptr, nullptr, NULL, nullptr);
		this->UpdateMatrix();
		LeaveCriticalSection(&this->Status.CSEngine);
	}
	return (int)msg.wParam;
}

void CEngine::SetCallBack(OnEvent cb)
{
	this->OnFrame = cb;
}

D3DXCOLOR& CEngine::GetAmbientLight(LFMT fmt)
{
	switch (fmt)
	{
	case LFMT_RGB: return this->Status.Ambient;
	case LFMT_YBR: return this->Status.AmbientYBR;
	default:
		return this->Status.Ambient;
	}
	return this->Status.Ambient;
}

void CEngine::SetAmbientLight(D3DXCOLOR c)
{
	this->Status.Ambient = c;
	this->Status.AmbientYBR = D3DXCOLOR(
		0.299*this->Status.Ambient.r + 0.587*this->Status.Ambient.g + 0.114*this->Status.Ambient.b,
		0.5 - 0.168736*this->Status.Ambient.r - 0.331264*this->Status.Ambient.g + 0.5*this->Status.Ambient.b,
		0.5 + 0.5*this->Status.Ambient.r - 0.418688*this->Status.Ambient.g - 0.081312*this->Status.Ambient.b,
		this->Status.Ambient.a
	);
	if(this->Dev())this->Dev()->SetRenderState(D3DRS_FOGCOLOR, c);
}

Plane * CEngine::AddReflectionPlane(D3DXVECTOR3 Origin, D3DXVECTOR3 Normal)
{
	if (this->lReflections.size() > 0)
	{
		for (std::list<Plane>::iterator it = this->lReflections.begin(); it != this->lReflections.end(); it++)
		{
			if (it->Normal == Normal && it->Origin == Origin) return &(*it);
		}
	}
	Plane p;
	HRESULT hr;
	D3DXPlaneFromPointNormal(&p._Plane, &D3DXVECTOR3(0,0,0), &Normal);
	p.Origin = Origin;
	p.Normal = Normal;
	D3DXMatrixReflect(&p.ReflectMatrix, &p._Plane);
	hr = D3DXCreateTexture(
		this->Dev(),
		this->Device.CurrentMode->Width / this->Status.ReflectQuality,
		this->Device.CurrentMode->Height / this->Status.ReflectQuality,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&p.RenderTarget
	);

	this->lReflections.push_back(p);

	return &this->lReflections.back();
}

void CEngine::FlushReflections()
{
	for (std::list<Plane>::iterator it = this->lReflections.begin(); it != this->lReflections.end(); it++)
	{
		SAFE_RELEASE(it->RenderTarget);
	}
	this->lReflections.clear();
}

D3DXMATRIX& CEngine::GetWorld()
{
	return this->m_World;
}

D3DXMATRIX& CEngine::GetWVP()
{
	//return this->m_World*((this->Status.Draw3D)?this->m_View*this->m_Project:this->m_ViewOrtho*this->m_Ortho);
	return this->m_pcWVP;
}

D3DXMATRIX& CEngine::GetWRP(Plane*p)
{
	return this->m_World*(p->View)*this->m_rProyection;
}

void CEngine::PushWorld(D3DXMATRIX *World)
{
	this->vWorld.push_back(this->m_World);
	if (World == nullptr)
	{
		D3DXMatrixIdentity(&this->m_World);
		this->Dev()->SetTransform(D3DTS_WORLD, &this->m_World);
		this->m_pcWVP = this->m_World*((this->Status.Draw3D) ? this->m_View*this->m_Project : this->m_ViewOrtho*this->m_Ortho);
		return;
	}
	this->m_World = *World;

	this->Dev()->SetTransform(D3DTS_WORLD, &this->m_World);
	this->m_pcWVP = this->m_World*((this->Status.Draw3D) ? this->m_View*this->m_Project : this->m_ViewOrtho*this->m_Ortho);
}

void CEngine::PushCamera(D3DXVECTOR3 Pos, D3DXVECTOR3 Look, D3DXVECTOR3 Up)
{
	_tagCamera c = { this->Status.Eye_Position,this->Status.Eye_ViewAt,this->Status.Eye_Up };
	this->vCamera.push_back(c);
	this->Status.Eye_Position = Pos;
	this->Status.Eye_ViewAt = Look+Pos;
	this->Status.Eye_Up = Up;
	this->UpdateMatrix();
}

void CEngine::PopWorld()
{
	this->m_World = this->vWorld.back();
	this->Dev()->SetTransform(D3DTS_WORLD, &this->m_World);
	this->m_pcWVP = this->m_World*((this->Status.Draw3D) ? this->m_View*this->m_Project : this->m_ViewOrtho*this->m_Ortho);
	this->vWorld.pop_back();
}

void CEngine::PopCamera()
{
	_tagCamera c = this->vCamera.back();
	this->vCamera.pop_back();
	this->Status.Eye_Position = c.Pos;
	this->Status.Eye_ViewAt = c.Look;
	this->Status.Eye_Up = c.Up;
	this->UpdateMatrix();
}

void CEngine::SetWorld(D3DXMATRIX * m)
{
	this->m_World = *m;
	this->Dev()->SetTransform(D3DTS_WORLD, m);
	this->m_pcWVP = this->m_World*((this->Status.Draw3D) ? this->m_View*this->m_Project : this->m_ViewOrtho*this->m_Ortho);
}

void CEngine::SetOrthoMatrix(bool update)
{
	if (update)
	{
		this->PushCamera(D3DXVECTOR3(0, 0, -10), D3DXVECTOR3(0, 0, 10), D3DXVECTOR3(0, 1, 0));
		D3DXMATRIX cpy = this->m_ViewOrtho;
		this->PopCamera();
		this->m_ViewOrtho = cpy;
	}
	this->Dev()->SetTransform(D3DTS_WORLD, &this->m_Identity);
	this->Dev()->SetTransform(D3DTS_VIEW, &this->m_ViewOrtho);
	this->Dev()->SetTransform(D3DTS_PROJECTION, &this->m_Ortho);
	this->Dev()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	this->Dev()->SetRenderState(D3DRS_ZENABLE, FALSE);
	this->Status.Draw3D = false;
}

void CEngine::SetProjectionMatrix(bool update)
{
	if(update) this->UpdateMatrix();
	this->Dev()->SetTransform(D3DTS_WORLD, &this->m_World);
	this->Dev()->SetTransform(D3DTS_VIEW, &this->m_View);
	this->Dev()->SetTransform(D3DTS_PROJECTION, &this->m_Project);
	this->Dev()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	this->Dev()->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	this->Dev()->SetRenderState(D3DRS_ZENABLE, TRUE);
	this->Dev()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	this->Status.Draw3D = true;
}

void CEngine::SetRenderTarget(LPDIRECT3DTEXTURE9 Texture)
{
	D3DSURFACE_DESC desc;
	LPDIRECT3DSURFACE9 Target;
	this->Dev()->GetRenderTarget(0, &this->BackBuffer);
	Texture->GetLevelDesc(0, &desc);
	Texture->GetSurfaceLevel(0, &Target);
	this->Dev()->SetRenderTarget(0, Target);
	/*if (this->Status.Right_hand)
	{
		D3DXMatrixPerspectiveFovRH(&this->m_Project,
			D3DX_PI / 4.0f / this->Status.Zoom,
			(float)desc.Width / (float)desc.Height,
			this->Status.plane_near, this->Status.plane_far
			);
	}
	else
	{
			D3DXMatrixPerspectiveFovLH(&this->m_Project,
				D3DX_PI / 4.0f / this->Status.Zoom,
				(float)desc.Width / (float)desc.Height,
				this->Status.plane_near, this->Status.plane_far
				);
	}*/
	D3DXMatrixPerspectiveFov(&this->m_Project,
		D3DX_PI / 4.0f / this->Status.Zoom,
		(float)desc.Width / (float)desc.Height,
		this->Status.plane_near, this->Status.plane_far
	);
	D3DXMatrixOrthoOffCenterLH(&this->m_Ortho, 0, desc.Width, desc.Height, 0, this->Status.plane_near, this->Status.plane_far);
	D3DXMatrixLookAtLH(&this->m_ViewOrtho, &this->Status.Eye_Position, &this->Status.Eye_ViewAt, &this->Status.Eye_Up);
}

void CEngine::SetBackBufferTarget()
{
	if (this->BackBuffer == nullptr) return;
	this->Dev()->SetRenderTarget(0, this->BackBuffer);
	this->BackBuffer = nullptr;
}

//void CEngine::UseRightHand(bool enable)
//{
//	this->Status.Right_hand = enable;
//}

void CEngine::SetFarNearPlanes(float far_plane, float near_plane)
{
	this->Status.plane_far = far_plane;
	this->Status.plane_near = near_plane;
	this->UpdateMatrix();
}

void CEngine::Text(const TCHAR * szText, RECT * rect, DWORD Color, DWORD Flags)
{
#ifdef UNICODE
	this->font_fps->DrawTextW(
#else
	this->font_fps->DrawTextA(
#endif
		nullptr, szText, -1, rect, Flags, Color);
}

HRESULT CEngine::LoadEffect(const TCHAR * EffectKey, const TCHAR * EffectFile, LPD3DXEFFECT * ppEffect)
{
	if (_tcslen(EffectKey)>=4)
	{
		DWORD dwKey = *(DWORD*)EffectKey;
		for (std::list<_tagEffect>::iterator it = this->vEffects.begin(); it != this->vEffects.end(); it++)
		{
			if (dwKey != *(DWORD*)it->szEffect || _tcscmp(EffectKey, it->szEffect)) continue;
			*ppEffect = it->lpEffect;
			return S_OK;
		}
	}

	_tagEffect fx;
	ZeroMemory(fx.szEffect, sizeof(fx.szEffect));
	memcpy(fx.szEffect, EffectKey, min(sizeof(fx.szEffect), _tcslen(EffectKey)*sizeof(TCHAR)));

	LPD3DXBUFFER pError;
	HRESULT hr = D3DXCreateEffectFromFile(this->Dev(), EffectFile, nullptr, nullptr, 0, nullptr, &fx.lpEffect, &pError);
	if (FAILED(hr))
	{
		if(pError) this->LOG(TEXT("Shader Error:%s"), pError->GetBufferPointer());
		//else this->LOG(TEXT("Shader Error:%d"), hr);
		*ppEffect = nullptr;
		return hr;
	}

	this->Log(TEXT("Fx:'%s' Loaded from '%s'"), EffectKey, EffectFile);

	this->vEffects.push_back(fx);
	*ppEffect = fx.lpEffect;

	return S_OK;
}

std::list<_tagResolution> CEngine::GetResolutionList()
{
	std::list<_tagResolution> vList;
	_tagResolution res;
	for (UINT i = 0; i < this->Device.VideoModesCount; i++)
	{
		res.Format = i;
		_stprintf_s(res.szFormat, TEXT("%dx%d"), this->Device.VideoModes[i].Width, this->Device.VideoModes[i].Height);
		vList.push_back(res);
	}
	return vList;
}

void CEngine::Log(const TCHAR * szError, ...)
{
	TCHAR szLogLine[3072], szLogFormat[2048];
	va_list ap;
	_stprintf_s(
		szLogFormat, 
		TEXT("%d:%d:%d > %s\r\n"), 
		this->Status.now.tm_hour, 
		this->Status.now.tm_min,
		this->Status.now.tm_sec,
		szError
		);

	va_start(ap, szError);
	_vstprintf_s(szLogLine, szLogFormat, ap);
	va_end(ap);

	OutputDebugString(szLogLine);
	_putts(szLogLine);

	if (this->hLog != INVALID_HANDLE_VALUE)
	{
		DWORD dwLong = _tcslen(szLogLine)*sizeof(TCHAR);
		WriteFile(
			this->hLog,
			szLogLine,
			dwLong,
			&dwLong,
			nullptr
			);
#ifdef _DEBUG
		WriteConsoleW(
			this->hConsole,
			szLogLine,
			dwLong/ sizeof(TCHAR),
			&dwLong,
			nullptr
			);
#endif
	}
}

void CEngine::ResetWindow()
{
	HRESULT hr = this->Device.Device->TestCooperativeLevel();
	if (FAILED(hr)) return;
	this->Status.Resizing = false;
	static bool OldMode = false;

	if (!this->PresentParameters.Windowed)
	{
		if (OldMode) return;
		OldMode = true;
		SetWindowLong(this->PresentParameters.hDeviceWindow, GWL_STYLE, WS_POPUPWINDOW);
		this->Log(TEXT("Full screen mode ON"));
		SetWindowPos(
			this->PresentParameters.hDeviceWindow,
			NULL,
			0,
			0,
			this->PresentParameters.BackBufferWidth,
			this->PresentParameters.BackBufferHeight,
			SWP_NOACTIVATE | SWP_FRAMECHANGED
			);
	}
	else
	{
		OldMode = false;
		SetWindowLong(this->PresentParameters.hDeviceWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		RECT wr = { 0, 0, this->PresentParameters.BackBufferWidth, this->PresentParameters.BackBufferHeight };    // set the size, but not the position
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size
		SetWindowPos(
			this->PresentParameters.hDeviceWindow,
			NULL,
			0,
			0,
			wr.right - wr.left,
			wr.bottom - wr.top,
			SWP_NOMOVE | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW
			);
		this->Log(TEXT("Full screen mode OFF"));
	}
}

void CEngine::DefaultOnFrame(float elapsed)
{
	std::list<CEngineElement*>::iterator it;
	for (it = this->vLoadeds.begin(); it != this->vLoadeds.end(); it++)
	{
		(*it)->Draw();
	}
}

void CEngine::GetDeviceFormat(D3DDEVTYPE*DevType, D3DFORMAT*Format)
{
	D3DFORMAT Formats[] = { D3DFMT_X8R8G8B8,D3DFMT_X1R5G5B5,D3DFMT_R5G6B5 };
	D3DDEVTYPE DeviceTypes[] = { D3DDEVTYPE_HAL, D3DDEVTYPE_REF };

	*DevType = DeviceTypes[0];
	*Format = Formats[0];

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (
				FAILED(
					this->d3d->CheckDeviceType(
						D3DADAPTER_DEFAULT,
						DeviceTypes[i],
						Formats[j],
						Formats[j],
						TRUE
						)
					)
				) continue;
			*DevType = DeviceTypes[i];
			*Format = Formats[j];
			return;
		}
	}

	this->LOG("No format compatible finded for device");
}

void CEngine::CloseThreads()
{
	if (this->hLoadQueue != INVALID_HANDLE_VALUE) {
		TerminateThread(this->hLoadQueue, 0x00);
	}
	if (this->hWorker = INVALID_HANDLE_VALUE) {
		TerminateThread(this->hWorker, 0x00);
	}
	this->hWorker = INVALID_HANDLE_VALUE;
	this->hLoadQueue = INVALID_HANDLE_VALUE;
}

void CEngine::UpdateMatrix()
{
	D3DXMATRIX m, mT;
	POINT Cursor = { 0 };
	int cx = this->PresentParameters.BackBufferWidth / 2, cy = this->PresentParameters.BackBufferHeight / 2;

	if (this->Status.Focus && this->Status.BindCamera &&!this->Status.DrawLoading)
	{
		GetCursorPos(&Cursor);
		ScreenToClient(this->PresentParameters.hDeviceWindow, &Cursor);

		this->Status.Z_Angle += (cx - Cursor.x)*this->Status.Z_Speed*this->Status.Elapsed;
		this->Status.X_Angle += (cy - Cursor.y)*this->Status.X_Speed*this->Status.Elapsed;
		Cursor.x = cx;
		Cursor.y = cy;
		ClientToScreen(this->PresentParameters.hDeviceWindow, &Cursor);
		if (this->Status.Z_Speed != 0 || this->Status.X_Speed != 0)
			SetCursorPos(Cursor.x, Cursor.y);

		D3DXMatrixRotationX(&mT, this->Status.X_Angle);
		m = mT;
		D3DXMatrixRotationZ(&mT, this->Status.Z_Angle);
		m *= mT;
		D3DXVECTOR4 Dir;
		D3DXVec3Transform(&Dir, &D3DXVECTOR3(0, 1, 0), &m);
		D3DXVECTOR3 Dir3(Dir.x, Dir.y, Dir.z);
		D3DXVECTOR3 Left;
		D3DXVec3Cross(&Left, &Dir3, &this->Status.Eye_Up);
		D3DXVECTOR3 Mov = (Dir3*this->Status.F_Speed + Left*this->Status.L_Speed)*this->Status.Elapsed;
		this->Status.Eye_Position += Mov;
		this->Status.Eye_ViewAt = Dir3 + this->Status.Eye_Position;
	}

	/*if (this->Status.Right_hand)
	{
		D3DXMatrixPerspectiveFovRH(
			&this->m_Project, 
			D3DX_PI / 4.0f / this->Status.Zoom, 
			(float)this->Device.CurrentMode->Width / (float)this->Device.CurrentMode->Height,
			this->Status.plane_near, this->Status.plane_far
			);
		D3DXMatrixPerspectiveFovRH(
			&this->m_rProyection,
			D3DX_PI / 4.0f / this->Status.Zoom,
			(float)this->Device.CurrentMode->Width / (float)this->Device.CurrentMode->Height,
			this->Status.plane_near, this->Status.plane_far
		);
		D3DXMatrixLookAtRH(&this->m_View, &this->Status.Eye_Position, &this->Status.Eye_ViewAt, &this->Status.Eye_Up);
	}
	else
	{
		D3DXMatrixPerspectiveFovLH(&this->m_Project, 
			D3DX_PI / 4.0f / this->Status.Zoom, 
			(float)this->Device.CurrentMode->Width / (float)this->Device.CurrentMode->Height,
			this->Status.plane_near, 
			this->Status.plane_far
			);
		D3DXMatrixPerspectiveFovLH(
			&this->m_rProyection,
			D3DX_PI / 4.0f / this->Status.Zoom,
			(float)this->Device.CurrentMode->Width / (float)this->Device.CurrentMode->Height,
			this->Status.plane_near, this->Status.plane_far
		);
		D3DXMatrixLookAtLH(&this->m_View, &this->Status.Eye_Position, &this->Status.Eye_ViewAt, &this->Status.Eye_Up);
	}*/

	D3DXMatrixPerspectiveFov(&this->m_Project,
		D3DX_PI / 4.0f / this->Status.Zoom,
		(float)this->Device.CurrentMode->Width / (float)this->Device.CurrentMode->Height,
		this->Status.plane_near,
		this->Status.plane_far
	);
	D3DXMatrixPerspectiveFov(
		&this->m_rProyection,
		D3DX_PI / 4.0f / this->Status.Zoom,
		(float)this->Device.CurrentMode->Width / (float)this->Device.CurrentMode->Height,
		this->Status.plane_near, this->Status.plane_far
	);
	D3DXMatrixLookAt(&this->m_View, &this->Status.Eye_Position, &this->Status.Eye_ViewAt, &this->Status.Eye_Up);

	D3DXMatrixOrthoOffCenterLH(
		&this->m_Ortho, 
		0, 
		this->Device.CurrentMode->Width, 
		this->Device.CurrentMode->Height, 
		0, 
		-10.0f,
		10.0f
		);
	D3DXMatrixLookAtLH(&this->m_ViewOrtho, &this->Status.Eye_Position, &this->Status.Eye_ViewAt, &this->Status.Eye_Up);
	/*D3DXMatrixLookAtLH(&this->m_ViewOrtho,
		&D3DXVECTOR3(0, 0, -100.0f),
		&D3DXVECTOR3(0, 0, 0.0f),
		&D3DXVECTOR3(0, 1, 0)
		);*/
	
	this->m_pcWVP = this->m_World*((this->Status.Draw3D) ? this->m_View*this->m_Project : this->m_ViewOrtho*this->m_Ortho);
}

CEngineElement::CEngineElement()
{
	this->Engine = nullptr;
	ZeroMemory(this->szName, sizeof(this->szName));
}

CEngineElement::~CEngineElement()
{
}

void CEngineElement::SetName(const TCHAR * szObjectName)
{
	if (szObjectName == nullptr)
	{
		Engine->Log(TEXT("Error: ObjectName is null"));
		return;
	}

	_tcscpy_s(this->szName, szObjectName);
}

void CEngineElement::CanSkip(bool enable)
{
	this->LoadSkip = enable;
}

bool CEngineElement::CanSkip()
{
	return this->LoadSkip;
}

const TCHAR * CEngineElement::GetName()
{
	return this->szName;
}

void CEngineElement::AsociateEngine(CEngine * eng)
{
	this->Engine = eng;
}

CEngine * CEngineElement::GetEngine()
{
	return this->Engine;
}

PDIRECT3DDEVICE9 CEngineElement::GetDevice()
{
	return this->Engine->Device.Device;
}
