/*
 * SACollison.h
 */

#ifndef SACOLLISION_H_INCLUDED
#define SACOLLISION_H_INCLUDED

// Project includes
#include <server/game/Collision.h>

class SACollision  : public Collision {
public:
	CollisionGEvent * resolve();

	SACollision();
	SACollision(ServerEntity * a, ServerEntity *b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction);
};

#endif