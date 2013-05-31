/*
 * MSCollision.cpp
 */

// Project includes
#include <server/game/MSCollision.h>

MSCollision::MSCollision() {
	Collision();
}

MSCollision::MSCollision(ServerEntity * a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction) :
	Collision(MS, a, b, closeA, closeB, elasticity, friction)
{}

CollisionGEvent * MSCollision::resolve() 
{
	S_Mothership * mothership = (S_Mothership *)m_a;
	S_Ship * ship = (S_Ship *) m_b;
	mothership->interact(ship);

	return Collision::resolve();
}
