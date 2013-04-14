/*
 * R_Ship.cpp
 */

#include "R_Ship.h"
#include "Gbls.h"
//Mesh R_Ship::s_mesh;

R_Ship::R_Ship()
{
}

R_Ship::R_Ship(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int pNum, bool tBeamOn, Mesh* pMesh, D3DXCOLOR color) :
	Ship(pos, dir, pNum, tBeamOn),
	m_pMesh(pMesh),
	m_color(color)
{
	D3DXMatrixIdentity(&m_matInitScaleRot);
}

void R_Ship::draw()
{
	//this can all probably be a lot faster

	//D3DXVECTOR3 zaxis; // z axis just direction vector
	D3DXVECTOR3 xaxis(m_dir.z, 0, -m_dir.x);
	D3DXVec3Normalize(&xaxis, &xaxis);
	D3DXVECTOR3 yaxis;
	D3DXVec3Cross(&yaxis, &m_dir, &xaxis);

	D3DXMATRIX matRotate(
		xaxis.x, yaxis.x, m_dir.x, 0,
		xaxis.y, yaxis.y, m_dir.y, 0,
		xaxis.z, yaxis.z, m_dir.z, 0,
		0, 0, 0, 1);
	D3DXMATRIX matTranslate;
	D3DXMatrixTranslation(&matTranslate, m_pos.x, m_pos.y, m_pos.z);
	Gbls::pd3dDevice->SetTransform(D3DTS_WORLD, &(m_matInitScaleRot*matRotate*matTranslate));
	m_pMesh->draw();
}
