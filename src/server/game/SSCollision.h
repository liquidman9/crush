/*
 * SSCollison.h
 */

#ifndef SSCOLLISION_H_INCLUDED
#define SSCOLLISION_H_INCLUDED

// Project includes
#include <server/game/Collision.h>

class SSCollision  : public Collision {
public:
	CollisionGEvent * resolve();

	SSCollision();
	SSCollision(ServerEntity * a, ServerEntity *b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction);
};

#endif