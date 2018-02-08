#include "stdafx.h"
#include "Engine.h"


CText::CText(CEngine * eng)
{
	this->Engine = eng;
	this->Descripcion.CharSet = DEFAULT_CHARSET;
	this->Descripcion.OutputPrecision = /*OUT_TT_ONLY_PRECIS;*/ OUT_DEFAULT_PRECIS;
	this->Descripcion.Quality = ANTIALIASED_QUALITY;
	this->Descripcion.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	this->Descripcion.MipLevels = 1;
	this->Descripcion.Italic = FALSE;
	this->Descripcion.Width = 0;
	_tcscpy_s(this->Descripcion.FaceName, CTXT_FONT);
}

CText::~CText()
{
	for (UINT i = 0; i < CTXT_MAXFONTS; i++)
	{
		if (this->vFonts[i] == nullptr) return;
		//this->vFonts[i]->Font->OnLostDevice();
		SAFE_RELEASE(this->vFonts[i]->Font);
		delete this->vFonts[i];
	}
}

void CText::Draw(const TCHAR * szString, int size, int x, int y)
{
	this->Descripcion.Height = size;
	LPD3DXFONT output = this->Find();
	RECT rDraw = { x, y, x + 10, y + 10 };
	output->DrawText(nullptr, szString, CTXT_NOCLIP, &rDraw, this->dwFormat|DT_NOCLIP, this->Color);
}

void CText::Draw(const TCHAR * szString, int size, int x, int y, int w, int h)
{
	this->Descripcion.Height = size;
	LPD3DXFONT output = this->Find();
	RECT rDraw = {x,y,x+w,y+h};
	if (w == CTXT_NOCLIP || h == CTXT_NOCLIP)
	{
		RECT rcalc = { 0,0,0,0 };
		output->DrawText(nullptr, szString, CTXT_NOCLIP, &rcalc, DT_CALCRECT, this->Color);
		if (w == CTXT_NOCLIP) rDraw.right += rcalc.right - rcalc.left + 1;
		if (h == CTXT_NOCLIP) rDraw.bottom += rcalc.bottom - rcalc.top + 1;
	}

	output->DrawText(nullptr, szString, CTXT_NOCLIP, &rDraw, this->dwFormat, this->Color);
}

void CText::Draw(const TCHAR * szString, int size, int x, int y, int w, int h, DWORD dwColor)
{
	this->SetColor(dwColor);
	this->Draw(szString, size, x, y, w, h);
}

void CText::Draw(const TCHAR * szString, int size, int x, int y, int w, int h, DWORD dwColor, BOOL Bold, BOOL Italic)
{
	this->SetStyle(Bold, Italic);
	this->Draw(szString,size,x,y,w,h,dwColor);
}

void CText::Calc(const TCHAR * szString, int size, int * w, int * h, BOOL Bold, BOOL Italic)
{
	this->Descripcion.Height = size;
	this->SetStyle(Bold, Italic);
	LPD3DXFONT output = this->Find();
	RECT rcalc = { 0,0,0,0 };
	output->DrawText(nullptr, szString, CTXT_NOCLIP, &rcalc, DT_CALCRECT, this->Color);
	if (w != nullptr) *w = rcalc.right - rcalc.left;
	if (h != nullptr) *h = rcalc.bottom - rcalc.top;
}

void CText::SetStyle(BOOL Bold, BOOL Italic)
{
	this->Descripcion.Italic = Italic;
	this->Descripcion.Weight = (Bold) ? FW_BOLD : FW_NORMAL;
}

void CText::SetColor(DWORD dwColor)
{
	this->Color = dwColor;
}

void CText::OnLostDevice()
{
	for (UINT i = 0; i < CTXT_MAXFONTS; i++)
	{
		if (this->vFonts[i] == nullptr) return;
		this->vFonts[i]->Font->OnLostDevice();
	}
}

void CText::OnResetDevice()
{
	for (UINT i = 0; i < CTXT_MAXFONTS; i++)
	{
		if (this->vFonts[i] == nullptr) return;
		this->vFonts[i]->Font->OnResetDevice();
	}
}

LPD3DXFONT CText::Find()
{
	for (UINT i = 0; i < CTXT_MAXFONTS; i++)
	{
		if (this->vFonts[i] == nullptr) break;
		if (memcmp(&this->vFonts[i]->Descripcion, &this->Descripcion, sizeof(D3DXFONT_DESC)) == FALSE) return this->vFonts[i]->Font;
	}
	for (UINT i = 0; i < CTXT_MAXFONTS; i++)
	{
		if (this->vFonts[i] != nullptr)
			continue;

		this->vFonts[i] = new _tagFonts();
		memcpy(&this->vFonts[i]->Descripcion, &this->Descripcion, sizeof(D3DXFONT_DESC));
		D3DXCreateFontIndirect(this->Engine->GetDevice(), &this->vFonts[i]->Descripcion, &this->vFonts[i]->Font);
		return this->vFonts[i]->Font;
	}
	return nullptr;
}
