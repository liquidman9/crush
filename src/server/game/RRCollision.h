/*
 * RRCollison.h
 */

#ifndef RRCOLLISION_H_INCLUDED
#define RRCOLLISION_H_INCLUDED

// Project includes
#include <server/game/Collision.h>

class RRCollision  : public Collision {
public:
	CollisionGEvent * resolve();

	RRCollision();
	RRCollision(ServerEntity * a, ServerEntity *b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction);
};

#endif