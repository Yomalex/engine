// DAEngine.cpp: define las funciones exportadas de la aplicación DLL.
//

#include "stdafx.h"
#include "DAEngine.h"
#include "HRTimer.h"


DAEngine::DAEngine() : m_iMaxFrameRate(30)
{
	this->iD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	
	if (this->iD3D9 == nullptr)
	{
		this->SetError(0);
		return;
	}

	if (SUCCEEDED(this->iD3D9->CheckDeviceType(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		D3DFMT_X8R8G8B8,
		D3DFMT_X8R8G8B8,
		TRUE
		)))
	{
		this->m_D3DDType = D3DDEVTYPE_HAL;
	}
	else
	{
		this->m_D3DDType = D3DDEVTYPE_SW;
	}

	this->iD3D9->GetDeviceCaps(
		D3DADAPTER_DEFAULT, 
		this->m_D3DDType, 
		&this->m_D3DCaps
		);

	UINT modes = this->iD3D9->GetAdapterModeCount(
		D3DADAPTER_DEFAULT,
		D3DFMT_X8R8G8B8
		);

	D3DDISPLAYMODE mode;
	for (UINT i = 0; i < modes; i++)
	{
		this->iD3D9->EnumAdapterModes(
			D3DADAPTER_DEFAULT,
			D3DFMT_X8R8G8B8,
			i,
			&mode
			);
		if (mode.Width<640 || mode.Height<480) continue;
		WCHAR modeName[100];
		swprintf_s(modeName, L"%dx%d", mode.Width, mode.Height);
		this->m_mVideoModes[modeName] = mode;
	}

	this->m_Resolution = this->m_mVideoModes.begin()->first;

	// Presentation Parameters
	ZeroMemory(&this->m_D3DPP, sizeof(this->m_D3DPP));

	// BackBuffer configuration
	this->m_D3DPP.BackBufferCount = 1;
	this->m_D3DPP.BackBufferFormat = D3DFMT_X8R8G8B8;
	this->m_D3DPP.BackBufferWidth = this->m_mVideoModes.begin()->second.Width;
	this->m_D3DPP.BackBufferHeight = this->m_mVideoModes.begin()->second.Height;

	// Auto depth stencil
	this->m_D3DPP.EnableAutoDepthStencil = TRUE;
	this->m_D3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;

	// Window Mode
	this->m_D3DPP.hDeviceWindow = NULL;
	this->m_D3DPP.Windowed = FALSE;
	this->m_D3DPP.FullScreen_RefreshRateInHz = 
		this->m_mVideoModes.begin()->second.RefreshRate;

	// Presentation
	this->m_D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	this->m_D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	this->_2DCamera = new Camera();
	this->_2DCamera->MoveTo(
		this->m_D3DPP.BackBufferWidth/2, 
		this->m_D3DPP.BackBufferHeight/2,
		0
	);
	this->_2DCamera->LookDir(0, 0, 10);
}


DAEngine::~DAEngine()
{
}

void DAEngine::SetLeftHand()
{
	D3DXMatrixLookAt = D3DXMatrixLookAtLH;
	D3DXMatrixPerspective = D3DXMatrixPerspectiveLH;
	D3DXMatrixPerspectiveFov = D3DXMatrixPerspectiveFovLH;
	D3DXMatrixPerspectiveOffCenter = D3DXMatrixPerspectiveOffCenterLH;
	D3DXMatrixOrtho = D3DXMatrixOrthoLH;
	D3DXMatrixOrthoOffCenter = D3DXMatrixOrthoOffCenterLH;
}

void DAEngine::SetRightHand()
{
	D3DXMatrixLookAt = D3DXMatrixLookAtRH;
	D3DXMatrixPerspective = D3DXMatrixPerspectiveRH;
	D3DXMatrixPerspectiveFov = D3DXMatrixPerspectiveFovRH;
	D3DXMatrixPerspectiveOffCenter = D3DXMatrixPerspectiveOffCenterRH;
	D3DXMatrixOrtho = D3DXMatrixOrthoRH;
	D3DXMatrixOrthoOffCenter = D3DXMatrixOrthoOffCenterRH;
}

bool DAEngine::CreateDevice()
{
	return this->SetError(
		this->iD3D9->CreateDevice(
			D3DADAPTER_DEFAULT,
			this->m_D3DDType,
			this->m_D3DPP.hDeviceWindow,
			(this->m_D3DCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) ?
			D3DCREATE_HARDWARE_VERTEXPROCESSING :
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&this->m_D3DPP,
			&this->iD3DD9
		)
	) == S_OK;
}

void DAEngine::WindowMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_CREATE:
		// Asociar la ventana creada a la unidad
		// actual del a interfaz D3D
		this->m_D3DPP.hDeviceWindow = hWnd;
		break;
	default:
		// Enviar el mensaje a toda la cola de la GUI
		for (auto it : this->m_RQGUI)
		{
			it->WindowMessage(Message, wParam, lParam);
		}

		if(this->_3DCamera)
			this->_3DCamera->WindowMessage(Message, wParam, lParam);
		
		this->_2DCamera->WindowMessage(Message, wParam, lParam);
	}
}

int DAEngine::Loop()
{
	MSG msg;
	int BBUsed = 0;
	bool Drawing = true;
	double fminFrameTime = 1.0f/(double)this->m_iMaxFrameRate;
	HRTimer timer;

	do
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			//this->Status.Focus = this->PresentParameters.hDeviceWindow == GetForegroundWindow();
		}

		// Esperamos que coincida el Frame rate
		if (
			timer.Consumed() >= fminFrameTime
			)
		{
			// Guardar el tiempo de dibujado
			this->m_fTime += timer.Consumed();

			// Reinicio el temporizador
			timer.Reset();

			if (this->_3DCamera != nullptr)
			{
				// Inicia la seccion 3D
				this->_3DCamera->Render();

				// Se establece el modo de proyeccion
				this->Projection(&this->mat_ProjPersp);

				// Se dibujan los elementos solidos
				for (auto& item : this->m_RQSolid)
				{
					item->Render();
				}

				// Se dibujan los elementos con transparencias
				for (auto& item : this->m_RQTranslucid)
				{
					item->Render();
				}
			}			

			// Inicia la seccion 2D
			this->_2DCamera->Render();

			// Se establece el modo de proyeccion
			this->Projection(&this->mat_ProjOrtho);

			// Se dibuja la interfaz
			for (auto& item : this->m_RQGUI)
			{
				item->Render();
			}
		}
		else // En caso contrario realizamos otras operaciones
		{
			// Cargamos elementos en espera
			if (this->m_Load.size()>0)
			{
				if (this->SetError(this->m_Load.front()->Load()) == S_OK)
				{
					this->m_Loaded.push_back(this->m_Load.front());
					this->m_Load.pop_front();
				}
			}
			else
			{
				// dibujamos elementos fuera de pantalla
				for (auto& item : this->m_RQOffScreen)
				{
					item->Render();
				}
			}
		}


	} while (msg.message != WM_QUIT);
	return 0;
}

HRESULT DAEngine::SetError(HRESULT hrError)
{
	TCHAR Message[260];
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		this->m_LastError = hrError,
		0,
		Message,
		260,
		nullptr
		);

	this->m_sLastError = Message;

	return hrError;
}

EngineObject * DAEngine::Load(EngineObject *obj)
{
	if (obj == nullptr)
		return obj;

	obj->SetEngine(this);
	this->m_Load.push_back(obj);

	return obj;
}

HRESULT DAEngine::World(D3DXMATRIX * mat)
{
	this->Transforms.push_back(*mat);
	mat = &this->Transforms.back();
	return this->SetError(this->iD3DD9->SetTransform(D3DTS_WORLD, this->lpmat_World = mat));
}

HRESULT DAEngine::World()
{
	this->Transforms.pop_back();
	D3DXMATRIX * mat = &this->Transforms.back();
	return this->SetError(this->iD3DD9->SetTransform(D3DTS_WORLD, this->lpmat_World = mat));
}

D3DXMATRIX* DAEngine::CurWorld()
{
	return &this->Transforms.back();
}

HRESULT DAEngine::View(D3DXMATRIX * mat)
{
	return this->SetError(this->iD3DD9->SetTransform(D3DTS_VIEW, this->lpmat_View = mat));
}

HRESULT DAEngine::Projection(D3DXMATRIX * mat)
{
	return this->SetError(this->iD3DD9->SetTransform(D3DTS_PROJECTION, this->lpmat_Proj = mat));
}

void DAEngine::PerspectiveFov(float Angle, float aspect, float fnear, float ffar)
{
	D3DXMatrixPerspectiveFov(&this->mat_ProjPersp, Angle, aspect, fnear, ffar);
}

void DAEngine::OrthogonalOffCenter(float l, float r, float t, float b, float fnear, float ffar)
{
	D3DXMatrixOrthoOffCenter(&this->mat_ProjOrtho, l, r, b, t, fnear, ffar);
}

D3DXMATRIX* (WINAPI *D3DXMatrixLookAt)
(D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pEye, CONST D3DXVECTOR3 *pAt,
	CONST D3DXVECTOR3 *pUp);

D3DXMATRIX* (WINAPI *D3DXMatrixPerspective)
(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf);

D3DXMATRIX* (WINAPI *D3DXMatrixPerspectiveFov)
(D3DXMATRIX *pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf);

D3DXMATRIX* (WINAPI *D3DXMatrixPerspectiveOffCenter)
(D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
	FLOAT zf);

D3DXMATRIX* (WINAPI *D3DXMatrixOrtho)
(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf);

D3DXMATRIX* (WINAPI *D3DXMatrixOrthoOffCenter)
(D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
	FLOAT zf);