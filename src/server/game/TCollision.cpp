/*
 * TTCollision.cpp
 */

// Project includes
#include <server/game/TCollision.h>

TCollision::TCollision() {
	Collision();
}

TCollision::TCollision(ServerEntity * a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction) :
	Collision(SR, a, b, closeA, closeB, elasticity, friction)
{}

CollisionGEvent * TCollision::resolve() 
{
	S_TractorBeam * beam = (S_TractorBeam *)m_a;
	ServerEntity * entity = m_b;

	beam->interact(entity); //lock on check 
	return new CollisionGEvent(m_a->m_id, m_b->m_id, m_poi, 0.0, T, -1, -1); // could give them the immovable tag 
}
