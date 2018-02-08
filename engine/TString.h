#pragma once

class CTString
{
public:
	CTString();
	CTString(const char * String);
	CTString(const wchar_t * String);
	~CTString();

	unsigned int Length();

	CTString & operator =(const char * right);
	CTString & operator =(const wchar_t * right);
	CTString & operator =(const CTString & right);
	/*CTString & operator +(const char * right);
	CTString & operator +(const wchar_t * right);
	CTString & operator +(const CTString & right);*/

	//Casting
	operator const char*() const;
	operator const wchar_t*() const;
private:

	char * BufferA;
	wchar_t * BufferW;
};

