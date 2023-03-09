#include "CollisionManager.h"



CollisionManager::CollisionManager() : collsionTagNumber(0), collsionRadius(0)
{
}

CollisionManager::CollisionManager(XMFLOAT3 const &Position, float const  &radius, int const  &collisionTag, XMFLOAT3 const &newVelocity, XMFLOAT3 const &newRotation) :
	collsionTagNumber(collisionTag), collsionRadius(radius), collisonPosition(Position), velocity(newVelocity), rotation(newRotation)
{
}


CollisionManager::~CollisionManager()
{
}

bool CollisionManager::UpdateCollsions(XMFLOAT3 const &dflyPosition, float const &dflyRadius) const
{
	const float a = collisonPosition.x - dflyPosition.x;
	const float b = collisonPosition.y - dflyPosition.y;
	const float c = collisonPosition.z - dflyPosition.z;

    const float d = sqrt(a * a) + (b * b) + (c * c);

	const float totalRadius = collsionRadius + dflyRadius;

	if (d < totalRadius)
	{
		return true;
	}
	return false;
}

const XMFLOAT3& CollisionManager::getVelocity()const
{
	return velocity;
}

const XMFLOAT3& CollisionManager::getRotation()const
{
	return rotation;
}

int CollisionManager::getTag()const
{
	return collsionTagNumber;
}
