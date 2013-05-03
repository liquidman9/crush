/*
 * C_TractorBeam.cpp
 */

// Project includes
#include <client/Gbls.h>
#include <client/graphics/entities/C_TractorBeam.h>
 
//Mesh C_Ship::s_mesh;

C_TractorBeam::C_TractorBeam() :
	Entity(TRACTORBEAM)
{ 
	//cerr << "Creating C_TractorBeam with default constructor" << endl;
}


C_TractorBeam::C_TractorBeam(Entity * newEnt) :
	Entity(TRACTORBEAM)
{
//cerr << "Creating C_TractorBeam with Entity * constructor" << endl;
	TractorBeam * srcTractorBeam = dynamic_cast<TractorBeam*>(newEnt);
	if (srcTractorBeam != NULL) {
		m_pos = srcTractorBeam->m_pos;
		m_orientation = srcTractorBeam->m_orientation;
		m_playerNum = srcTractorBeam->m_playerNum;
		m_isOn= srcTractorBeam->m_isOn;
	    m_start= srcTractorBeam->m_start;
		m_end = srcTractorBeam->m_end;
		m_sentRadius = srcTractorBeam->m_sentRadius;

		m_pMesh = &Gbls::tractorBeamMesh;
	} else {
		cerr << "SEVERE : Unable to create Client TractorBeam!" << endl;
	}
}

void C_TractorBeam::draw()
{
	//// Scale Asteroid 
	//float conv = 3.5;
	//D3DXMATRIX matScale;
	//D3DXMatrixScaling(&matScale, conv, conv, conv);

	//// Translate TractorBeam to correct possition
	//D3DXMATRIX matTranslate;
	//D3DXMatrixTranslation(&matTranslate, m_pos.x, m_pos.y, m_pos.z);

	//// Rotation with quaternion
	//D3DXMATRIX matRotate;
	//D3DXQUATERNION temp_q;
	//D3DXMatrixRotationQuaternion(&matRotate, D3DXQuaternionNormalize(&temp_q, &m_orientation));

	//// Apply transforms
	//Gbls::pd3dDevice->SetTransform(D3DTS_WORLD, &(m_pMesh->m_matInitScaleRot*matScale*matRotate*matTranslate));
	//
	//if(m_isOn) 
	//	m_pMesh->draw(); //tmp
}