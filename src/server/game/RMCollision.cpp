/*
 * RMCollision.cpp
 */

// Project includes
#include <server/game/RMCollision.h>

RMCollision::RMCollision() {
	Collision();
}

RMCollision::RMCollision(ServerEntity * a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction) :
	Collision(RM, a, b, closeA, closeB, elasticity, friction)
{}

CollisionGEvent * RMCollision::resolve() 
{
	S_Mothership * mothership = (S_Mothership *) m_b;
	S_Resource * res = (S_Resource *) m_a;
	if(res->m_carrier == NULL)
		mothership->getResource(res);
	return new CollisionGEvent(m_a->m_id, m_b->m_id, m_poi, 0.0, RM);	// change if possible to push a resource into the mothership with the tractorbeam
}
