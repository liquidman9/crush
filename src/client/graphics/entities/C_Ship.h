/*
 * R_Ship.h
 */

#ifndef C_SHIP_H_INCLUDED
#define C_SHIP_H_INCLUDED

// Project includes
#include <shared/game/Ship.h>
#include <client/graphics/entities/C_Entity.h>
#include <client/graphics/Renderable.h>

#pragma warning( push )
#pragma warning( disable : 4250 )

class C_Ship : public Ship, public C_Entity {
public:
	Mesh* m_pMesh;
	D3DXCOLOR m_color;

	C_Ship();
	C_Ship(Entity *);
	void draw();
};

#pragma warning( pop )


#endif