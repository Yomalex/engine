#pragma once
#include "pch.h"
class CResult
{
	HRESULT hResult;
public:
	CResult();
	CResult(HRESULT hr);

	bool Result();
	std::string GetMessage();

	CResult operator =(const HRESULT hr);
};

