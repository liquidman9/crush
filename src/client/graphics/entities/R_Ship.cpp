/*
 * R_Ship.cpp
 */

// Project includes
#include <client/Gbls.h>
#include <client/graphics/entities/R_Ship.h>
 
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
	// Rotate ship to face correct direction

	/* use this code if we want to handle edge case (y = 1 or y = -1) in this function */
	//D3DXVec3Normalize(&m_dir, &m_dir);
	//D3DXMATRIX matRotate;
	//if(m_dir.y > 0.99999f) {
	//	D3DXMatrixRotationX(&matRotate, D3DXToRadian(-90));
	//} else if(m_dir.y < -0.99999f) {
	//	D3DXMatrixRotationX(&matRotate, D3DXToRadian(90));
	//} else {
	//	D3DXVECTOR3 right;
	//	D3DXVECTOR3 up(0,1.0f,0);
	//	D3DXVec3Cross(&right, &up, &m_dir);
	//	D3DXVec3Normalize(&right, &right);
	//	D3DXVec3Cross(&up, &m_dir, &right);
	//	// TODO waste of copy, fix later
	//	matRotate = D3DXMATRIX(
	//		right.x, right.y, right.z, 0,
	//		up.x, up.y, up.z, 0,
	//		m_dir.x, m_dir.y, m_dir.z, 0, 
	//		0, 0, 0, 1);
	//}

	D3DXVec3Normalize(&m_dir, &m_dir);
	D3DXVECTOR3 right;
	D3DXVECTOR3 up(0,1.0f,0);
	D3DXVec3Cross(&right, &up, &m_dir);
	D3DXVec3Normalize(&right, &right);
	D3DXVec3Cross(&up, &m_dir, &right);
	D3DXMATRIX matRotate(
		right.x, right.y, right.z, 0,
		up.x, up.y, up.z, 0,
		m_dir.x, m_dir.y, m_dir.z, 0, 
		0, 0, 0, 1);

	// Translate ship to correct possition
	D3DXMATRIX matTranslate;
	D3DXMatrixTranslation(&matTranslate, m_pos.x, m_pos.y, m_pos.z);

	// Apply transforms
	Gbls::pd3dDevice->SetTransform(D3DTS_WORLD, &(m_matInitScaleRot*matRotate*matTranslate));
	m_pMesh->draw();
}