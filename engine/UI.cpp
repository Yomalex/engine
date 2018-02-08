#include "stdafx.h"
#include "Engine.h"
#include <tchar.h>

const TCHAR * szDsk = TEXT("DESKTOP");
_tagScrollBarInfo g_ScrollInfo;
LPDIRECT3DTEXTURE9 lpWhite=nullptr;

CUI::CUI(CEngine*eng)
{
	this->Desktop = new CWindow(eng);
	this->Desktop->Show();
	this->Desktop->SetText(szDsk);
	this->Eng = eng;
}

CUI::~CUI()
{
	SAFE_RELEASE(lpWhite);
	delete this->Desktop;
}

void CUI::Clear()
{
	delete this->Desktop;
	this->Desktop = new CWindow(Eng);
	this->Desktop->Show();
	this->Desktop->SetText(szDsk);
}

void CUI::Draw()
{
	this->Eng->Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
	this->Desktop->Draw();
	this->Eng->Sprite()->End();
}

void CUI::OffScreenDraw()
{
	for (std::list<CUIModel*>::iterator it = this->vModels.begin(); it != this->vModels.end(); it++)
	{
		(*it)->DrawOff();
	}
}

bool CUI::WndProtocol(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (this->Desktop->Protocol(msg, wParam, lParam)) return true;

	if (WM_KEYUP != msg) return false;

	BYTE KeyUp = (BYTE)wParam;
	if (this->vBinds[KeyUp] == nullptr) return false;
	this->vBinds[KeyUp](this->Desktop, wParam);

	return true;
}

void CUI::OnLostDevice()
{
	for (std::list<CUIModel*>::iterator it = this->vModels.begin(); it != this->vModels.end(); it++)
	{
		(*it)->OnLostDevice();
	}
}

void CUI::OnResetDevice()
{
	for (std::list<CUIModel*>::iterator it = this->vModels.begin(); it != this->vModels.end(); it++)
	{
		(*it)->OnResetDevice();
	}
}

void CUI::BindKey(BYTE Key, UIEvent CallBack)
{
	this->vBinds[Key] = CallBack;
}

void CUI::Initialize()
{
	if (lpWhite == nullptr)
	{
		HRESULT hr = D3DXCreateTexture(this->Eng->GetDevice(), 1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &lpWhite);
		if (SUCCEEDED(hr))
		{
			D3DLOCKED_RECT r;
			lpWhite->LockRect(0, &r, nullptr, 0);
			*(D3DCOLOR*)r.pBits = D3DCOLOR_XRGB(255, 255, 255);
			lpWhite->UnlockRect(0);
		}
		else
		{
			this->Eng->Log(TEXT("%08X"), hr);
		}
	}
}

CWindow::CWindow():eng(nullptr),lpBackGround(nullptr),Parent(nullptr),Opacity(255)
{
}

CWindow::CWindow(CEngine * Eng):
	Parent(nullptr),KBFocus(nullptr),lpBackGround(nullptr),eng(Eng),Opacity(255)
{
}

CWindow::CWindow(const TCHAR * szText, int x, int y, CWindow * Parent, LPDIRECT3DTEXTURE9 lpTexture) : 
	KBFocus(nullptr), lpBackGround(lpTexture), Opacity(255)
{
	this->SetParent(Parent);
	this->SetText(szText);
	D3DSURFACE_DESC desc;
	lpTexture->GetLevelDesc(0, &desc);
	this->SetPos(x, y, desc.Width, desc.Height);
}

CWindow::CWindow(const TCHAR * szText, int x, int y, CWindow * Parent):
	KBFocus(nullptr), Opacity(255)
{
	this->SetParent(Parent);
	this->SetText(szText);
	this->SetPos(x, y);
	this->Opacity = 255;
}

CWindow::~CWindow()
{
	while (!this->Childrens.empty())
	{
		CWindow * c = this->Childrens.front();
		c->Parent = nullptr;
		delete c;
		this->Childrens.pop_front();
	}
	if (this->Parent) this->Parent->RemoveChild(this);
}

void CWindow::Initialize(CEngine* Eng)
{
	this->eng = Eng;
}

void CWindow::SetTexture(LPDIRECT3DTEXTURE9 lpTexture)
{
	this->lpBackGround = lpTexture;
}

void CWindow::SetText(const TCHAR * szText)
{
	if(szText!= nullptr) _tcscpy_s(this->szWindow, szText);
}

void CWindow::GetText(TCHAR * szReturned, int size)
{
	_tcscpy_s(szReturned, size, this->szWindow);
}

void CWindow::SetPos(int x, int y, int w, int h)
{
	this->X = (x >= 0) ? x : this->X;
	this->Y = (y >= 0) ? y : this->Y;
	this->W = (w >= 0) ? w : this->W;
	this->H = (h >= 0) ? h : this->H;

	this->DrawRect.left = 0;
	this->DrawRect.right = this->W;
	this->DrawRect.top = 0;
	this->DrawRect.bottom = this->H;
}

bool CWindow::Protocol(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (this->KBFocus != nullptr && this->KBFocus->Protocol(msg, wParam, lParam)) return true;
	if(msg> WM_KEYFIRST && msg <  WM_KEYLAST || msg==WM_LBUTTONUP)
		this->SetKeyboardFocus(nullptr);

	if(
		/*(msg>= WM_KEYFIRST && msg <=  WM_KEYLAST)||*/(msg>= WM_MOUSEFIRST && msg <=  WM_MOUSELAST)
		)

	for (std::list<CWindow*>::iterator it = this->Childrens.begin(); it != this->Childrens.end(); it++)
	{
		if (!(*it)->Visible()) continue;
		if((*it)->Protocol(msg, wParam, lParam)) return true;
	}

	return false;
}

void CWindow::Draw()
{
	int abx, aby;
	this->AbsolutePos(abx, aby);
	RECT r = { 0,0, this->W, this->H };

	if(this->lpBackGround!=nullptr) this->eng->Sprite()->Draw(
		this->lpBackGround,
		&r, 
		nullptr,
		&D3DXVECTOR3(abx, aby, 0),
		D3DCOLOR_ARGB(this->Opacity,255,255,255)
		);

	for (std::list<CWindow*>::iterator it = this->Childrens.begin(); it != this->Childrens.end(); it++)
	{
		if((*it)->Visible()) (*it)->Draw();
	}
	/*RECT r = { this->X,this->Y, this->W, this->H };
	this->eng->Text(this->szWindow,&r,0xFFFFFFFF);*/
}

float CWindow::AttachChild(CWindow * Child)
{
	this->Childrens.push_back(Child);
	Child->Initialize(this->eng);
	return this->d - 0.01;
}

void CWindow::RemoveChild(CWindow * Child)
{
	for (std::list<CWindow*>::iterator it = this->Childrens.begin(); it != this->Childrens.end(); it++)
	{
		if ((*it) == Child)
		{
			this->Childrens.erase(it);
			return;
		}
	}
}

void CWindow::SetKeyboardFocus(CWindow * Child)
{
	if(this->KBFocus != nullptr) this->KBFocus->Protocol(WM_KILLFOCUS, 0, 0);
	this->KBFocus = nullptr;
	if (this->Parent) this->Parent->SetKeyboardFocus(Child);
	else
	{
		this->KBFocus = Child;
		if(Child) Child->Protocol(WM_SETFOCUS,0,0);
	}
}

CWindow * CWindow::GetKeyboardFocus(void)
{
	if (this->Parent != nullptr) return this->Parent->GetKeyboardFocus();
	return this->KBFocus;
}

void CWindow::AbsolutePos(int & x, int & y)
{
	int px=0, py=0;
	if (this->Parent!=nullptr)this->Parent->AbsolutePos(px, py);
	x = this->X + px;
	y = this->Y + py;
}

bool CWindow::inBox(int x, int y)
{
	int abx, aby;
	this->AbsolutePos(abx, aby);
	return (x>=abx&&x<=abx+this->W)&&(y >= aby&&y <= aby + this->H);
}

void CWindow::SetParent(CWindow * p)
{
	if (this->Parent) this->Parent->RemoveChild(this);
	this->Parent = p;
	if(this->Parent) this->d = this->Parent->AttachChild(this);
}

void DefaultEmptyEvent(CWindow*Caller,LPARAM lParam)
{
	// None
}

CButton::CButton(int x, int y, LPDIRECT3DTEXTURE9 tx, int states, CWindow * parent)
{
	this->OnClick = DefaultEmptyEvent;
	this->Opacity = 255;
	this->R = 255;
	this->G = 255;
	this->B = 255;
	D3DSURFACE_DESC desc;
	this->SetParent(parent);
	this->SetTexture(nullptr);
	if (tx!=nullptr)
	{
		tx->GetLevelDesc(0, &desc);
		this->SetPos(x, y, desc.Width, desc.Height / states);
		this->SetTexture(tx);
	}
	else
	{
		this->SetPos(x, y);
	}
	this->SetStates(states);
	this->Disable = false;
	this->FontSize = 16;
	this->bShow = true;
}

CButton::~CButton()
{
}

bool CButton::Protocol(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (this->Disable || !this->bShow) return false;
	int mx = 0xffff & lParam, my = 0xffff & (lParam >> 16);
	switch (msg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
		if (wParam == this->FastKey)
		{
			if (msg == WM_KEYUP)
			{
				if (this->Parent != nullptr) this->Parent->Protocol(WM_COMMAND, MAKELONG(this->ID, 3), (LPARAM)this);
				this->OnClick(this, this->lParam);
			}
			return true;
		}
		break;
	case WM_MOUSEMOVE:
		if (this->inBox(mx, my)) this->Hover = true;
		else this->Hover = false;
		break;
	case WM_LBUTTONDOWN:
		if (this->Hover)
		{
			this->Down = true;
			return true;
		}
		break;
	case WM_LBUTTONUP:
		if (this->Down)
		{
			this->Down = false;
			if (this->Hover)
			{
				if (this->Parent != nullptr) this->Parent->Protocol(WM_COMMAND, MAKELONG(this->ID,3), (LPARAM)this);
				this->OnClick(this, this->lParam);
			}
			return true;
		}
		break;
	}
	return false;
}

void CButton::Draw()
{
	int ax, ay;
	this->AbsolutePos(ax, ay);
	int ty;
	if (this->States==2)
	{
		ty = (this->Down) ? 1 : 0;
	}
	else if(this->States == 3)
	{
		ty = (this->Down) ? 2 : (this->Hover)? 1 : 0;
	}
	else
	{
		ty = (this->Disable)? 3 : (this->Down) ? 2 : (this->Hover) ? 1 : 0;
	}
	RECT r = {
		0, this->H*ty, this->W, this->H+this->H*ty
	};
	if(this->lpBackGround!=nullptr)
	{
		this->eng->Sprite()->Draw(
		this->lpBackGround,
		&r,
		nullptr,
		&D3DXVECTOR3(ax, ay, 0),
			D3DCOLOR_ARGB(this->Opacity, 255, 255, 255)
		);
	}
	else
	{
		this->eng->Sprite()->Draw(
			lpWhite,
			&r,
			nullptr,
			&D3DXVECTOR3(ax, ay, 0),
			(this->Hover&&this->Disable==false) ? D3DCOLOR_ARGB(190, 50, 50, 50) :D3DCOLOR_ARGB(190, 0, 0, 0)
			);
	}

	if (_tcslen(this->szWindow) > 0)
	{
		RECT rt = { ax + 4,ay + 4,ax + this->W - 4,ay + this->H - 4 };
		this->eng->Sprite()->Flush();
		//this->eng->Text(this->szWindow, &rt, D3DCOLOR_ARGB(this->Opacity, this->R, this->G, this->B), DT_CENTER);
		this->eng->TextManager->JustifyCenter();
		this->eng->TextManager->SetColor(D3DCOLOR_ARGB(this->Opacity, this->R, this->G, this->B));
		this->eng->TextManager->VerticalCentered(TRUE);
		this->eng->TextManager->Draw(this->szWindow, this->FontSize, ax, ay, this->W, this->H);
		this->eng->TextManager->JustifyLeft();
	}
}

void CButton::SetStates(int st)
{
	this->States = st;
}

CProgressBar::CProgressBar(const TCHAR * szName, LPDIRECT3DTEXTURE9 lpTex, int x, int y, CWindow * parent)
{
	this->SetParent(parent);
	this->SetTexture(lpTex);
	D3DSURFACE_DESC desc;
	lpTex->GetLevelDesc(0, &desc);
	this->Tag = new CStatic(szName, x, y, 0, 0, this);
	this->Tag->SetBackGround(D3DCOLOR_ARGB(190,0,0,0));
	this->SetPos(x, y, desc.Width, desc.Height/2);
	this->oldvalue = 0;
	this->prog = 0;
	this->max = 100;
	this->Opacity = 255;
	this->R = 255;
	this->G = 255;
	this->B = 255;
	this->bShow = true;
}

CProgressBar::CProgressBar(const TCHAR * szName, LPDIRECT3DTEXTURE9 lpTexEmpty, LPDIRECT3DTEXTURE9 lpTexFull, int x, int y, CWindow * parent)
{
	this->SetParent(parent);
	this->SetTexture(lpTexEmpty);
	D3DSURFACE_DESC desc;
	if(lpTexEmpty) lpTexEmpty->GetLevelDesc(0, &desc);
	else lpTexFull->GetLevelDesc(0, &desc);

	this->TwoTextures = true;
	this->TexFull = lpTexFull;

	this->Tag = new CStatic(szName, x, y, 0, 0, this);
	this->Tag->SetBackGround(D3DCOLOR_ARGB(190, 0, 0, 0));
	this->SetPos(x, y, desc.Width, desc.Height);
	this->oldvalue = 0;
	this->prog = 0;
	this->max = 100;
	this->Opacity = 255;
	this->R = 255;
	this->G = 255;
	this->B = 255;
	this->bShow = true;
}

CProgressBar::~CProgressBar()
{
	if(this->Parent) this->Parent->RemoveChild(this->Tag);
	delete this->Tag;
}

bool CProgressBar::Protocol(UINT msg, WPARAM wParam, LPARAM lParam)
{
	int mx = 0xffff & lParam, my = 0xffff & (lParam >> 16), ax, ay;
	bool mi = this->inBox(mx, my);
	if (msg==WM_MOUSEMOVE)
	{
		this->AbsolutePos(ax, ay);
		this->Tag->SetPos(mx-ax+50, my-ay);
		if (mi) this->Tag->Show();
		else this->Tag->Hide();
	}
	return false;
}

void CProgressBar::Draw()
{
	float d = this->prog - this->oldvalue;
	this->oldvalue += d*eng->GetStatus()->Elapsed;
	this->oldvalue = max(min(this->oldvalue, this->max),0);
	int Ax, Ay;
	this->AbsolutePos(Ax, Ay);

	if (this->TwoTextures)
	{
		int H2 = this->H*(1.0 - this->oldvalue / this->max);
		RECT r1 = { 0, 0,this->W,this->H };
		RECT r2 = { 
			0, //x
			(this->Vertical) ? H2 :0,//y
			(this->Vertical) ? this->W :this->W*(this->oldvalue / this->max),//x2
			this->H//y2
		};

		if (this->lpBackGround != nullptr) this->eng->Sprite()->Draw(
			this->lpBackGround,
			&r1,
			nullptr,
			&D3DXVECTOR3(Ax, Ay, 0),
			D3DCOLOR_ARGB(this->Opacity,255,255,255)
			);
		if (this->TexFull != nullptr) this->eng->Sprite()->Draw(
			this->TexFull,
			&r2,
			nullptr,
			&D3DXVECTOR3(Ax, Ay + ((this->Vertical) ? H2 : 0), 0),
			D3DCOLOR_ARGB(this->Opacity, 255, 255, 255)
			);
	}
	else
	{
		RECT r1 = { 0, 0,this->W,this->H };
		RECT r2 = { 0, this->H,this->W*(this->oldvalue / this->max),this->H * 2.0 };

		this->eng->Sprite()->Draw(
			this->lpBackGround,
			&r1,
			nullptr,
			&D3DXVECTOR3(Ax, Ay, 0),
			D3DCOLOR_ARGB(this->Opacity, 255, 255, 255)
			);
		this->eng->Sprite()->Draw(
			this->lpBackGround,
			&r2,
			nullptr,
			&D3DXVECTOR3(Ax, Ay, 0),
			D3DCOLOR_ARGB(this->Opacity, 255, 255, 255)
			);
	}

	if (this->Tag->Visible()) this->Tag->Draw();
}

CStatic::CStatic(const TCHAR * szText, int x, int y, int w, int h, CWindow * parent)
{
	this->SetParent(parent);
	this->SetText(szText);
	RECT empty = { 0,0,0,0 };
	this->eng->Text(
		this->szWindow,
		&empty,
		0xffffffff,
		DT_CALCRECT
		);

	this->W = empty.right - empty.left;
	this->H = empty.bottom - empty.top;
	this->SetPos(x, y, (w>0)?w:-1, (h>0)?h:-1);

	this->Opacity = 255;
	this->R = 255;
	this->G = 255;
	this->B = 255;

	this->dwBG = 0;
	this->bShow = true;
}

CStatic::~CStatic()
{
}

bool CStatic::Protocol(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void CStatic::Draw()
{
	int ax, ay;
	this->AbsolutePos(ax, ay);
	RECT empty = { 0,0,0,0 };
	if (this->W==0||this->H==0)
	{
		this->eng->Text(
			this->szWindow,
			&empty,
			0xffffffff,
			DT_CALCRECT
			);

		if(this->W==0) this->W = empty.right - empty.left;
		if(this->H==0) this->H = empty.bottom - empty.top;
	}

	RECT r = { ax,ay,this->W,this->H };
	RECT r2 = { 0,0,this->W+8,this->H+8 };
	this->eng->Sprite()->Draw(lpWhite, &r2, nullptr, &D3DXVECTOR3(ax-4,ay-4,0), this->dwBG);
	this->eng->Sprite()->Flush();
	this->eng->Text(
		this->szWindow,
		&r,
		D3DCOLOR_ARGB(this->Opacity, this->R, this->G, this->B)
		);
}

CImage::CImage(int x, int y, LPDIRECT3DTEXTURE9 lpTex, CWindow * parent)
{
	this->bShow = false;
	if (lpTex == nullptr)
		return;
	this->SetParent(parent);
	this->SetTexture(lpTex);
	D3DSURFACE_DESC desc;
	lpTex->GetLevelDesc(0, &desc);
	this->SetPos(x, y, desc.Width, desc.Height);

	this->Opacity = 255;
	this->R = 255;
	this->G = 255;
	this->B = 255;
	this->bShow = true;
}

CImage::~CImage()
{
}

bool CImage::Protocol(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void CImage::Draw()
{
	if (!this->bShow)
		return;

	int ax, ay;
	RECT r = { 0,0,this->W, this->H };
	this->AbsolutePos(ax, ay);
	this->eng->Sprite()->Draw(this->lpBackGround,
		&r,
		nullptr,
		&D3DXVECTOR3(ax, ay, 0),
		D3DCOLOR_ARGB(this->Opacity,255,255,255)
		);
}

CEdit::CEdit(int x, int y, int w, int h, CWindow * parent)
{
	this->SetParent(parent);
	this->SetPos(x, y, w, h);

	this->Opacity = 255;
	this->R = 255;
	this->G = 255;
	this->B = 255;

	this->CurrentView = 0;
	this->bShow = true;
}

CEdit::CEdit(int x, int y, LPDIRECT3DTEXTURE9 lpText, CWindow * parent)
{
	this->SetParent(parent);
	D3DSURFACE_DESC desc;
	lpText->GetLevelDesc(0, &desc);
	this->SetPos(x, y, desc.Width, desc.Height);
	this->SetTexture(lpText);
	this->SetText(TEXT(""));

	this->Opacity = 255;
	this->R = 255;
	this->G = 255;
	this->B = 255;

	this->CurrentView = 0;
	this->bShow = true;
}

CEdit::~CEdit()
{
}

void InsertChar(TCHAR * inout, int len, const TCHAR c, int& pos)
{
	int ilen = _tcslen(inout);
	if (pos >= len-1) return;
	TCHAR * tmp = new TCHAR[ilen + 2];
	ZeroMemory(tmp, (ilen + 2)*sizeof(TCHAR));
	for (int i = 0; i < pos; i++)
	{
		tmp[i] = inout[i];
	}
	tmp[pos] = c;
	for (int i = pos; i < ilen; i++)
	{
		tmp[i+1] = inout[i];
	}
	pos++;
	_tcscpy_s(inout, len, tmp);
	delete[] tmp;
}

bool CEdit::Protocol(UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool mIn = this->inBox(0xffff & lParam, 0xffff & (lParam >> 16));
	int ccpy;
	TCHAR tmpsz[1024];
	RECT empty = { 0,0,0,0 };
	switch (msg)
	{
	case WM_SETFOCUS:
		return true;
	case WM_KILLFOCUS:
		return true;
	case WM_MOUSEMOVE:
		if(mIn)
		{
			//hover event
			return true;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		if (mIn)
		{
			this->Parent->SetKeyboardFocus(this);
			this->FindCaretpos(0xffff & lParam, 0xffff & (lParam >> 16));
			this->last_caret = 0;
			return true;
		}
		break;
	case WM_CHAR:
		this->last_caret = 0;
		switch (wParam)
		{
		case 0x08:  // backspace
			if (this->Cursor)
			{
				TCHAR szCopy[MAX_STRING];
				_tcscpy_s(szCopy, this->szWindow + this->Cursor);
				this->Cursor--;
				_tcscpy_s(this->szWindow + this->Cursor, MAX_STRING-this->Cursor, szCopy);
				if (this->Cursor<this->CurrentView)
				{
					this->CurrentView = this->Cursor-4;
					this->CurrentView = max(this->CurrentView, 0);
				}
				this->UpdateCaret();
			}
			break;
		case VK_TAB:
		case VK_RETURN:
		case VK_ESCAPE:
			return false;
		/*case 0x09://tab
			break;
		case 0x0D:  // carriage return 
			// OnReturn
			break;*/
		default:
			if (this->Cursor > _tcslen(this->szWindow))
			{
				this->Cursor = _tcslen(this->szWindow);
				this->CurrentView = 0;
			}
			InsertChar(this->szWindow, MAX_STRING, wParam, this->Cursor);
			this->ViewPort();
		}
		for (int i = 0; i < _tcslen(this->szWindow); i++)
		{
			this->szPassword[i] = TEXT('*');
			this->szPassword[i+1] = NULL;
		}
		return true;
	case WM_KEYUP:
		return true;
	case WM_KEYDOWN:
		if (wParam == VK_LEFT && this->Cursor>0)
		{
			this->Cursor--;
			if (this->Cursor<this->CurrentView)
			{
				this->CurrentView = this->Cursor-4;
				this->CurrentView = max(this->CurrentView, 0);
			}
			this->UpdateCaret();
			return true;
		}
		else if (wParam == VK_RIGHT && this->Cursor<_tcslen(this->szWindow))
		{
			this->Cursor++;
			this->ViewPort();
			return true;
		}
		switch(wParam)
		{
			case VK_HOME:
				this->Cursor = 0;
				this->CurrentView = 0;
				this->ViewPort();
				break;
			case VK_END:
				this->Cursor = _tcslen(this->szWindow);
				this->CurrentView = 0;
				this->ViewPort();
				break;
				return true;
		}
	}
	return false;
}

void CEdit::Draw()
{
	int ax, ay;
	this->AbsolutePos(ax, ay);
	RECT r = {ax+4,ay,ax+this->W-4, ay+this->H};
	if (this->Cursor > _tcslen(this->szWindow))
	{
		this->Cursor = _tcslen(this->szWindow);
		this->CurrentView = 0;
	}
	if (this->lpBackGround != nullptr)
	{
		this->eng->Sprite()->Draw(this->lpBackGround, nullptr, nullptr, &D3DXVECTOR3(ax, ay, 0), D3DCOLOR_ARGB(this->Opacity,255,255,255));
	}
	this->eng->Sprite()->Flush();

	if(!this->bPassword) this->eng->Text(this->szWindow+this->CurrentView, &r, D3DCOLOR_ARGB(this->Opacity,this->R,this->G,this->B), DT_LEFT | DT_VCENTER);
	else this->eng->Text(this->szPassword, &r, D3DCOLOR_ARGB(this->Opacity,this->R,this->G,this->B), DT_LEFT | DT_VCENTER);

	if (this->GetKeyboardFocus() == this)
	{
		r.left = this->CaretX+ax+4;
		r.top += 2;
		if(this->last_caret<1.0) this->eng->Text(TEXT("|"),&r,0xFFFFFFFF);
		else if(this->last_caret>2.0) this->last_caret = 0;

		this->last_caret += eng->GetStatus()->Elapsed;
	}
}

void CEdit::ViewPort()
{
	TCHAR szView[1024];
	RECT e = { 0,0,0,0 };
	while (true) {
		ZeroMemory(szView, 1024);
		ZeroMemory(&e, sizeof(RECT));

		memcpy(szView, this->szWindow + this->CurrentView, (this->Cursor - this->CurrentView+1)*sizeof(TCHAR));
		this->eng->Text(szView, &e, 0xffffffff, DT_CALCRECT);
		if (e.right - e.left <= this->W-8) break;
		this->CurrentView++;
		if (this->CurrentView >= this->Cursor) break;
	}

	this->UpdateCaret();
}

void CEdit::FindCaretpos(int x, int y)
{
	this->CaretX = 0;
	int abx, aby;
	this->AbsolutePos(abx, aby);
	for (int i = 0; i < _tcslen(this->szWindow); i++)
	{
		if (i<this->CurrentView) continue;
		if (this->CaretX + this->iWidth[i] < x-abx)
		{
			this->Cursor = i;
			this->CaretX += this->iWidth[i];
			continue;
		}
	}
}

void CEdit::UpdateCaret()
{
	RECT e;
	TCHAR szView[3];
	ZeroMemory(this->iWidth, sizeof(this->iWidth));
	this->CaretX = 0;
	for (int i = 0; i < _tcslen(this->szWindow); i++)
	{
		ZeroMemory(&e, sizeof(RECT));
		szView[0] = (this->szWindow[i]!=' ')?this->szWindow[i]:'-';
		szView[1] = 0;
		this->eng->Text(szView, &e, 0xffffffff, DT_CALCRECT);
		this->iWidth[i] = e.right - e.left;
		if (i >= this->Cursor || i<this->CurrentView) continue;
		this->CaretX += this->iWidth[i];
	}
	this->last_caret = 0;
	int abx, aby;
	this->AbsolutePos(abx, aby);
	//SetCaretPos(CaretX + abx, aby);
}

CUIModel::CUIModel(int x, int y, int w, int h, int bw, int bh, CEngineElement * Model, CWindow * parent)
{
	this->SetParent(parent);
	this->SetPos(x, y, w, h);
	this->mdl = Model;
	this->eng->AddLoadQueueElement(Model);
	BackBuffer[0] = bw;
	BackBuffer[1] = bh;
	this->OnResetDevice();
	this->eng->GUI->vModels.push_back(this);

	this->Opacity = 255;
	this->R = 255;
	this->G = 255;
	this->B = 255;
	this->bShow = true;
}

CUIModel::~CUIModel()
{
	this->eng->Release(this->mdl);
	SAFE_RELEASE(this->lpBackGround);
	for (std::list<CUIModel*>::iterator it = this->eng->GUI->vModels.begin();it != this->eng->GUI->vModels.end(); it++)
	{
		if ((*it) == this)
		{
			this->eng->GUI->vModels.erase(it);
			break;
		}
	}
}

bool CUIModel::Protocol(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void CUIModel::DrawOff()
{
	if (!this->bShow || !this->Parent->Visible()) return;

	this->eng->PushCamera(
		this->Center+D3DXVECTOR3(0, -this->zBack, 0),
		D3DXVECTOR3(0, this->zBack, 0),
		D3DXVECTOR3(0, 0, 1)
		);
	this->eng->SetRenderTarget(this->lpBackGround);
	this->eng->GetDevice()->Clear(1, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1, 0);
	this->eng->GetDevice()->BeginScene();
	this->eng->SetProjectionMatrix(false);
	//this->eng->SetOrthoMatrix(false);
	this->eng->PushWorld(&this->Transform);//mT
	this->eng->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	this->mdl->Draw();
	this->eng->PopWorld();
	this->eng->PopCamera();
	this->eng->GetDevice()->EndScene();
	this->eng->SetBackBufferTarget();
}

void CUIModel::SetBoundingBox(D3DXVECTOR3 bb[2])
{
	D3DXVECTOR4 tmp;
	D3DXVECTOR3 bbc[2];
	
	for (int i = 0; i < 2; i++)
	{
		D3DXVec3Transform(&tmp, &bb[i], &this->Transform);
		//memcpy(bb + 1, &tmp, sizeof(D3DXVECTOR3));
		bbc[i] = D3DXVECTOR3(tmp.x, tmp.y, tmp.z);
	}

	this->Center = (bbc[0] + bbc[1]) / 2.0;
	this->zBack = bbc[1].y*3.5;
}

void CUIModel::OnLostDevice()
{
	SAFE_RELEASE(this->lpBackGround);
}

void CUIModel::OnResetDevice()
{
	HRESULT hr = this->eng->GetDevice()->CreateTexture(
		BackBuffer[0], BackBuffer[1], 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &this->lpBackGround, NULL);
	if (FAILED(hr))
	{
		this->eng->LOG(TEXT("CreateTexture %s"), DXGetErrorDescription(hr));
	}
}

CScroll::CScroll(int x, int y, int imax, _tagScrollBarInfo* Texture, CWindow * parent, bool HScroll)
{
	this->SetParent(parent);
	D3DSURFACE_DESC desc;
	Texture->ScrollOn->GetLevelDesc(0, &desc);
	this->SetPos(x, y, desc.Width, desc.Height);
	this->bHScroll = HScroll;
	this->OriginalPos = (HScroll) ? x : y;
	this->MaxPos = imax;
	//memcpy(&this->TextureInfo, Texture, sizeof(_tagScrollBarInfo));
	this->TextureInfo = Texture;

	this->Opacity = 255;
	this->R = 255;
	this->G = 255;
	this->B = 255;
	this->bShow = true;
}

CScroll::~CScroll()
{
	this->Parent->RemoveChild(this);
}

bool CScroll::Protocol(UINT msg, WPARAM wParam, LPARAM lParam)
{
	int mx= 0xffff & lParam, my= 0xffff & (lParam >> 16);
	bool mi = this->inBox(mx, my);
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		if (mi)
		{
			this->bDrag = true;
			this->dx = mx;
			this->dy = my;
			return true;
		}
		this->bDrag = false;
		break;
	case WM_LBUTTONUP:
		if (this->bDrag)
		{
			if(this->bHScroll)
				this->Parent->Protocol(WM_HSCROLL, MAKELONG(SB_THUMBPOSITION, this->X - this->OriginalPos), (LPARAM)this);
			else
				this->Parent->Protocol(WM_VSCROLL, MAKELONG(SB_THUMBPOSITION, this->Y - this->OriginalPos), (LPARAM)this);
		}
		this->bDrag = false;
		if (mi)
			return true;
		break;
	case WM_MOUSEMOVE:
		if (this->bDrag)
		{
			if (this->bHScroll)
			{
				int delta = this->dx - mx;
				this->X += delta;
				this->X = max(min(this->X, this->OriginalPos + this->MaxPos), this->OriginalPos);
				this->Parent->Protocol(WM_HSCROLL, MAKELONG(SB_THUMBTRACK,this->X-this->OriginalPos), (LPARAM)this);
			}
			else
			{
				int delta = -(this->dy - my);
				this->Y += delta;
				this->dy = my;
				this->Y = max(min(this->Y, this->OriginalPos + this->MaxPos), this->OriginalPos);
				this->Parent->Protocol(WM_VSCROLL, MAKELONG(SB_THUMBTRACK, this->Y - this->OriginalPos), (LPARAM)this);
			}
		}
		break;
	}
	return false;
}

void CScroll::Draw(float fElapsed)
{
	int abx, aby;
	this->AbsolutePos(abx, aby);

	D3DSURFACE_DESC desca, descb;
	this->TextureInfo->Top->GetLevelDesc(0, &desca);
	this->TextureInfo->ScrollOn->GetLevelDesc(0, &descb);

	int Cwa = desca.Width/2, Cwb = descb.Width / 2;

	if(this->Parent!=nullptr)
		if (this->bHScroll)
		{
			this->OriginalPos = 0;
			this->MaxPos = this->Parent->GetWidth();

			this->X = min(this->X, this->MaxPos);
		}
		else
		{
			this->OriginalPos = 0;
			this->MaxPos = this->Parent->GetHeight();

			this->Y = min(this->Y, this->MaxPos);
		}

	if (!this->bHScroll) abx += Cwb;
	else aby += descb.Height;

	this->eng->Sprite()->Draw(
		this->TextureInfo->Top,
		nullptr,
		nullptr,
		&D3DXVECTOR3(
			(this->bHScroll)? abx-this->X+this->OriginalPos : abx-Cwa, 
			(this->bHScroll) ? aby : aby-this->Y+this->OriginalPos, 0),
		D3DCOLOR_ARGB(this->Opacity,255,255,255)
		);

	RECT rBG = {
		0,
		0,
		(this->bHScroll)? this->MaxPos - desca.Width:desca.Width,
		(this->bHScroll) ? desca.Height : this->MaxPos- desca.Height };

	this->eng->Sprite()->Draw(
		this->TextureInfo->Mid,
		&rBG,
		nullptr,
		&D3DXVECTOR3(
			(this->bHScroll) ? abx - this->X + this->OriginalPos + desca.Width : abx - Cwa,
			(this->bHScroll) ? aby : aby - this->Y + this->OriginalPos + desca.Height, 0),
		D3DCOLOR_ARGB(this->Opacity,255,255,255)
		);

	this->eng->Sprite()->Draw(
		this->TextureInfo->Bot,
		nullptr,
		nullptr,
		&D3DXVECTOR3((this->bHScroll) ? abx - this->X + this->OriginalPos+this->MaxPos : abx-Cwa,
			(this->bHScroll) ? aby : aby - this->Y+this->OriginalPos + this->MaxPos, 0),
		D3DCOLOR_ARGB(this->Opacity,255,255,255)
		);

	this->eng->Sprite()->Draw(
		(this->bEnable)?this->TextureInfo->ScrollOn:this->TextureInfo->ScrollOff,
		nullptr,
		nullptr,
		&D3DXVECTOR3(abx-Cwb, aby, 0),
		D3DCOLOR_ARGB(this->Opacity,255,255,255)
		);
}

CMenu::CMenu(int x, int y, int w, int h, CWindow * parent)
{
	this->SetParent(parent);
	this->SetPos(x, y, w, h);
	this->MenuScroll = new CScroll(w, 0, h, &g_ScrollInfo, this);
	//this->MenuScroll->Show();
	this->MenuScroll->Enable(false);
	this->OnSelect = DefaultEmptyEvent;

	this->Opacity = 255;
	this->R = 255;
	this->G = 255;
	this->B = 255;
	this->bShow = true;
}

CMenu::~CMenu()
{
	while (!this->vMenu.empty())
	{
		delete (this->vMenu.back()).bt;
		this->vMenu.pop_back();
	}
	if (this->Parent != nullptr) this->Parent->RemoveChild(this);
}

bool CMenu::Protocol(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_VSCROLL:
	{
		CScroll * pScroll = (CScroll*)lParam;
		int Pos = HIWORD(wParam);
		int ElementOverflow = this->vMenu.size()*MENU_ELEMENT_H-this->H;
		int OFCount = (int)ceil((float)ElementOverflow / (float)MENU_ELEMENT_H);
		int Element=0;
		int ScrollPerElement = this->H / OFCount;
		int EVisibles = this->H / MENU_ELEMENT_H;
		int dis;
		int Proximity = 0xffff;
		for (int i = 0; i < OFCount; i++)
		{
			dis = abs(i*ScrollPerElement - Pos);
			if (Proximity > dis)
			{
				Proximity = dis;
				Element = i;
			}
		}

		for (int i = 0; i < this->vMenu.size(); i++)
		{
			if (i < Element || i - Element >= EVisibles)
				this->vMenu[i].bt->Hide();
			else
			{
				this->vMenu[i].bt->Show();
				this->vMenu[i].bt->SetPos(-1, (i - Element)*MENU_ELEMENT_H);
			}
		}

		if (LOWORD(wParam) == SB_THUMBPOSITION)
		{
			pScroll->SetPos(-1, Element*ScrollPerElement);
		}
	}
		return true;
	case WM_COMMAND:
	{
		WORD wID = LOWORD(wParam);
		WORD wEvent = HIWORD(wParam);
		if (wEvent != 3) return false;
		if (this->Parent) this->Parent->Protocol(WM_MENUSELECT, MAKELONG(wID, NULL), lParam);
		this->OnSelect((CWindow*)lParam, this->lParam);
	}	
		break;
	}


	for (std::list<CWindow*>::iterator it = this->Childrens.begin(); it != this->Childrens.end(); it++)
	{
		if ((*it)->Protocol(msg, wParam, lParam)) return true;
	}

	return false;
}

void CMenu::AddElement(const TCHAR * szText, DWORD dwValue)
{
	_tagMenuElement tmp;
	tmp.Value = dwValue;
	int btY = this->vMenu.size()*MENU_ELEMENT_H;
	tmp.bt = new CButton(0, btY, nullptr, 1, this);
	tmp.bt->SetPos(-1, -1, this->W, MENU_ELEMENT_H);
	tmp.bt->Show();
	tmp.bt->ID = this->vMenu.size();
	tmp.bt->SetText(szText);
	tmp.bt->SetFontSize(MENU_ELEMENT_H - 4);

	if (btY >= this->H- MENU_ELEMENT_H)
	{
		this->MenuScroll->Show();
		this->MenuScroll->Enable(true);
		tmp.bt->Hide();
	}

	this->vMenu.push_back(tmp);
}

void CMenu::GetElement(UINT element, TCHAR * szText, int imax, DWORD* dwValue)
{
	if (element >= this->vMenu.size()) return;
	if (szText != nullptr)
		this->vMenu[element].bt->GetText(szText, imax);
	if (dwValue != nullptr) *dwValue = this->vMenu[element].Value;
}

void CMenu::GetElement(CWindow * element, TCHAR * szText, int imax, DWORD * dwValue)
{
	this->GetElement((UINT)((CButton*)element)->ID, szText, imax, dwValue);
}

CJumpList::CJumpList(int x, int y, LPDIRECT3DTEXTURE9 Text, LPDIRECT3DTEXTURE9 Drop, int states, CWindow * parent)
{
	this->SetParent(parent);
	this->SetText(TEXT(""));
	this->OptionSelected = -1;
	this->Info = new CImage(0, 0, Text, this);
	this->DropList = new CButton(this->Info->GetWidth(), 0, Drop, states, this);
	this->DropList->ID = 0xffff;
	this->JumpMenu = new CMenu(0, this->Info->GetHeight(), this->Info->GetWidth(), 100, this);
	this->JumpMenu->Hide();
	this->OnChange = DefaultEmptyEvent;
	this->SetPos(x, y, this->Info->GetWidth()+ this->DropList->GetWidth());

	this->Info->Show();
	this->DropList->Show();

	this->Opacity = 255;
	this->R = 255;
	this->G = 255;
	this->B = 255;
	this->bShow = true;
}

CJumpList::~CJumpList()
{
	delete this->Info;
	delete this->DropList;
	delete this->JumpMenu;
}

bool CJumpList::Protocol(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_MENUSELECT:
		{
			WORD wID = LOWORD(wParam);
			this->JumpMenu->GetElement(wID, this->szWindow, 512, nullptr);
			this->JumpMenu->Hide();
			if (this->OptionSelected != wID)
			{
				this->OptionSelected = wID;
				this->OnChange(this, this->OptionSelected);
			}
		}
		return true;
	case WM_COMMAND:
		{
			WORD wID = LOWORD(wParam);
			WORD wEvent = HIWORD(wParam);
			if (wEvent != 3) break;
			if (wID==0xffff)
			{
				this->JumpMenu->Visible()? this->JumpMenu->Hide() :this->JumpMenu->Show();
				return true;
			}
			return true;
		}
	}

	for (std::list<CWindow*>::iterator it = this->Childrens.begin(); it != this->Childrens.end(); it++)
	{
		if (!(*it)->Visible()) continue;
		if ((*it)->Protocol(msg, wParam, lParam)) return true;
	}
	return false;
}

void CJumpList::Draw(float fElapsed)
{
	int abx, aby;
	this->AbsolutePos(abx,aby);
	RECT r = {abx+4,aby+4,abx+this->JumpMenu->GetWidth() -4,aby+this->JumpMenu->GetHeight()-4};
	for (std::list<CWindow*>::iterator it = this->Childrens.begin(); it != this->Childrens.end(); it++)
	{
		if((*it)->Visible()) (*it)->Draw();
	}
	this->eng->Sprite()->Flush();
	this->eng->Text(this->szWindow, &r, D3DCOLOR_ARGB(this->Opacity,this->R,this->G, this->B), DT_CENTER|DT_NOCLIP);
}

void CJumpList::SetSelected(UINT Element)
{
	if (Element >= this->JumpMenu->Count()) return;

	this->OptionSelected = Element;
	this->JumpMenu->GetElement(Element, this->szWindow, 512, nullptr);
}

CCheckBox::CCheckBox(int x, int y, LPDIRECT3DTEXTURE9 Texture, CWindow * parent)
{
	this->SetParent(parent);
	D3DSURFACE_DESC desc;
	Texture->GetLevelDesc(0, &desc);
	this->SetPos(x, y, desc.Width, desc.Height/2);
	this->TwoTextures = false;
	this->OnChange = DefaultEmptyEvent;
	this->SetTexture(Texture);

	this->Opacity = 255;
	this->R = 255;
	this->G = 255;
	this->B = 255;
	this->bShow = true;
}

CCheckBox::CCheckBox(int x, int y, LPDIRECT3DTEXTURE9 TextureUn, LPDIRECT3DTEXTURE9 TextureCh, CWindow * parent)
{
	this->SetParent(parent);
	D3DSURFACE_DESC desc;
	if(TextureUn!=nullptr) TextureUn->GetLevelDesc(0, &desc);
	else if (TextureCh != nullptr) TextureCh->GetLevelDesc(0, &desc);
	this->SetTexture(TextureUn);
	this->Secundary = TextureCh;
	this->SetPos(x, y, desc.Width, desc.Height);
	this->TwoTextures = true;
	this->OnChange = DefaultEmptyEvent;

	this->Opacity = 255;
	this->R = 255;
	this->G = 255;
	this->B = 255;
}

CCheckBox::~CCheckBox()
{
}

void CCheckBox::Draw(float fElapsed)
{
	int abx, aby;
	this->AbsolutePos(abx,aby);
	LPDIRECT3DTEXTURE9 TDest;
	if (this->TwoTextures)
	{
		TDest = (this->Checked) ? this->Secundary : this->lpBackGround;
	}
	else
	{
		TDest = this->lpBackGround;
		this->DrawRect.top = (!this->Checked) ? this->W: 0;
		this->DrawRect.bottom = (!this->Checked) ? 2*this->W : this->W;
	}
	if (TDest != nullptr)
		this->eng->Sprite()->Draw(
			TDest,
			&this->DrawRect,
			nullptr,
			&D3DXVECTOR3(abx, aby, 0),
			D3DCOLOR_ARGB(this->Opacity,255,255,255)
			);

	if (_tcslen(this->szWindow)) {
		this->eng->Sprite()->Flush();
		RECT Text = {abx+this->W+5,aby,100,30};
		this->eng->Text(this->szWindow, &Text, D3DCOLOR_ARGB(this->Opacity,this->R,this->G,this->B));
	}
}

bool CCheckBox::Protocol(UINT msg, WPARAM wParam, LPARAM lParam)
{
	int mx = 0xffff & lParam, my = 0xffff & (lParam >> 16);
	bool mi = this->inBox(mx, my);
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		if (!this->bHover) return false;
		this->bDown = true;
		return true;
	case WM_LBUTTONUP:
		if (!this->bHover) this->bDown = false;
		if(!this->bDown) return false;
		this->Checked = !this->Checked;
		this->OnChange(this, this->lParam);
		this->bDown = false;
		return true;
	case WM_MOUSEMOVE:
		if (mi)
		{
			this->bHover = true;
			return true;
		}
		this->bHover = false;
		break;
	}
	return false;
}

CRichBox::CRichBox(int x, int y, int w, int h, CWindow * parent)
{
	this->SetParent(parent);
	this->SetPos(x, y, w, h);
	this->pScroll = new CScroll(w, 0, h, &g_ScrollInfo, this);
	this->pScroll->Hide();
	this->pScroll->Enable(false);
	this->bShow = true;
}

CRichBox::~CRichBox()
{
	delete this->pScroll;
}

void CRichBox::innerText(const TCHAR * szFormated)
{
	int iLen = _tcslen(szFormated);
	bool text = false;

	TCHAR szTmp[1024] = { 0 }, *pTmp, szTmp2[1024] = {0};
	_Format tmp;
	tmp.Bold = FALSE;
	tmp.dwColor = 0xff000000;
	tmp.Italic = FALSE;
	tmp.X = RICH_OFFSET;
	tmp.Y = 0;
	pTmp = szTmp2;
	//std::list<_Format> vTmp;

	while (Sync) Sleep(1);
	Sync = true;

	while (!this->vLines.empty())
	{
		free(this->vLines.front().szText);
		this->vLines.pop_front();
	}
	ZeroMemory(szTmp, 1024);
	ZeroMemory(szTmp2, 1024);
	this->rowcount=0;
	for (int i = 0; i<=iLen; i++)
	{
		if (szFormated[i] == '~' && szFormated[i+1] == '#')
		{
			if (text)
			{
				if(_tcslen(szTmp2)>0) _tcscpy_s(szTmp, szTmp2);
				text = false;
				if (_tcslen(szTmp) == 0) continue;

				int need_w = 0;
				bool change = (szTmp[_tcslen(szTmp) - 1] == ' ') ? true : false;
				if(change) szTmp[_tcslen(szTmp) - 1] = '_';
				this->eng->TextManager->Calc(szTmp, RICH_SIZE, &need_w, nullptr, tmp.Bold, tmp.Italic);
				if (change) szTmp[_tcslen(szTmp) - 1] = ' ';

				int iLenSz = _tcslen(szTmp)+1;
				tmp.szText = (TCHAR*)malloc((iLenSz+1)*sizeof(TCHAR));
				_tcscpy_s(tmp.szText, iLenSz, szTmp);
				ZeroMemory(szTmp, 1024);
				ZeroMemory(szTmp2, 1024);
				pTmp = szTmp2;
				this->vLines.push_back(tmp);

				tmp.Bold = FALSE;
				tmp.dwColor = 0xff000000;
				tmp.Italic = FALSE;
				tmp.X += need_w;
			}
			switch (szFormated[i+2])
			{
			case 'C':
				_stscanf_s(&szFormated[i + 3], TEXT("%x"), &tmp.dwColor);
				i += 10;
				continue;
			case 'F':
				switch (szFormated[i + 3])
				{
				case 'B':
					tmp.Bold = TRUE;
				case 'N':
					break;
				case 'I':
					tmp.Italic = TRUE;
					break;
				}
				i += 3;
				continue;
			default:
				text = true;
			}
		}

		if (szFormated[i] == ' ' || szFormated[i] == '\n' || szFormated[i]==0)
		{
			int need_w = 0;
			this->eng->TextManager->Calc(szTmp2, RICH_SIZE, &need_w, nullptr, tmp.Bold, tmp.Italic);
			if (tmp.X + need_w > this->W)
			{
				int iLenSz = _tcslen(szTmp)+1;
				if (iLenSz==0) { iLenSz = _tcslen(szTmp2) + 1; _tcscpy_s(szTmp, szTmp2); }
				tmp.szText = (TCHAR*)malloc((iLenSz + 1)*sizeof(TCHAR));
				_tcscpy_s(tmp.szText, iLenSz, szTmp);
				ZeroMemory(szTmp, 1024);
				this->vLines.push_back(tmp);

				tmp.Y += RICH_SIZE;
				tmp.X = RICH_OFFSET;
				_tcscpy_s(szTmp, szTmp2 + iLenSz);
				ZeroMemory(szTmp2, 1024);
				_tcscpy_s(szTmp2, szTmp);
				pTmp = szTmp2 + _tcslen(szTmp);
				this->rowcount++;
			}
			else
			{
				_tcscpy_s(szTmp, szTmp2);
			}

			if (szFormated[i] == '\n' || szFormated[i] == 0)
			{
				int iLenSz = _tcslen(szTmp) + 1;
				tmp.szText = (TCHAR*)malloc((iLenSz + 1)*sizeof(TCHAR));
				_tcscpy_s(tmp.szText, iLenSz, szTmp);
				ZeroMemory(szTmp, 1024);
				ZeroMemory(szTmp2, 1024);
				this->vLines.push_back(tmp);

				tmp.Y += RICH_SIZE;
				tmp.X = RICH_OFFSET;
				pTmp = szTmp2;
				text = false;
				this->rowcount++;
				continue;
			}
		}
		else  if (szFormated[i] == '\r')
		{
			//tmp.X = 0;
			continue;
		}

		*pTmp = szFormated[i];
		pTmp++;
		*pTmp = 0;
		text = true;
	}

	if (tmp.Y - RICH_SIZE > this->H)
	{
		this->pScroll->Show();
		this->pScroll->Enable(true);
	}
	else
	{
		this->pScroll->Hide();
	}
	Sync = false;
}

void CRichBox::Draw(float fElapsed)
{
	this->Max = this->H / RICH_SIZE;
	int abx, aby;
	this->AbsolutePos(abx, aby);

	RECT bg = { 0,0,this->W,this->H };
	this->eng->Sprite()->Draw(lpWhite,
		&bg,
		nullptr,
		&D3DXVECTOR3(abx, aby, 0),
		D3DCOLOR_ARGB(151,0,0,0)
		);

	aby -= this->Index*RICH_SIZE;
	this->eng->Sprite()->Flush();

	while (Sync) Sleep(1);

	this->eng->Sprite()->Flush();
	Sync = true;
	for (std::list<_Format>::iterator it = this->vLines.begin(); it != this->vLines.end(); it++)
	{
		if (this->Index*RICH_SIZE > it->Y) continue;
		if (it->Y + RICH_SIZE - this->Index*RICH_SIZE > this->H) break;
		this->eng->TextManager->Draw(it->szText, RICH_SIZE, abx+it->X, aby+it->Y, CTXT_NOCLIP, CTXT_NOCLIP, it->dwColor, it->Bold, it->Italic);
	}
	Sync = false;

	if(this->pScroll->Visible()) this->pScroll->Draw(fElapsed);
}

bool CRichBox::Protocol(UINT msg, WPARAM wParam, LPARAM lParam)
{
	int ov = (this->rowcount - this->Max)-1;
	int ScrollPerElement = (ov)?this->H / ov :0;
	switch (msg)
	{
	case WM_VSCROLL:
		{
			int Pos = HIWORD(wParam);

			int dis;
			int Proximity = 0xffff;
			for (int i = 0; i < ov; i++)
			{
				dis = abs(i*ScrollPerElement - Pos);
				if (Proximity > dis)
				{
					Proximity = dis;
					this->Index = i;
				}
			}
			if (LOWORD(wParam) == SB_THUMBPOSITION)
			{
				this->pScroll->SetPos(-1, this->Index*ScrollPerElement);
			}
		}
		return true;
	case WM_MOUSEWHEEL:
		{
			short zDelta = -GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
			this->Index += zDelta;
			this->Index = max(min(this->Index, ov), 0);
			this->pScroll->SetPos(-1, this->Index*ScrollPerElement);
		}		
		return true;
	}

	return this->pScroll->Protocol(msg, wParam, lParam);
}
