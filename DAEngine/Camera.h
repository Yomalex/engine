#pragma once

class Camera : public EngineObject
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 LookAt;
	D3DXVECTOR3 Up;
public:
	Camera(DAEngine*link);
	~Camera() {}
	HRESULT Load() { return E_NOTIMPL; };
	virtual HRESULT Render();
	virtual HRESULT Release() { return E_NOTIMPL; };

	void Move(D3DXVECTOR3& dist);
	void Move(double x, double y, double z) { this->Move(D3DXVECTOR3(x, y, z)); }
	void MoveTo(D3DXVECTOR3& dist);
	void MoveTo(double x, double y, double z) { this->MoveTo(D3DXVECTOR3(x, y, z)); }
	void LookDir(D3DXVECTOR3& dist);
	void LookDir(double x, double y, double z) { this->LookDir(D3DXVECTOR3(x, y, z)); }
	void RotateX(double fAngle);
	void RotateY(double fAngle);
	void RotateZ(double fAngle);
};