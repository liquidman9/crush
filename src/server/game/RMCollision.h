/*
 * RMCollison.h
 */

#ifndef RMCOLLISION_H_INCLUDED
#define RMCOLLISION_H_INCLUDED

// Project includes
#include <server/game/Collision.h>

class RMCollision  : public Collision {
public:
	CollisionGEvent * resolve();

	RMCollision();
	RMCollision(ServerEntity * a, ServerEntity *b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction);
};

#endif