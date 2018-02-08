#pragma once

#include <vector>
#define MENU_ELEMENT_H 18
#define MAX_STRING 512
#define RICH_SIZE 14
#define RICH_OFFSET 5


struct _tagScrollBarInfo
{
	LPDIRECT3DTEXTURE9 Top;
	LPDIRECT3DTEXTURE9 Mid;
	LPDIRECT3DTEXTURE9 Bot;
	LPDIRECT3DTEXTURE9 ScrollOn;
	LPDIRECT3DTEXTURE9 ScrollOff;
};

class CWindow;
class CUI;

typedef void(*UIEvent)(CWindow*, LPARAM);
#define UIEVENT(x) void x(CWindow*caller,LPARAM lParam)

class CWindow
{
public:
	CWindow();
	CWindow(CEngine * Eng);
	CWindow(const TCHAR * szText, int x, int y, CWindow * Parent, LPDIRECT3DTEXTURE9 lpTexture);
	CWindow(const TCHAR * szText, int x, int y, CWindow * Parent);
	~CWindow();
	void Initialize(CEngine* Eng);
	void SetParam(LPARAM Param) { this->lParam = Param; }
	void SetTexture(LPDIRECT3DTEXTURE9 lpText);
	void SetText(const TCHAR * szText);
	void GetText(TCHAR * szReturned, int size);
	const TCHAR * GetText() { return this->szWindow; }
	void SetPos(int x, int y, int w = -1, int h = -1);
	virtual bool Protocol(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void Draw();
	void Show() { this->bShow = true; }
	void Hide() { this->bShow = false; }
	float AttachChild(CWindow*Child);
	void RemoveChild(CWindow*Child);
	void SetKeyboardFocus(CWindow*Child);
	CWindow* GetKeyboardFocus(void);
	void AbsolutePos(int &x, int &y);
	bool inBox(int x, int y);

	bool Visible() { return this->bShow; }
	int GetX() { return this->X; }
	int GetY() { return this->Y; }
	int GetWidth() { return this->W; }
	int GetHeight() { return this->H; }

	void SetOpacity(BYTE btOpacity) { this->Opacity = btOpacity; }
	void SetTextColor(BYTE r, BYTE g, BYTE b) { this->R = r; this->G = g; this->B = b; }

	void SetParent(CWindow * p);
protected:
	LPARAM lParam;
	int X, Y;
	int W, H;
	bool bShow;
	TCHAR szWindow[MAX_STRING];
	CWindow * Parent,* KBFocus;
	std::list<CWindow*> Childrens;
	LPDIRECT3DTEXTURE9 lpBackGround;

	RECT DrawRect;
	BYTE Opacity, R,G,B;

	CEngine * eng;
	float d;
};

class CButton : public CWindow
{
public:
	CButton(int x, int y, LPDIRECT3DTEXTURE9 tx, int states, CWindow * parent = nullptr);
	~CButton();

	bool Protocol(UINT msg, WPARAM wParam, LPARAM lParam);
	void Draw();
	void SetStates(int st);
	void Enable(bool enable) { this->Disable = !enable; }
	void SetFontSize(int s) { this->FontSize = s; }

	UIEvent OnClick;
	DWORD ID;
private:
	bool Hover, Down, Disable;
	int States;
	int FontSize;
	TCHAR FastKey;
};

class CStatic :public CWindow
{
public:
	CStatic(const TCHAR * szText, int x, int y, int w, int h, CWindow * parent = nullptr);
	~CStatic();
	bool Protocol(UINT msg, WPARAM wParam, LPARAM lParam);
	void Draw();

	void SetBackGround(D3DCOLOR Color) { this->dwBG = Color; }
private:
	DWORD dwBG;
};

class CProgressBar:public CWindow
{
public:
	CProgressBar(const TCHAR * szName, LPDIRECT3DTEXTURE9 lpTex, int x, int y, CWindow * parent=nullptr);
	CProgressBar(const TCHAR * szName, LPDIRECT3DTEXTURE9 lpTexEmpty, LPDIRECT3DTEXTURE9 lpTexFull, int x, int y, CWindow * parent = nullptr);
	~CProgressBar();

	bool Protocol(UINT msg, WPARAM wParam, LPARAM lParam);
	void Draw();
	void SetProgress(float p) { this->prog = p; }
	void SetMax(float m) { this->max = m; }
	void VerticalMode(bool enable) { this->Vertical = enable; }
private:
	bool TwoTextures;
	bool Vertical;
	float prog, max, oldvalue;
	LPDIRECT3DTEXTURE9 TexFull;
	CStatic * Tag;
};

class CImage :public CWindow
{
public:
	CImage(int x, int y, LPDIRECT3DTEXTURE9 lpTex, CWindow*Parent=nullptr);
	~CImage();

	bool Protocol(UINT msg, WPARAM wParam, LPARAM lParam);
	void Draw();
};

class CEdit :public CWindow
{
public:
	CEdit(int x, int y, int w, int h, CWindow*parent=nullptr);
	CEdit(int x, int y, LPDIRECT3DTEXTURE9 lpText, CWindow*parent = nullptr);
	~CEdit();

	bool Protocol(UINT msg, WPARAM wParam, LPARAM lParam);
	void Draw();
	void SetPassword(bool set) { this->bPassword = set; }
private:
	void ViewPort();
	void FindCaretpos(int x, int y);
	void UpdateCaret();

	int Cursor, CurrentView;
	int CaretX;
	bool bPassword;
	TCHAR szPassword[MAX_STRING];
	int iWidth[MAX_STRING];
	float last_caret;
};

class CUIModel : public CWindow
{
public:
	CUIModel(int x, int y, int w, int h, int bw, int bh, CEngineElement * Model, CWindow * parent = nullptr);
	~CUIModel();

	bool Protocol(UINT msg, WPARAM wParam, LPARAM lParam);
	void DrawOff();
	void SetBoundingBox(D3DXVECTOR3 bb[2]);
	void SetTransform(D3DXMATRIX m) { this->Transform = m; }
	void OnLostDevice();
	void OnResetDevice();
private:
	int BackBuffer[2];
	CEngineElement * mdl;
	D3DXMATRIX Transform;
	D3DXVECTOR3 Center;
	double zBack;
};

class CScroll : public CWindow
{
public:
	CScroll(int x, int y, int imax, _tagScrollBarInfo* Texture, CWindow * Parent, bool HScroll = false);
	~CScroll();

	bool Protocol(UINT msg, WPARAM wParam, LPARAM lParam);
	void Enable(bool IsEnable) { this->bEnable = IsEnable; }
	void Draw(float fElapsed);
private:
	bool bHScroll, bEnable;
	int OriginalPos, MaxPos;
	bool bDrag;
	int dx, dy;

	_tagScrollBarInfo *TextureInfo;
};

class CMenu : public CWindow
{
	struct _tagMenuElement
	{
		DWORD Value;
		CButton * bt;
	};
public:
	CMenu(int x, int y, int w, int h, CWindow * parent = nullptr);
	~CMenu();

	bool Protocol(UINT msg, WPARAM wParam, LPARAM lParam);
	void AddElement(const TCHAR * szText, DWORD dwValue);
	void GetElement(UINT element, TCHAR * szText, int imax, DWORD *dwValue);
	void GetElement(CWindow*element, TCHAR * szText, int imax, DWORD *dwValue);
	UINT Count() { return this->vMenu.size(); }
	UIEvent OnSelect;
private:
	std::vector<_tagMenuElement> vMenu;
	CScroll * MenuScroll;
};

class CJumpList : public CWindow
{
public:
	CJumpList(int x, int y, LPDIRECT3DTEXTURE9 Text, LPDIRECT3DTEXTURE9 Drop, int states, CWindow * parent = nullptr);
	~CJumpList();
	void AddElement(const TCHAR * szText, DWORD dwValue) { this->JumpMenu->AddElement(szText, dwValue); }
	void GetSelected(TCHAR * szText, int imax, DWORD *dwValue) { this->JumpMenu->GetElement(this->OptionSelected, szText, imax, dwValue); }
	bool Protocol(UINT msg, WPARAM wParam, LPARAM lParam);
	void Draw(float fElapsed);
	void SetSelected(UINT Element);

	UIEvent OnChange;
private:
	int OptionSelected;

	CMenu * JumpMenu;
	CImage * Info;
	CButton * DropList;
};

class CCheckBox : public CWindow
{
public:
	CCheckBox(int x, int y, LPDIRECT3DTEXTURE9 Texture, CWindow * parent = nullptr);
	CCheckBox(int x, int y, LPDIRECT3DTEXTURE9 TextureUn, LPDIRECT3DTEXTURE9 TextureCh, CWindow * parent = nullptr);
	~CCheckBox();

	void Draw(float fElapsed);
	bool Protocol(UINT msg, WPARAM wParam, LPARAM lParam);
	bool isChecked() { return this->Checked; }
	void Check(bool isChecked) { this->Checked = isChecked; }

	UIEvent OnChange;
private:
	bool TwoTextures;
	bool Checked;

	bool bHover, bDown;

	LPDIRECT3DTEXTURE9 Secundary;
};

class CRichBox : public CWindow
{
	struct _Format
	{
		int X, Y;
		DWORD dwColor;
		BOOL Bold;
		BOOL Italic;
		TCHAR * szText;
	};
public:
	CRichBox(int x, int y,int w, int h, CWindow * parent = nullptr);
	~CRichBox();
	void innerText(const TCHAR * szFormated);

	void Draw(float fElapsed);
	bool Protocol(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	CScroll * pScroll;
	std::list<_Format> vLines;
	int rowcount;
	int Index, Max;
	bool Sync;
};

class CUI
{
public:
	CUI(CEngine * eng);
	~CUI();
	void Clear();
	void Draw();
	void OffScreenDraw();
	CWindow * GetDesktop() { return this->Desktop; }
	bool WndProtocol(UINT msg, WPARAM wParam, LPARAM lParam);
	void OnLostDevice();
	void OnResetDevice();
	void BindKey(BYTE Key, UIEvent CallBack = nullptr);
	void Initialize();

private:
	CWindow * Desktop;
	CEngine * Eng;
	std::list<CUIModel*> vModels;
	UIEvent vBinds[256];
	//LPDIRECT3DTEXTURE9 lpWhite;
	friend CUIModel;
};

extern _tagScrollBarInfo g_ScrollInfo;