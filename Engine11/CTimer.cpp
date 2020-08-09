#include "pch.h"
#include "CTimer.h"

CTimer::CTimer()
{
	QueryPerformanceFrequency(&Frec);
	freq = double(Frec.QuadPart) / 1000.0;
}

void CTimer::Start()
{
	QueryPerformanceCounter(&Counter);
}

double CTimer::Stop()
{
	QueryPerformanceCounter(&End);
	return double(End.QuadPart - Counter.QuadPart) / freq / 1000.0;
}
