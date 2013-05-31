/*
 * SECollison.h
 */

#ifndef SECOLLISION_H_INCLUDED
#define SECOLLISION_H_INCLUDED

// Project includes
#include <server/game/Collision.h>

class SECollision  : public Collision {
public:
	CollisionGEvent * resolve();

	SECollision();
	SECollision(ServerEntity * a, ServerEntity *b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction);
};

#endif