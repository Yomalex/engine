#pragma once

struct HRTimer
{
	LARGE_INTEGER Frec, Counter;
	double freq;
	HRTimer()
	{
		this->Reset();
	}
	void Reset()
	{
		QueryPerformanceFrequency(&Frec);
		freq = double(Frec.QuadPart) / 1000.0;
		QueryPerformanceCounter(&Counter);
	}
	double Consumed()
	{
		LARGE_INTEGER End;
		QueryPerformanceCounter(&End);
		return double(End.QuadPart - Counter.QuadPart) / freq / 1000.0;
	}
};