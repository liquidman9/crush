/*
 * C_TractorBeam.h
 */

#ifndef C_TRACTORBEAM_H_INCLUDED
#define C_TRACTORBEAM_H_INCLUDED

// Project includes
#include <shared/game/TractorBeam.h>
#include <client/graphics/entities/C_Entity.h>
#include <client/graphics/Renderable.h>

#pragma warning( push )
#pragma warning( disable : 4250 )

class C_TractorBeam : public TractorBeam, public C_Entity {
public:
	Mesh* m_pMesh;

	C_TractorBeam();
	C_TractorBeam(Entity *);
	void draw();
};

#pragma warning( pop )


#endif