/*
 * PPCollison.h
 */

#ifndef PPCOLLISION_H_INCLUDED
#define PPCOLLISION_H_INCLUDED

// Project includes
#include <server/game/Collision.h>

class PPCollision  : public Collision {
public:
	CollisionGEvent * resolve();

	PPCollision();
	PPCollision(ServerEntity * a, ServerEntity *b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction);
};

#endif