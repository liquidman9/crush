/*
 * C_Mothership.h
 */

#ifndef C_MOTHERSHIP_H_INCLUDED
#define C_MOTHERSHIP_H_INCLUDED

// Project includes
#include <shared/game/Mothership.h>
#include <client/graphics/entities/C_Entity.h>

#pragma warning( push )
#pragma warning( disable : 4250 )

class C_Mothership : public Mothership, public C_Entity {
public:
	//Mesh* m_pMesh;

	C_Mothership();
	C_Mothership(Entity *);
	//void draw();
};

#pragma warning( pop )


#endif