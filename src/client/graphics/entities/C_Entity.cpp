/*
 * R_Ship.cpp
 */

// Global includes
#include <iostream>

// Project includes
#include <client/Gbls.h>
#include <client/graphics/entities/C_Entity.h>
 
//Mesh C_Ship::s_mesh;

// These two constructors (C_Entity() and C_Entity(Entity *) have been commented out as they should -not- be used.
/*
C_Entity::C_Entity()
{
}

C_Entity::C_Entity(Entity * newEnt) {
	Entity * srcEntity = dynamic_cast<Entity*>(newEnt);
	if (srcEntity != NULL) {
		m_pos = srcEntity->m_pos;
		m_dir = srcEntity->m_dir;
	} else {
		// This is a problem, print some debug output but keep going
		cerr << "SEVERE : Unable to create Client Entity!" << endl;
	}
}
*/

// Draw is commented out as well, for the same reason. C_Entity should not be drawn. Subclasses of C_Entity should be.
/*
void C_Entity::draw()
{
	// Rotate ship to face correct direction

	/* use this code if we want to handle edge case (y = 1 or y = -1) in this function /
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
	Gbls::pd3dDevice->SetTransform(D3DTS_WORLD, &(m_pMesh->m_matInitScaleRot*matRotate*matTranslate));
	m_pMesh->draw();

}

*/
