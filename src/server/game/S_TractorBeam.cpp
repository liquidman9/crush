/*
 * S_TractorBeam.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_TractorBeam.h>


S_TractorBeam::S_TractorBeam(int pNum) :
	Entity(TRACTORBEAM),
	TractorBeam(pNum),
	ServerEntity(1000, D3DXVECTOR3(1, 1, 1))
{

}

	
S_TractorBeam::S_TractorBeam(D3DXVECTOR3 pos, Quaternion orientation, int pNum) :
	Entity(genId(), TRACTORBEAM, pos, orientation),
	TractorBeam(pNum),
	ServerEntity(1000, D3DXVECTOR3(1, 1, 1))
{	

}



D3DXVECTOR3 S_TractorBeam::calculateRotationalInertia(float mass){
	float radius_squared = 50;
	float height_squared = 50;
	return D3DXVECTOR3( (1.0f / 12.0f) * mass * (3 * radius_squared + height_squared),
						(0.5f) * mass * radius_squared,
						(1.0f / 12.0f) * mass * (3 * radius_squared + height_squared));
};

