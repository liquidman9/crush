/*
 * SRCollison.h
 */

#ifndef SRCOLLISION_H_INCLUDED
#define SRCOLLISION_H_INCLUDED

// Project includes
#include <server/game/Collision.h>

class SRCollision  : public Collision {
public:
	CollisionGEvent * resolve();

	SRCollision();
	SRCollision(ServerEntity * a, ServerEntity *b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction);
};

#endif