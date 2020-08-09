#pragma once
DirectX::XMFLOAT3 XMFloat3Substract(const DirectX::XMFLOAT3& first, const DirectX::XMFLOAT3& second)
{
	return {
		first.x - second.x,
		first.y - second.y,
		first.z - second.z,
	};
}

double XMFloat3Length(const DirectX::XMFLOAT3& first)
{
	return first.x * first.x + first.y * first.y + first.z * first.z;
}