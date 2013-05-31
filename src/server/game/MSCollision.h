/*
 * MSCollison.h
 */

#ifndef MSCOLLISION_H_INCLUDED
#define MSCOLLISION_H_INCLUDED

// Project includes
#include <server/game/Collision.h>

class MSCollision  : public Collision {
public:
	CollisionGEvent * resolve();

	MSCollision();
	MSCollision(ServerEntity * a, ServerEntity *b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction);
};

#endif