/*
 * SECollision.cpp
 */

// Project includes
#include <server/game/SECollision.h>

SECollision::SECollision() {
	Collision();
}

SECollision::SECollision(ServerEntity * a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction) :
	Collision(SE, a, b, closeA, closeB, elasticity, friction)
{}

CollisionGEvent * SECollision::resolve() 
{
	return Collision::resolve();
}
