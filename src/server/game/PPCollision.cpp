/*
 * PPCollision.cpp
 */

// Project includes
#include <server/game/PPCollision.h>

PPCollision::PPCollision() {
	Collision();
}

PPCollision::PPCollision(ServerEntity * a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction) :
	Collision(PP, a, b, closeA, closeB, elasticity, friction)
{}

CollisionGEvent * PPCollision::resolve() 
{
	return new CollisionGEvent(m_a->m_id, m_b->m_id, m_poi, 0.0, PP, -1, -1);
}
