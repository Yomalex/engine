// Mu.cpp: define el punto de entrada de la aplicación.
//

#include "stdafx.h"
#include "Mu.h"

CEngine *eng;
CTerrain * Terrains;
CLoadSplash * Screen;
CUI * Interface;
CTexMng *TextureManager;
CModel * Faces[7];
CWZGate Gate;
CWZText Text;
CWZMoveReq MoveReq;
CWZItem Item;

#define MAX_LOADSTRING 100

// Variables globales:
HINSTANCE hInst;                                // Instancia actual
WCHAR szTitle[MAX_LOADSTRING];                  // Texto de la barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // nombre de clase de la ventana principal

// Declaraciones de funciones adelantadas incluidas en este módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

BYTE Path[][2] =
{
	{247,51},
	{247,24},
	{191,24},
	{191,51},
};

float Zoom = 9;

#define Rad 10.0
void GetCamDir(const BYTE *b0, const BYTE *b1, const float *a, float& cdx, float& cdy, float dx, float dy)
{
	float sy = dx < 0 ? 1 : -1;
	float sx = dy >= 0 ? 1 : -1;
	if (b0[0] == b1[0])
	{
		float x = (b0[0] - a[0]);
		float y = sqrt(Rad*Rad - x*x)*sy;
		float m = sqrt(x*x + y*y);
		cdx = x / m;
		cdy = y / m;
	}
	else if (b0[1] == b1[1])
	{
		float y = b0[1] - a[1];
		float x = sqrt(Rad*Rad - y*y)*sx;
		float m = sqrt(x*x + y*y);
		cdx = x / m;
		cdy = y / m;
	}
}
void ServerAnimation(CEngine * p, float fElapsed)
{
	static float cx = Path[0][0], cy = Path[0][1], speed = 5, dx, dy, cdx, cdy;
	static BYTE dest = 0;
	if (
		((BYTE)cx >= Path[dest][0] && dx>0) ||
		((BYTE)cy >= Path[dest][1] && dy>0) ||
		((BYTE)cx < Path[dest][0] && dx<0) ||
		((BYTE)cy < Path[dest][1] && dy<0)
		)
	{
		cx = Path[dest][0];
		cy = Path[dest][1];
		dest++;
		dest %= 4;
		dx = Path[dest][0] - cx;
		dy = Path[dest][1] - cy;
		float m = sqrt(dx*dx + dy*dy);
		dx /= m;
		dy /= m;
	}
	else if (dx == 0 && dy == 0)
	{
		cx = Path[dest][0];
		cy = Path[dest][1];
		dest++;
		dest %= 4;
		dx = Path[dest][0] - cx;
		dy = Path[dest][1] - cy;
		float m = sqrt(dx*dx + dy*dy);
		dx /= m;
		dy /= m;
	}
	cx += dx*speed*fElapsed;
	cy += dy*speed*fElapsed;
	if ((cx + dx*Rad > Path[dest][0] && dx>0) || (cy + dy*Rad > Path[dest][1] && dy>0) ||
		(cx + dx*Rad < Path[dest][0] && dx<0) || (cy + dy*Rad < Path[dest][1] && dy<0))
	{
		BYTE pDest = (dest + 1) % 4;
		float A[] = { cx,cy };
		GetCamDir(Path[dest], Path[pDest], A, cdx, cdy, dx, dy);
	}
	else
	{
		cdx = dx;
		cdy = dy;
	}
	p->SetCameraPos(cx*CELL_SIZE, cy*CELL_SIZE, 400);
	p->SetCameraDir(cdx, cdy, 0.2);

	Terrains->Center.x = cx*CELL_SIZE + cdx*RADIUS;
	Terrains->Center.y = cy*CELL_SIZE + cdy*RADIUS;
}

E_EVENT(OnDeviceCreate)
{
}

E_EVENT(OnDeviceReset)
{
	ResetMuUI();
	int wm, hm;
	p->GetDevInfo(wm, hm);
	WritePrivateProfileInt(TEXT("Game"), TEXT("Width"), wm, TEXT("./config.mu.ini"));
	WritePrivateProfileInt(TEXT("Game"), TEXT("Height"), hm, TEXT("./config.mu.ini"));
	WritePrivateProfileInt(TEXT("Game"), TEXT("Full_Screen"), (p->isFullScreen())?1:0, TEXT("./config.mu.ini"));
}

E_EVENT(OnFrame_NOTLOGGED)
{
	ServerAnimation(p, p->GetStatus()->Elapsed);
	Terrains->Draw();
}
E_EVENT(OnFrame_LOGGED)
{
	Terrains->Center.x = 10000 - RADIUS*0.98;
	Terrains->Center.y = 19000 + RADIUS*0.4;
	Terrains->Draw();
}
E_EVENT(OnFrame_PLAYING)
{
	Player.Position.x = X;
	Player.Position.y = Y;
	Terrains->Center.x = X - CELL_SIZE*Zoom/1.5;
	Terrains->Center.y = Y + CELL_SIZE*Zoom/1.5;
	Terrains->GetPoint(&Player.Position);
	p->SetCameraPos(X + CELL_SIZE*Zoom, Y - CELL_SIZE*Zoom, (Zoom + 1)*CELL_SIZE + Player.Position.z);
	p->SetCameraDir(-1, 1, -1);

	/*TCHAR szText[100];
	_stprintf_s(szText, TEXT("%.0f, %.0f"), X / CELL_SIZE, Y / CELL_SIZE);
	Position->SetText(szText);*/
	Player.Draw(p->GetStatus()->Elapsed);
	X = Player.Position.x;
	Y = Player.Position.y;

	Terrains->Draw();
}

E_EVENT(OnFrame)
{
	switch (Game)
	{
	case CS_NOTLOGGED:
		p->SetFogLinear(0.25, 0.9);
		p->SetAmbientLight((DWORD)0x00000000);
		//p->BindCamera(false);
		ServerAnimation(p, p->GetStatus()->Elapsed);
		break;
	case CS_LOGGED:
		p->SetFogLinear(0.45, 0.9);
		p->SetAmbientLight((DWORD)0x00000000);
		//p->BindCamera(false);
		p->SetCameraDir(-10, 2, -1);
		p->SetCameraPos(10000, 19000, 800);
		//eng->SetCameraPos()
		break;
	case CS_PLAYING: {
		p->SetFogLinear(0.45, 0.9);
		//p->BindCamera(false);
		//if() p->SetAmbientLight(D3DXCOLOR(0.5, 0.8, 0.9, 1));

		//CharacterWalk(fElapsed);
		Player.Position.x=X;
		Player.Position.y=Y;
		Terrains->GetPoint(&Player.Position);
		p->SetCameraPos(X+CELL_SIZE*Zoom, Y-CELL_SIZE*Zoom, (Zoom+1)*CELL_SIZE);
		p->SetCameraDir(-1, 1, -1);

		TCHAR szText[100];
		_stprintf_s(szText, TEXT("%.0f,%.0f"), X / CELL_SIZE, Y / CELL_SIZE);
		Position->SetText(szText);
		Player.Draw(p->GetStatus()->Elapsed);
		X = Player.Position.x;
		Y = Player.Position.y;
		}
		break;
	}
	Terrains->Draw();
}

E_EVENT(OnReflections)
{
	Terrains->Draw();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: colocar código aquí.

    // Inicializar cadenas globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MU, szWindowClass, MAX_LOADSTRING);
   // MyRegisterClass(hInstance);

	Gate.Load(TEXT("./Data/Gate.bmd"));
	Text.Load(TEXT("./Data/Local/Text.bmd"));
	MoveReq.Load(TEXT("./Data/Local/Movereq.bmd"));
	Item.Load(TEXT("./Data/Local/Item.bmd"));

	INT ww = GetPrivateProfileInt(TEXT("Game"), TEXT("Width"), 640, TEXT("./config.mu.ini"));
	INT wh = GetPrivateProfileInt(TEXT("Game"), TEXT("Height"), 480, TEXT("./config.mu.ini"));
	bool fs = GetPrivateProfileInt(TEXT("Game"), TEXT("Full_Screen"), 0, TEXT("./config.mu.ini"))==1;

	eng = new CEngine(hInstance, szWindowClass, ww, wh, fs, OnDeviceCreate, WndProc, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MU)));
	Screen = new CLoadSplash();
	Terrains = new CTerrain(74);
	TextureManager = new CTexMng();
	TextureManager->AsociateEngine(eng);
	
	Interface = eng->GUI;
	StartMuUI();
	ResetMuUI();
	//eng->UseRightHand();
	eng->SetStatus(Z_Angle, 0);
	eng->SetStatus(X_Angle, 0);
	eng->SetStatus(Z_Speed, 0.15);
	eng->SetStatus(X_Speed, 0.15);
	eng->SetAmbientLight((DWORD)0x00000000);
	eng->SetFarNearPlanes(RADIUS*2);
	eng->SetFogLinear(0.25, 0.9);
	eng->SetCallBack(OnFrame_NOTLOGGED);
	eng->FrameReflections = OnReflections;
	eng->OnReset = OnDeviceReset;

	eng->AddLoadQueueElement(TextureManager);
	eng->AddLoadQueueElement(Terrains);

	InitializeClasses();
	InitializeItem();

	eng->SetLoadScreen(Screen);
	eng->StartLoad();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MU));

	int ret = eng->Loop(hAccelTable);

	eng->Kill();

	delete eng;
	delete Terrains;
	delete TextureManager;
	delete Screen;

    return ret;
}



//
//  FUNCIÓN: MyRegisterClass()
//
//  PROPÓSITO: registrar la clase de ventana.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MU));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_MU);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCIÓN: InitInstance(HINSTANCE, int)
//
//   PROPÓSITO: guardar el identificador de instancia y crear la ventana principal
//
//   COMENTARIOS:
//
//        En esta función, se guarda el identificador de instancia en una variable común y
//        se crea y muestra la ventana principal del programa.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Almacenar identificador de instancia en una variable global

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCIÓN: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PROPÓSITO:  procesar mensajes de la ventana principal.
//
//  WM_COMMAND  - procesar el menú de aplicaciones
//  WM_PAINT    - Pintar la ventana principal
//  WM_DESTROY  - publicar un mensaje de salida y volver
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Interface->WndProtocol(message, wParam, lParam);
	int mx = 0xffff & lParam, my = 0xffff & (lParam >> 16);
	D3DXVECTOR3 gt;
    switch (message)
    {
	case WM_LBUTTONUP:
		gt = D3DXVECTOR3(mx, my, 0);
		if (Terrains->TestRay(&gt))
		{
			BYTE CharPath[MAX_PATH][2];
			int PathLen;
			CPathFinding ruta(Terrains->Attributes, X / CELL_SIZE, Y / CELL_SIZE, gt.x/CELL_SIZE, gt.y/CELL_SIZE);
			ruta.GetPath((BYTE*)CharPath, &PathLen);
			Player.SetPath((BYTE*)CharPath, PathLen);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizar las selecciones de menú:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
				return 0;// DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Agregar cualquier código de dibujo que use hDC aquí...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
		return 1;
    }
    return 0;
}

// Controlador de mensajes del cuadro Acerca de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
