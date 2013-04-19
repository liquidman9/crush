/*
 * C_Entity.h
 */

#ifndef C_ENTITY_H_INCLUDED
#define C_ENTITY_H_INCLUDED

// Project includes
#include <shared/game/Ship.h>
#include <client/graphics/Mesh.h>
#include <client/graphics/Renderable.h>

class C_Entity : public virtual Entity, public virtual Renderable {
public:
	// Fields
	Mesh* m_pMesh;
	D3DXCOLOR m_color;
};


#endif