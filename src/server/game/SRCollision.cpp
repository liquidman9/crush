/*
 * SRCollision.cpp
 */

// Project includes
#include <server/game/SRCollision.h>

SRCollision::SRCollision() {
	Collision();
}

SRCollision::SRCollision(ServerEntity * a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction) :
	Collision(SR, a, b, closeA, closeB, elasticity, friction)
{}

CollisionGEvent * SRCollision::resolve() 
{
	S_Ship * ship = (S_Ship *)m_a;
	S_Resource * res = (S_Resource *)m_b;
	bool gathered = ship->interact(res);

	if(gathered)
		return new CollisionGEvent(m_a->m_id, m_b->m_id, m_poi, 0.0, SR);

	else
		return Collision::resolve();
}
