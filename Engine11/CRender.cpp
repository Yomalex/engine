#include "pch.h"
#include "CEngine.h"
#include "CRender.h"

void CRender::OnInitialize()
{
	for (auto it : this->_childs)
		it->OnInitialize();
}

void CRender::OnUpdate(double elapsed)
{
	for (auto it : this->_childs)
		it->OnUpdate(elapsed);
}

void CRender::OnFrame()
{
	static UINT stride = sizeof(VertexPNT);
	static UINT offset = 0;

	if (this->_vertexBuffer.get() != nullptr)
	{
		GameEngine->_context->IASetVertexBuffers(0, 1, this->_vertexBuffer.GetAddressOf(), &stride, &offset);
		
		// set the primitive topology
		GameEngine->_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// draw 3 vertices, starting from vertex 0
		GameEngine->_context->Draw(this->_meshVertexCount, 0);
	}

	for (auto it : this->_childs)
		it->OnFrame();
}

void CRender::OnDispose()
{
	for (auto it : this->_childs)
		it->OnDispose();
}

void CRender::Show()
{
	if(_parent == nullptr)
		GameEngine->_renderList.push_back(this);
	else
		_parent->_childs.push_back(this);
}

void CRender::Hide()
{
	if (_parent == nullptr)
		GameEngine->_renderList.remove(this);
	else
		_parent->_childs.remove(this);
}

void CRender::Set2D(bool flag)
{
	this->is2D = flag;
}

void CRender::SetParent(CRender* parent)
{
	this->_parent = parent;
}

void CRender::LoadVertex(VertexPNT* vertex, int length)
{
	// Fill in a buffer description.
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VertexPNT) * length;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertex;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	CResult hr = GameEngine->_device->CreateBuffer(&bufferDesc, &InitData, _vertexBuffer.put());
}

void CRender::LoadIndex(UINT* index, int length)
{
	// Fill in a buffer description.
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(unsigned int) * length;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	
	// Create the vertex buffer.
	CResult hr = GameEngine->_device->CreateBuffer(&bufferDesc, &InitData, _indexBuffer.put());
}
