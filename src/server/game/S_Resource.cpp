/*
 * S_Resource.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Resource.h>

S_Resource::S_Resource() :
	Entity(RESOURCE),
	Resource(),
	ServerEntity(100, 1, 10, calculateRotationalInertia(10))
{

}

S_Resource::S_Resource(D3DXVECTOR3 pos, Quaternion orientation) :
	Entity(genId(), RESOURCE, pos, orientation),
	Resource(),
	ServerEntity(100, 1, 10, calculateRotationalInertia(10))
{	

}



D3DXVECTOR3 S_Resource::calculateRotationalInertia(float mass){
	float radius_squared = 1;
	float height_squared = 1;
	return D3DXVECTOR3( (1.0f / 12.0f) * mass * (3 * radius_squared + height_squared),
						(0.5f) * mass * radius_squared,
						(1.0f / 12.0f) * mass * (3 * radius_squared + height_squared));
};

