/*
 * C_Asteroid.h
 */

#ifndef C_ASTEROID_H_INCLUDED
#define C_ASTEROID_H_INCLUDED

// Project includes
#include <shared/game/Asteroid.h>
#include <client/graphics/entities/C_Entity.h>

#pragma warning( push )
#pragma warning( disable : 4250 )

class C_Asteroid : public Asteroid, public C_Entity {
public:
	//Mesh* m_pMesh;

	C_Asteroid();
	C_Asteroid(Entity *);
	//void draw();
	void updateWorldMat();
};

#pragma warning( pop )


#endif