/*
 * SACollision.cpp
 */

// Project includes
#include <server/game/SACollision.h>

SACollision::SACollision() {
	Collision();
}

SACollision::SACollision(ServerEntity * a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction) :
	Collision(SA, a, b, closeA, closeB, elasticity, friction)
{}

CollisionGEvent * SACollision::resolve() 
{
	S_Asteroid * asteroid = (S_Asteroid *) m_b;
	S_Ship * ship = (S_Ship *) m_a;
	if(ship->interact(asteroid))
		return Collision::resolve();

	else
		return new CollisionGEvent(m_a->m_id, m_b->m_id, m_poi, 0.0, SA, ship->m_playerNum, -1);
}
