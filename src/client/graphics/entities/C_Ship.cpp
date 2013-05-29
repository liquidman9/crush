/*
 * R_Ship.cpp
 */

// Project includes
#include <client/Gbls.h>
#include <client/graphics/entities/C_Ship.h>
 
//Mesh C_Ship::s_mesh;

C_Ship::C_Ship() :
	Entity(SHIP)
{ 
	//cerr << "Creating C_Ship with default constructor" << endl;
}

/* This ctor is broken (and others might be too) because ship's base class of entity was inherited from
 * in Ship.h using the virtual keyword as shown in the code snippet below
 * "class Ship : public virtual Entity {"
 * read this link for more info: http://www.parashift.com/c++-faq-lite/virtual-inheritance-ctors.html
 */
//R_Ship::R_Ship(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int pNum, bool tBeamOn, Mesh* pMesh, D3DXCOLOR color) :
//	Ship(pos, dir, pNum, tBeamOn),
//	m_pMesh(pMesh),
//	m_color(color)
//{
//}


C_Ship::C_Ship(Entity * newEnt) :
	Entity(SHIP)
{
//cerr << "Creating C_Ship with Entity * constructor" << endl;
	Ship * srcShip = dynamic_cast<Ship*>(newEnt);
	if (srcShip != NULL) {
		m_pos = srcShip->m_pos;
		m_orientation = srcShip->m_orientation;
		m_thruster = srcShip->m_thruster;
		m_playerNum = srcShip -> m_playerNum;
		m_pMesh = new Mesh();
		m_pMesh->m_dwNumMaterials = 1;
		m_pMesh->m_pMeshMaterials = new D3DMATERIAL9[1];
		m_pMesh->m_pMeshTextures = new LPDIRECT3DTEXTURE9[1];
		m_pMesh->m_pMesh = Gbls::shipMesh.m_pMesh;
		m_pMesh->m_matInitScaleRot = Gbls::shipMesh.m_matInitScaleRot;
		
		switch (m_playerNum) {
		case 0:
			m_pMesh->m_pMeshTextures[0] = Gbls::shipTexture1;
			break;
		case 1:
			m_pMesh->m_pMeshTextures[0] = Gbls::shipTexture2;
			break;
		case 2:
			m_pMesh->m_pMeshTextures[0] = Gbls::shipTexture3;
			break;
		case 3:
			m_pMesh->m_pMeshTextures[0] = Gbls::shipTexture4;
			break;
		default:
			m_pMesh->m_pMeshTextures[0] = Gbls::shipTexture1;
			break;
		}
	} else {
		cerr << "SEVERE : Unable to create Client Ship!" << endl;
	}
}

//void C_Ship::draw()
//{
//	// Translate ship to correct possition
//	D3DXMATRIX matTranslate;
//	D3DXMatrixTranslation(&matTranslate, m_pos.x, m_pos.y, m_pos.z);
//
//	// Rotation with quaternion
//	D3DXMATRIX matRotate;
//	D3DXQUATERNION temp_q;
//	D3DXMatrixRotationQuaternion(&matRotate, D3DXQuaternionNormalize(&temp_q, &m_orientation));
//
//	// Apply transforms
//	Gbls::pd3dDevice->SetTransform(D3DTS_WORLD, &(m_pMesh->m_matInitScaleRot*matRotate*matTranslate));
//	m_pMesh->draw();
//}