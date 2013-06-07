/*
 * SPCollision.cpp
 */

// Project includes
#include <server/game/SPCollision.h>

SPCollision::SPCollision() {
	Collision();
}

SPCollision::SPCollision(ServerEntity * a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction) :
	Collision(SP, a, b, closeA, closeB, elasticity, friction)
{}

CollisionGEvent * SPCollision::resolve() 
{
	S_Powerup * power = (S_Powerup *)m_b;
	S_Ship * ship = (S_Ship *)m_a;
	if(ship->interact(power))
		return new CollisionGEvent(m_a->m_id, m_b->m_id, m_poi, 0.0, SP, ship->m_playerNum, -1);
	return NULL;
}
