/*
 * C_Asteroid.cpp
 */

// Project includes
#include <client/Gbls.h>
#include <client/graphics/entities/C_Asteroid.h>
 
//Mesh C_Ship::s_mesh;

C_Asteroid::C_Asteroid() :
	Entity(ASTEROID)
{ 
	cerr << "Creating C_Asteroid with default constructor" << endl;
}


C_Asteroid::C_Asteroid(Entity * newEnt) :
	Entity(ASTEROID)
{
cerr << "Creating C_Asteroid with Entity * constructor" << endl;
	Asteroid * srcAsteroid = dynamic_cast<Asteroid*>(newEnt);
	if (srcAsteroid != NULL) {
		m_pos = srcAsteroid->m_pos;
		m_orientation = srcAsteroid->m_orientation;
		m_scale = srcAsteroid->m_scale;
		m_pMesh = &Gbls::asteroidMesh;
	} else {
		cerr << "SEVERE : Unable to create Client Asteroid!" << endl;
	}
}

void C_Asteroid::draw()
{

	// Scale Asteroid 
	float conv = 120*100;
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, m_scale/conv, m_scale/conv, m_scale/conv);
	
	// Translate Asteroid to correct possition
	D3DXMATRIX matTranslate;
	D3DXMatrixTranslation(&matTranslate, m_pos.x, m_pos.y, m_pos.z);

	// Rotation with quaternion
	D3DXMATRIX matRotate;
	D3DXQUATERNION temp_q;
	D3DXMatrixRotationQuaternion(&matRotate, D3DXQuaternionNormalize(&temp_q, &m_orientation));

	// Apply transforms
	Gbls::pd3dDevice->SetTransform(D3DTS_WORLD, &(m_pMesh->m_matInitScaleRot*matScale*matRotate*matTranslate));
	m_pMesh->draw();
}