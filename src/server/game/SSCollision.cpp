/*
 * SSCollision.cpp
 */

// Project includes
#include <server/game/SSCollision.h>

SSCollision::SSCollision() {
	Collision();
}

SSCollision::SSCollision(ServerEntity * a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction) :
	Collision(SS, a, b, closeA, closeB, elasticity, friction)
{}

CollisionGEvent * SSCollision::resolve() 
{
	S_Ship * ship1 = (S_Ship *) m_a;
	S_Ship * ship2 = (S_Ship *) m_b;
	if(ship1->interact(ship2))
		return Collision::resolve();

	else
		return new CollisionGEvent(m_a->m_id, m_b->m_id, m_poi, 0.0, SS);		
}
