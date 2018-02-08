#pragma once
#include <d3d9.h>
#include <d3dx9.h>

enum VectorVar
{
	x = 1 << 0,
	y = 1 << 1,
	z = 1 << 2,
	w = 1 << 3,
	xy = x|y,
	xz = x|z,
	xw = x|w,
	yz = y|z,
	yw = y|w,
	zw = z|w,
	xyz = x|y|z,
	yzw = y|z|w,
	xyzw = x|y|z|w,
};
class Vectorplus
{
//protected:
	D3DXVECTOR4 Value;
	int VectorLevel;
public:
	Vectorplus();
	~Vectorplus();

	Vectorplus(D3DXVECTOR2 * v);
	Vectorplus(D3DXVECTOR3 * v);
	Vectorplus(D3DXVECTOR4 * v);

	static float dot(Vectorplus * a, Vectorplus * b);
	static Vectorplus cross(Vectorplus * a, Vectorplus * b);
	Vectorplus *Normalize();
	Vectorplus *Homogenize();

	D3DXVECTOR2 V2();
	D3DXVECTOR3 V3();
	D3DXVECTOR4 V4();
public:
	//Operators
	Vectorplus& operator ()(int n);
	Vectorplus& operator =(const Vectorplus& rhs);
	Vectorplus& operator *=(float Scalar);
};
