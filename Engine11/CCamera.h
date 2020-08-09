#pragma once
#include "CEObject.h"
class CCamera : public CEObject
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Up;
	DirectX::XMFLOAT3 Look;
public:


	// Heredado vía CEObject
	virtual void OnUpdate(double elapsed) override;

	virtual void OnFrame() override;


	// Heredado vía CEObject
	virtual void OnInitialize() override;

	virtual void OnDispose() override;

	friend class CEngine;
	friend class CRender;
};

