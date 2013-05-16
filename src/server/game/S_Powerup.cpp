/*
 * S_Powerup.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Powerup.h>



S_Powerup::S_Powerup(D3DXVECTOR3 pos, Quaternion orientation) :
	Entity(genId(), POWERUP, pos, orientation),
	Powerup(),
	ServerEntity(10, 1.0, calculateRotationalInertia(10))
{	
}

D3DXMATRIX S_Powerup::calculateRotationalInertia(float mass){
	float radius_squared = 1;
	return *D3DXMatrixScaling(&D3DXMATRIX(), (2.0f / 5.0f) * mass * radius_squared,
											 (2.0f / 5.0f) * mass * radius_squared,
											 (2.0f / 5.0f) * mass * radius_squared);
};


void S_Powerup::update(float delta_time){
	
	ServerEntity::update(delta_time);

}