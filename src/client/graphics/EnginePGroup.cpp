/*
 * EngineBeamPGroup.cpp
 */

// Project includes
#include <client/graphics/EnginePGroup.h>
#include <client/graphics/ParticleSystem.h>

const float EnginePGroup::defaultTTL = 1.5f;
const float EnginePGroup::defaultSize = 4.0f;
const float EnginePGroup::defaultSpeed = -7.5f;
const float EnginePGroup::zStartOffset = -5.5f;
const int EnginePGroup::color_r = 200;
const int EnginePGroup::color_g = 0;
const int EnginePGroup::color_b = 255;

EnginePGroup::EnginePGroup(LPDIRECT3DTEXTURE9 ptexParticleNorm, LPDIRECT3DTEXTURE9 ptexParticleSpeedup) {
	m_partList = NULL;
	D3DXMatrixIdentity(&m_worldTransformMat);
    m_numToRelease = 1;
	m_releaseInterval = 1.0f/40.0f; 
	m_lastUpdate = 0.0f;
	m_currentTime = 0.0f;
	m_ptexParticleNorm = ptexParticleNorm;
	m_ptexParticleSpeedup = ptexParticleSpeedup;
    m_ptexParticle = ptexParticleNorm; // Particle's texture


	m_ttl = defaultTTL;
    m_size = defaultSize; // Particle's size
	m_speed = defaultSpeed;
	
	
	shipEnt = NULL;
	float prevElapsedTime = -1.0f;
}

EnginePGroup::~EnginePGroup() {
	while( m_partList )
    {
        Particle *pParticle = m_partList;
        m_partList = pParticle->m_pNext;
        delete pParticle;
    }
    m_partList = NULL;
	shipEnt = NULL; // not mine to delete though
}

void EnginePGroup::updateGroup() {
	if (this->shipEnt) {
		if (shipEnt->m_hasPowerup && shipEnt->m_powerupType == SPEEDUP && shipEnt->m_powerupStateType == CONSUMED) {
			m_size = defaultSize*2.0;
			//m_speed = defaultSpeed*2.0;
			m_ttl = defaultTTL*2.0;
			m_ptexParticle = m_ptexParticleSpeedup;
		} else {
			m_size = defaultSize;
			m_speed = defaultSpeed;
			m_ttl = defaultTTL;
			m_ptexParticle = m_ptexParticleNorm;
		}
	}
	////update color when color added to tbeam
	////m_color = tBeamEnt->m_color;
	//Quaternion orientation = shipEnt->m_orientation;
	////float scaleXY = tBeamEnt->m_sentRadius;
	////float scaleZ = D3DXVec3Length(&beamVec)/defaultLength;
	////m_size = defaultSize * sqrt(tBeamEnt->m_sentRadius * scaleZ);
	//D3DXMATRIX /*scaleMat,*/ rotMat, transMat;
	////create scale matrix
	////D3DXMatrixScaling(&scaleMat, scaleXY, scaleXY, scaleZ);
	////rotate to direction
	//D3DXMatrixIdentity(&rotMat);
	//if (!(beamVec.x == 0 && beamVec.y == 0)) { // only do if new vec isn't z-axis vec
	//	D3DXVec3Normalize(&beamVec, &beamVec);
	//	D3DXVECTOR3 right;
	//	D3DXVECTOR3 up(0,1.0f,0);
	//	D3DXVec3Cross(&right, &up, &beamVec);
	//	D3DXVec3Normalize(&right, &right);
	//	D3DXVec3Cross(&up, &beamVec, &right);
	//	rotMat._11 = right.x;   rotMat._12 = right.y;   rotMat._13 = right.z;
	//	rotMat._21 = up.x;      rotMat._22 = up.y;      rotMat._23 = up.z;
	//	rotMat._31 = beamVec.x; rotMat._32 = beamVec.y; rotMat._33 = beamVec.z;
	//}
	////translate to tBeamEnt->m_start
	//D3DXMatrixTranslation(&transMat, tBeamEnt->m_start.x, tBeamEnt->m_start.y, tBeamEnt->m_start.z);
	//m_worldTransformMat = scaleMat*rotMat*transMat;

	//// TODO fix to work when real tractor beams are sent from network
}

bool EnginePGroup::initNewParticle(Particle * pParticle) {
	//float x, y;
	//do { // get random, evenly distributed points in circle
	//	x = ParticleSystem::getRandomMinMax(-1.0f, 1.0f);
	//	y = ParticleSystem::getRandomMinMax(-1.0f, 1.0f);
	//} while (sqrtf(x*x + y*y) > 1.0f);

	if (shipEnt->m_thruster < 0.00001) { // essentially no thrust
		return false;
	}

	pParticle->m_color = D3DCOLOR_XRGB(color_r,color_g,color_b);
	pParticle->m_vCurPos.x = 0.0f;
	pParticle->m_vCurPos.y = 0.0f;
	pParticle->m_vCurPos.z = zStartOffset;
	pParticle->m_vCurVel.x = 0.0f;
	pParticle->m_vCurVel.y = 0.0f;
	pParticle->m_vCurVel.z = m_speed;
	Quaternion temp_quat;
	D3DXMATRIX mat;
	D3DXMatrixRotationQuaternion(&mat, D3DXQuaternionNormalize(&temp_quat, &shipEnt->m_orientation));
	D3DXVec3TransformCoord(&pParticle->m_vCurVel, &pParticle->m_vCurVel, &mat);
	D3DXVec3TransformCoord(&pParticle->m_vCurPos, &pParticle->m_vCurPos, &mat);
	pParticle->m_vCurVel += shipEnt->m_velocity;
	pParticle->m_vCurPos += shipEnt->m_pos;

	return true;
}

/* Update particle position/velocity.
 * Returns true if particle is still valid
 */
bool EnginePGroup::updateParticle(Particle * pParticle, float elapsedTime) {

	// check time to live
	float age = (m_currentTime - pParticle->m_fInitTime)/m_ttl;
	if ( age > 1.0f )
		return false;

	float scale = (1.0f-age);
	
	//pParticle->m_color = D3DCOLOR_XRGB((int)(color_r*scale),(int)(color_g*scale),(int)(color_b*scale));
	pParticle->m_color = D3DCOLOR_XRGB((int)(color_r*scale),(int)(color_g*scale),(int)(color_b*scale));
	
	pParticle->m_vCurPos += pParticle->m_vCurVel*elapsedTime;

	//pParticle->m_vCurPos.z += speed * elapsedTime;
	//if (pParticle->m_vCurPos.z > zEndOffset)
	//	return false;
	//if (prevElapsedTime != elapsedTime) { // Different rotation, build new matrix
	//	D3DXMatrixRotationZ(&rotMat, rotSpeed * elapsedTime);
	//}
	//D3DXVec3TransformCoord(&pParticle->m_vCurPos, &pParticle->m_vCurPos, &rotMat);
	return true;
}