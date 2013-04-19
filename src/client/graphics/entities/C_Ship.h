/*
 * C_Ship.h
 */

#ifndef C_SHIP_H_INCLUDED
#define C_SHIP_H_INCLUDED

// Project includes
#include <shared/game/Ship.h>
#include <client/graphics/entities/C_Entity.h>
#include <client/graphics/Renderable.h>
/*
- Represents a Client-side Ship Entity
- 
- This class must call Entity's constructor directly!
 */
class C_Ship : public Ship, public C_Entity {
public:
	// Fields
	Mesh* m_pMesh;
	D3DXCOLOR m_color;

	// Constructors
	C_Ship();
	C_Ship(Entity *);
	void draw();
};


#endif