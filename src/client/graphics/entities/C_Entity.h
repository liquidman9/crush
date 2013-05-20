/*
 * R_Ship.h
 */

#ifndef C_ENTITY_H_INCLUDED
#define C_ENTITY_H_INCLUDED

#include <shared/game/Ship.h>
#include <client/graphics/Mesh.h>
#include <client/graphics/Renderable.h>

class C_Entity : public virtual Entity {
public:
	Mesh* m_pMesh;
	D3DXMATRIX worldMat;
	virtual void updateWorldMat() {
		D3DXMATRIX matTranslate;
		D3DXMatrixTranslation(&matTranslate, m_pos.x, m_pos.y, m_pos.z);

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


#endif