/*
 * C_Mothership.cpp
 */

// Project includes
#include <client/Gbls.h>
#include <client/graphics/entities/C_Mothership.h>
 
//Mesh C_Ship::s_mesh;

C_Mothership::C_Mothership() :
	Entity(MOTHERSHIP)
{ 
	//cerr << "Creating C_Asteroid with default constructor" << endl;
}


C_Mothership::C_Mothership(Entity * newEnt) :
	Entity(MOTHERSHIP)
{
//cerr << "Creating C_Mothership with Entity * constructor" << endl;
	Mothership * srcMothership = dynamic_cast<Mothership*>(newEnt);
	if (srcMothership != NULL) {
		m_pos = srcMothership->m_pos;
		m_orientation = srcMothership->m_orientation;
		m_playerNum = srcMothership->m_playerNum;
	    m_resources = srcMothership->m_resources;
		m_pMesh = &Gbls::mothershipMesh;
	} else {
		cerr << "SEVERE : Unable to create Client Asteroid!" << endl;
	}
}

//void C_Mothership::draw()
//{
//
//	// Translate Mothership to correct possition
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