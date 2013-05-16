/*
 * C_Powerup.cpp
 */

// Project includes
#include <client/Gbls.h>
#include <client/graphics/entities/C_Powerup.h>
 

C_Powerup::C_Powerup() :
	Entity(POWERUP)
{ 
	//cerr << "Creating C_Powerup with default constructor" << endl;
}


C_Powerup::C_Powerup(Entity * newEnt) :
	Entity(POWERUP)
{
//cerr << "Creating C_Powerup with Entity * constructor" << endl;
	Powerup * srcPowerup = dynamic_cast<Powerup*>(newEnt);
	if (srcPowerup != NULL) {
		m_pos = srcPowerup->m_pos;
		m_orientation = srcPowerup->m_orientation;
		m_pMesh = &Gbls::powerupMesh;
	} else {
		cerr << "SEVERE : Unable to create Client Powerup!" << endl;
	}
}

void C_Powerup::draw()
{
	
	// Translate Powerup to correct possition
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