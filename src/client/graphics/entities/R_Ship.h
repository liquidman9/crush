/*
 * R_Ship.h
 */

#ifndef R_SHIP_H_INCLUDED
#define R_SHIP_H_INCLUDED

#include <shared/game/Ship.h>
#include <client/graphics/Renderable.h>

class R_Ship : public Ship, public Renderable {
public:
	Mesh* m_pMesh;
	D3DXCOLOR m_color;
	D3DXMATRIX m_matInitScaleRot;

	R_Ship();
	R_Ship(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int pNum, bool tBeamOn, Mesh* pMesh, D3DXCOLOR color);
	void draw();
};


#endif