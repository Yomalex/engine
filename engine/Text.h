#pragma once

#define CTXT_NOCLIP -1
#define CTXT_MAXFONTS 100
#define CTXT_FONT TEXT("")

class CText
{
	struct _tagFonts
	{
		D3DXFONT_DESC Descripcion;
		LPD3DXFONT Font;
	};
public:
	CText(CEngine * eng);
	~CText();

	void Draw(const TCHAR * szString, int size, int x, int y);
	void Draw(const TCHAR * szString, int size, int x, int y, int w, int h);
	void Draw(const TCHAR * szString, int size, int x, int y, int w, int h, DWORD dwColor);
	void Draw(const TCHAR * szString, int size, int x, int y, int w, int h, DWORD dwColor, BOOL Bold, BOOL Italic);

	void Calc(const TCHAR * szString, int size, int *w, int *h, BOOL Bold, BOOL Italic);

	void SetStyle(BOOL Bold=FALSE, BOOL Italic=FALSE);
	void JustifyLeft() { this->dwFormat = (this->dwFormat&DT_VCENTER) | DT_LEFT; }
	void JustifyCenter() { this->dwFormat = (this->dwFormat&DT_VCENTER) | DT_CENTER; }
	void JustifyRight() { this->dwFormat = (this->dwFormat&DT_VCENTER) | DT_RIGHT; }
	void VerticalCentered(BOOL v) { this->dwFormat &= ~DT_VCENTER; this->dwFormat |= (v) ? DT_VCENTER : 0; }
	void SetColor(DWORD dwColor);

	void OnLostDevice();
	void OnResetDevice();
private:
	LPD3DXFONT Find();

	CEngine * Engine;
	D3DCOLOR Color;
	DWORD dwFormat;
	D3DXFONT_DESC Descripcion;

	_tagFonts * vFonts[CTXT_MAXFONTS];
};

