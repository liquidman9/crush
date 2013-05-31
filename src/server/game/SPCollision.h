/*
 * SPCollison.h
 */

#ifndef SPCOLLISION_H_INCLUDED
#define SPCOLLISION_H_INCLUDED

// Project includes
#include <server/game/Collision.h>

class SPCollision  : public Collision {
public:
	CollisionGEvent * resolve();

	SPCollision();
	SPCollision(ServerEntity * a, ServerEntity *b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction);
};

#endif