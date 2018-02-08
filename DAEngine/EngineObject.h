#pragma once

class DAEngine;
#include <list>
#include <d3d9.h>
#include <d3dx9.h>
using namespace std;

class EngineObject
{
protected:
	bool m_loaded;
	bool m_locked;

	DAEngine * engine;
	EngineObject * parent;
	list<EngineObject*> childs;
	
	D3DXMATRIX m_Transform;
public:

	EngineObject(DAEngine * Link)
	{
		this->parent = nullptr;
		this->engine = Link;
	}

	~EngineObject()
	{
		for (auto &it : this->childs)
		{
			delete it;
		}

		this->childs.clear();
	}

	virtual HRESULT Load() = 0;
	virtual HRESULT Render() = 0;
	virtual HRESULT Release() = 0;

	virtual HRESULT OnCreateDevice() { return E_NOTIMPL; }
	virtual HRESULT OnResetDevice() { return E_NOTIMPL; }
	virtual HRESULT OnLostDevice() { return E_NOTIMPL; }

	virtual void WindowMessage(UINT Message, WPARAM wParam, LPARAM lParam) {};

	//void SetEngine(DAEngine * eng) { this->Engine = eng; }

	bool GetLock() { return this->m_locked; }
	void SetLock(bool value) { this->m_locked = value; }

	bool IsLoaded() { return this->m_loaded; }

	EngineObject * Attach(EngineObject* child) { 
		child->parent = this;  
		childs.push_back(child);
	}
	void Deattach(EngineObject* child);

	void ResetTransform();
	void Translate(float x, float y, float z);
	void Rotate(int axis, float angle);
};