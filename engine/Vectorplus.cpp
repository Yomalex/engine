#include "stdafx.h"
#include "Vectorplus.h"


Vectorplus::Vectorplus() : Value(0,0,0,1)
{
	this->VectorLevel = 0;
}


Vectorplus::~Vectorplus()
{
}

Vectorplus::Vectorplus(D3DXVECTOR2 * v) : Value(0, 0, 0, 1)
{
	//ZeroMemory(&this->Value, sizeof(D3DXVECTOR4));
	//Vectorplus::Vectorplus();
	memcpy(&this->Value, v, sizeof(D3DXVECTOR2));
	this->Value.w = 1;
	this->VectorLevel = 2;
}

Vectorplus::Vectorplus(D3DXVECTOR3 * v) : Value(0, 0, 0, 1)
{
	memcpy(&this->Value, v, sizeof(D3DXVECTOR3));
	this->Value.w = 1;
	this->VectorLevel = 3;
}

Vectorplus::Vectorplus(D3DXVECTOR4 * v)
{
	memcpy(&this->Value, v, sizeof(D3DXVECTOR4));
	this->VectorLevel = 4;
}

float Vectorplus::dot(Vectorplus * a, Vectorplus * b)
{
	switch (min(a->VectorLevel, b->VectorLevel))
	{
	case 2: return D3DXVec2Dot(&a->V2(), &b->V2());
	case 3: return D3DXVec3Dot(&a->V3(), &b->V3());
	case 4: return D3DXVec4Dot(&a->V4(), &b->V4());
	}
	return 0;
}

Vectorplus Vectorplus::cross(Vectorplus * a, Vectorplus * b)
{
	D3DXVECTOR3 Cross;
	return Vectorplus(D3DXVec3Cross(&Cross, &a->V3(), &b->V3()));
}

Vectorplus *Vectorplus::Normalize()
{
	D3DXVECTOR3 v3 = this->V3();
	D3DXVec3Normalize(&v3, &v3);
	memcpy(&this->Value, &v3, sizeof(D3DXVECTOR3));
	this->Value.w = 1;
	return this;
}

Vectorplus *Vectorplus::Homogenize()
{
	this->Value = (this->Value / this->Value.w);
	return this;
}

D3DXVECTOR2 Vectorplus::V2()
{
	return D3DXVECTOR2(this->Value.x, this->Value.y);
}

D3DXVECTOR3 Vectorplus::V3()
{
	return D3DXVECTOR3(this->Value.x, this->Value.y, this->Value.z);
}

D3DXVECTOR4 Vectorplus::V4()
{
	return this->Value;
}

Vectorplus& Vectorplus::operator()(int n)
{
	switch (n)
	{
	case x: return Vectorplus(&D3DXVECTOR3(this->Value.x, 0, 0));
	case y: return Vectorplus(&D3DXVECTOR3( 0, this->Value.y,0));
	case z: return Vectorplus(&D3DXVECTOR3(0, 0, this->Value.z));
	case w: return Vectorplus(&D3DXVECTOR4(0, 0, 0, this->Value.w));
	case xy: return Vectorplus(&D3DXVECTOR3(this->Value.x, this->Value.y, 0));
	case xyz: return Vectorplus(&D3DXVECTOR3(this->Value.x, this->Value.y, this->Value.z));
	case xyzw: return *this;
	default: return Vectorplus();
	}
}

Vectorplus & Vectorplus::operator=(const Vectorplus & rhs)
{
	this->Value = rhs.Value;
	return *this;
}

Vectorplus & Vectorplus::operator*=(float Scalar)
{
	float space = this->Value.w;
	this->Value *= Scalar;
	this->Value.w = space;
	return *this;
}
