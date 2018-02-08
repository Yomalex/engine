#pragma once

enum STATUS
{
	CS_NOTLOGGED,
	CS_LOGGED,
	CS_PLAYING
};

void Warp(int Map, BYTE x, BYTE y);
void WarpGate(int Gate);

extern float X, Y;