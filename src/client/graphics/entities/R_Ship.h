/*
 * R_Ship.h
 */

#ifndef R_SHIP_H_INCLUDED
#define R_SHIP_H_INCLUDED

// Project includes
#include <shared/game/Ship.h>
#include <client/graphics/RenderableEntity.h>

class R_Ship : public Ship, public RenderableEntity {
public:
	Mesh* m_pMesh;
	D3DXMATRIX m_matInitScaleRot;

	R_Ship();
	R_Ship(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int pNum, bool tBeamOn, Mesh* pMesh);
	void draw();
};


#endif