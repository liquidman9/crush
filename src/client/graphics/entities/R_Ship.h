/*
 * R_Ship.h
 */

#ifndef R_SHIP_H_INCLUDED
#define R_SHIP_H_INCLUDED

// Project includes
#include <shared/game/Ship.h>
#include <client/graphics/Renderable.h>

class R_Ship : public Ship {
public:
	Mesh* m_pMesh;
	D3DXCOLOR m_color;

	R_Ship();
	R_Ship(Entity *);
	//this ctor is broken. See it's definition in R_Ship.cpp for more info
	//R_Ship(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int pNum, bool tBeamOn, Mesh* pMesh, D3DXCOLOR color);
	void draw();
};


#endif