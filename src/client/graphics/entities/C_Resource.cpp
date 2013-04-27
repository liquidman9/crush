/*
 * C_Resource.cpp
 */

// Project includes
#include <client/Gbls.h>
#include <client/graphics/entities/C_Resource.h>
 
//Mesh C_Ship::s_mesh;

C_Resource::C_Resource() :
	Entity(RESOURCE)
{ 
	cerr << "Creating C_Resource with default constructor" << endl;
}


C_Resource::C_Resource(Entity * newEnt) :
	Entity(RESOURCE)
{
cerr << "Creating C_Resource with Entity * constructor" << endl;
	Resource * srcResource = dynamic_cast<Resource*>(newEnt);
	if (srcResource != NULL) {
		m_pos = srcResource->m_pos;
		m_orientation = srcResource->m_orientation;
		m_pMesh = &Gbls::resourceMesh;
	} else {
		cerr << "SEVERE : Unable to create Client Resource!" << endl;
	}
}

void C_Resource::draw()
{
	
	// Translate Resource to correct possition
	D3DXMATRIX matTranslate;
	D3DXMatrixTranslation(&matTranslate, m_pos.x, m_pos.y, m_pos.z);

	// Rotation with quaternion
	D3DXMATRIX matRotate;
	D3DXQUATERNION temp_q;
	D3DXMatrixRotationQuaternion(&matRotate, D3DXQuaternionNormalize(&temp_q, &m_orientation));

	// Apply transforms
	Gbls::pd3dDevice->SetTransform(D3DTS_WORLD, &(m_pMesh->m_matInitScaleRot*matRotate*matTranslate));
	m_pMesh->draw();
}