/*
 * TTCollison.h
 */

#ifndef TCOLLISION_H_INCLUDED
#define TCOLLISION_H_INCLUDED

// Project includes
#include <server/game/Collision.h>

class TCollision  : public Collision {
public:
	CollisionGEvent * resolve();

	TCollision();
	TCollision(ServerEntity * a, ServerEntity *b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction);
};

#endif