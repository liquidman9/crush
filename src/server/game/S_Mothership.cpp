/*
 * S_Mothership.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Mothership.h>


S_Mothership::S_Mothership(int pNum) :
	Entity(MOTHERSHIP),
	Mothership(pNum),
	ServerEntity(D3DXVECTOR3(0,0,0), 0, 1000)
{

}

S_Mothership::S_Mothership(D3DXVECTOR3 pos, Quaternion orientation, int pNum) :
	Entity(genId(), MOTHERSHIP, pos, orientation),
	Mothership(pNum),
	ServerEntity(D3DXVECTOR3(0,0,0), 0, 1000)
{	

}


	
D3DXVECTOR3 S_Mothership::calculateRotationalInertia(float mass){
	float radius_squared = 50;
	float height_squared = 50;
	return D3DXVECTOR3( (1.0f / 12.0f) * mass * (3 * radius_squared + height_squared),
						(0.5f) * mass * radius_squared,
						(1.0f / 12.0f) * mass * (3 * radius_squared + height_squared));
};

