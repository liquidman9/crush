/*
 * C_Powerup.h
 */

#ifndef C_POWERUP_H_INCLUDED
#define C_POWERUP_H_INCLUDED

// Project includes
#include <shared/game/Powerup.h>
#include <client/graphics/entities/C_Entity.h>

#pragma warning( push )
#pragma warning( disable : 4250 )

class C_Powerup : public Powerup, public C_Entity {
public:
	C_Powerup();
	C_Powerup(Entity *);
	//void draw();
	virtual void updateWorldMat() {
		D3DXMATRIX matTranslate;
		D3DXMatrixTranslation(&matTranslate, m_pos.x, m_pos.y-3, m_pos.z);

		// Rotation with quaternion
		D3DXMATRIX matRotate;
		D3DXQUATERNION temp_q;
		D3DXMatrixRotationQuaternion(&matRotate, D3DXQuaternionNormalize(&temp_q, &m_orientation));

		// Apply transforms
		if (m_pMesh)
			worldMat = m_pMesh->m_matInitScaleRot*matRotate*matTranslate;
		else
			worldMat = /*m_pMesh->m_matInitScaleRot**/matRotate*matTranslate;
	}
};

#pragma warning( pop )


#endif