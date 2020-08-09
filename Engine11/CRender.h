#pragma once
#include "CEObject.h"
#include "BufferUtil.h"

class CRender : public CEObject
{
protected:
	bool is2D;
	DirectX::XMFLOAT3 _position;
	CRender* _parent;
	std::list<CRender*> _childs;

	com_ptr<ID3D11Buffer> _vertexBuffer;
	com_ptr<ID3D11Buffer> _indexBuffer;
	com_ptr<ID3D11Texture2D> _meshTexture;

	UINT _meshVertexCount;
public:

	// Heredado vía CEObject
	virtual void OnInitialize() override;
	virtual void OnUpdate(double elapsed) override;
	virtual void OnFrame() override;
	virtual void OnDispose() override;
	void Show();
	void Hide();
	void Set2D(bool flag);
	void SetParent(CRender* parent);
	void LoadVertex(VertexPNT * vertex, int length);
	void LoadIndex(UINT* index, int length);
	//void LoadVertex(VertexPNC);

private:
	// Draw first far of camera
	static bool Sorter(const CRender*& first, const CRender*& second)
	{
		auto resA = XMFloat3Substract(first->_position, GameEngine->MainCamera.Position);
		auto resB = XMFloat3Substract(second->_position, GameEngine->MainCamera.Position);

		auto valA = XMFloat3Length(resA);
		auto valB = XMFloat3Length(resB);

		if (!first->is2D && second->is2D)
			return true;
		else if (first->is2D && !second->is2D)
			return false;

		return valA > valB;
	}

	friend class CEngine;
};

