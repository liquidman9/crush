/*
 * C_Extractor.cpp
 */

// Project includes
#include <client/Gbls.h>
#include <client/graphics/entities/C_Extractor.h>
 

C_Extractor::C_Extractor() :
	Entity(EXTRACTOR)
{ 
	//cerr << "Creating C_Extractor with default constructor" << endl;
}


C_Extractor::C_Extractor(Entity * newEnt) :
	Entity(EXTRACTOR)
{
//cerr << "Creating C_Extractor with Entity * constructor" << endl;
	Extractor * srcExtractor = dynamic_cast<Extractor*>(newEnt);
	if (srcExtractor != NULL) {
		m_pos = srcExtractor->m_pos;
		m_orientation = srcExtractor->m_orientation;
		m_pMesh = &Gbls::extractorMesh;
	} else {
		cerr << "SEVERE : Unable to create Client Extractor!" << endl;
	}
}

void C_Extractor::draw()
{
	
	// Translate Extractor to correct possition
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