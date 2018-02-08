#include "stdafx.h"
#include "EngineObject.h"

void EngineObject::Deattach(EngineObject * child)
{
	for (list<EngineObject*>::iterator it = this->childs.begin();
		it != this->childs.end(); it++)
	{
		if (*it != child)
			continue;

		this->childs.erase(it);
		return;
	}
}

void EngineObject::ResetTransform()
{
	D3DXMatrixIdentity(&this->m_Transform);
}

void EngineObject::Translate(float x, float y, float z)
{
	D3DXMATRIX mOut;
	D3DXMatrixTranslation(&mOut, x, y, z);

	this->m_Transform *= mOut;
}

void EngineObject::Rotate(int axis, float angle)
{
	D3DXMATRIX mOut;
	D3DXMatrixIdentity(&mOut);

	switch (axis)
	{
	case 0:
		D3DXMatrixRotationX(&mOut, angle);
		break;
	case 1:
		D3DXMatrixRotationY(&mOut, angle);
		break;
	case 2:
		D3DXMatrixRotationZ(&mOut, angle);
		break;
	}

	this->m_Transform *= mOut;
}
