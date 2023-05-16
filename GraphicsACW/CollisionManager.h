#pragma once
#include <directxmath.h>
#include <windows.h>
#include <d3d11.h>
using namespace DirectX;
class CollisionManager
{
public:
	CollisionManager();
	CollisionManager(XMFLOAT3 const& Position, float const& radius, int const& collisionTag, XMFLOAT3 const& newVelocity, XMFLOAT3 const& newRotation);
	~CollisionManager();
	bool UpdateCollsions(XMFLOAT3 const& dflyPosition, float const& dflyRadius)const;
	const XMFLOAT3& getVelocity()const;
	const XMFLOAT3& getRotation()const;
	int getTag()const;

private:
	int collsionTagNumber;
	float collsionRadius;
	XMFLOAT3 collisonPosition;
	XMFLOAT3 velocity;
	XMFLOAT3 rotation;
};
