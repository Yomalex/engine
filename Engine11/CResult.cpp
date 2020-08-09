#include "pch.h"
#include "CResult.h"
#include <comdef.h>

CResult::CResult()
{
	hResult = S_OK;
}

CResult::CResult(HRESULT hr)
{
	hResult = hr;
	if (!Result())
	{
		throw new std::exception(GetMessage().c_str(), (int)hr);
	}
}

bool CResult::Result()
{
	return hResult == S_OK;
}

std::string CResult::GetMessage()
{
    if (FACILITY_WINDOWS == HRESULT_FACILITY(hResult))
        hResult = HRESULT_CODE(hResult);
    char* szErrMsg;

    std::string str;

    if (FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, hResult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&szErrMsg, 0, NULL) != 0)
    {
        str = szErrMsg;
        LocalFree(szErrMsg);
    }
    else
        str = "[Could not find a description for error]";
	return str;
}

CResult CResult::operator=(const HRESULT hr)
{
	hResult = hr;
	if (!Result())
	{
		throw new std::exception(GetMessage().c_str(), (int)hr);
	}
	return *this;
}
