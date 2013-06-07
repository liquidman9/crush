/*
 * RRCollision.cpp
 */

// Project includes
#include <server/game/RRCollision.h>

RRCollision::RRCollision() {
	Collision();
}

RRCollision::RRCollision(ServerEntity * a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction) :
	Collision(RR, a, b, closeA, closeB, elasticity, friction)
{}

CollisionGEvent * RRCollision::resolve() 
{
	S_Resource * res1 = (S_Resource *)m_a;
	S_Resource * res2 = (S_Resource *)m_b;

	if(res1->m_carrier != NULL && res2->m_carrier != NULL)
		return Collision::resolve();

	else
		return new CollisionGEvent(m_a->m_id, m_b->m_id, m_poi, 0.0, RR, -1, -1);
	// resources can be placed on top of it each other when on the mothership
	// or could give them the immovable tag (relative to their carrier) when on the mothership and while being held
		
}
