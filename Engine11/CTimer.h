#pragma once
class CTimer
{
	LARGE_INTEGER Frec, Counter, End;
	double freq;
public:
	CTimer();

	void Start();

	/// Return Value in Seconds
	double Stop();
};

