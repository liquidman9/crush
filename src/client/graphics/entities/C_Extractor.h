/*
 * C_Resource.h
 */

#ifndef C_EXTRACTOR_H_INCLUDED
#define C_EXTRACTOR_H_INCLUDED

// Project includes
#include <shared/game/Extractor.h>
#include <client/graphics/entities/C_Entity.h>
#include <client/graphics/Renderable.h>

#pragma warning( push )
#pragma warning( disable : 4250 )

class C_Extractor : public Extractor, public C_Entity {
public:
	//Mesh* m_pMesh;

	C_Extractor();
	C_Extractor(Entity *);
	//void draw();
};

#pragma warning( pop )


#endif