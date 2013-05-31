/*
 * Collison.h
 */

#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

// Project includes
#include <server/game/ServerEntity.h>
#include <shared/CollisionGEvent.h>
#include <server/game/S_Ship.h>

class Collision {
public:
	CType m_type;
	ServerEntity * m_a, * m_b;
	D3DXVECTOR3 m_closeA, m_closeB, m_poi, m_collision_normal;
	float m_elasticity, m_friction;

	static Collision * generateCollision(ServerEntity * a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB);
	CollisionGEvent * resolve();

	Collision();
	Collision(CType type, ServerEntity * a, ServerEntity *b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB,  float elasticity, float friction);
};

#endif