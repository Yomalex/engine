#include "stdafx.h"
#include "TString.h"
#include <stdlib.h>
#include <string>


CTString::CTString():BufferA(nullptr),BufferW(nullptr)
{
}

CTString::CTString(const char * String)
{
	this->BufferA = nullptr;
	this->BufferW = nullptr;
	this->operator=(String);
}

CTString::CTString(const wchar_t * String)
{
	this->BufferA = nullptr;
	this->BufferW = nullptr;
	this->operator=(String);
}


CTString::~CTString()
{
	free(this->BufferA);
	this->BufferA = nullptr;
	free(this->BufferW);
	this->BufferW = nullptr;
}

unsigned int CTString::Length()
{
	if (this->BufferA == nullptr) return 0;
	return strlen(this->BufferA);
}

CTString & CTString::operator=(const char * right)
{
	size_t converted;
	int len = strlen(right)+1;
	this->BufferA = (char*)realloc(this->BufferA, len);
	this->BufferW = (wchar_t*)realloc(this->BufferW, len*2);
	memcpy(this->BufferA, right, len);
	mbstowcs_s(&converted, this->BufferW, len, right, len);
	return *this;
}

CTString & CTString::operator=(const wchar_t * right)
{
	size_t converted;
	int len = wcslen(right) + 1;
	this->BufferA = (char*)realloc(this->BufferA, len);
	this->BufferW = (wchar_t*)realloc(this->BufferW, len * 2);
	memcpy(this->BufferW, right, len*2);
	wcstombs_s(&converted, this->BufferA, len, right, len);
	return *this;
}

CTString & CTString::operator=(const CTString & right)
{
	if (this == &right) return *this;

	int len = strlen(right.BufferA)+1;

	realloc(this->BufferA, len);
	realloc(this->BufferW, len * 2);
	memcpy(this->BufferA, right.BufferA, len + 1);
	memcpy(this->BufferW, right.BufferW, (len + 1)*2);
	return *this;
}

CTString::operator const char*() const
{
	return this->BufferA;
}

CTString::operator const wchar_t*() const
{
	return this->BufferW;
}
