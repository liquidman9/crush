/*
 * C_Resource.h
 */

#ifndef C_RESOURCE_H_INCLUDED
#define C_RESOURCE_H_INCLUDED

// Project includes
#include <shared/game/Resource.h>
#include <client/graphics/entities/C_Entity.h>
#include <client/graphics/Renderable.h>

#pragma warning( push )
#pragma warning( disable : 4250 )

class C_Resource : public Resource, public C_Entity {
public:
	//Mesh* m_pMesh;

	C_Resource();
	C_Resource(Entity *);
	void draw();
};

#pragma warning( pop )


#endif