#include "stdafx.h"
#include "DAEngine.h"

Camera::Camera(DAEngine*link):EngineObject(link)
{
	this->Position = D3DXVECTOR3(0, 0, 0);
	this->LookAt = D3DXVECTOR3(100, 0, 0);
	this->Up = D3DXVECTOR3(0, 0, 1);
}

HRESULT Camera::Render()
{
	D3DXMATRIX mOut;
	D3DXMatrixLookAt(
		&mOut,
		&this->Position,
		&(this->Position + this->LookAt),
		&this->Up
		);

	return this->engine->View(&mOut);
}

void Camera::Move(D3DXVECTOR3 & dist)
{
	this->Position += dist;
}

void Camera::MoveTo(D3DXVECTOR3 & dist)
{
	this->Position = dist;
}

void Camera::LookDir(D3DXVECTOR3 & dist)
{
	this->LookAt = dist;
}

void Camera::RotateX(double fAngle)
{
	D3DXMATRIX mOut;
	D3DXVECTOR4 vOut;
	D3DXMatrixRotationX(&mOut, fAngle);
	D3DXVec3Transform(&vOut, &this->LookAt, &mOut);
	this->LookAt = D3DXVECTOR3(vOut.x, vOut.y, vOut.z);
}

void Camera::RotateY(double fAngle)
{
	D3DXMATRIX mOut;
	D3DXVECTOR4 vOut;
	D3DXMatrixRotationY(&mOut, fAngle);
	D3DXVec3Transform(&vOut, &this->LookAt, &mOut);
	this->LookAt = D3DXVECTOR3(vOut.x, vOut.y, vOut.z);
}

void Camera::RotateZ(double fAngle)
{
	D3DXMATRIX mOut;
	D3DXVECTOR4 vOut;
	D3DXMatrixRotationZ(&mOut, fAngle);
	D3DXVec3Transform(&vOut, &this->LookAt, &mOut);
	this->LookAt = D3DXVECTOR3(vOut.x, vOut.y, vOut.z);
}
