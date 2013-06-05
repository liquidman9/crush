/*
 * EngineBeamPGroup.cpp
 */

// Project includes
#include <client/graphics/PowerupPGroup.h>
#include <client/graphics/ParticleSystem.h>

const float PowerupPGroup::ttl = 0.5f;
const float PowerupPGroup::defaultSize = 4.0f;
const float PowerupPGroup::ballScale = 4.0f;
const float PowerupPGroup::speed = 60.0f;
const int PowerupPGroup::color_r = 255;
const int PowerupPGroup::color_g = 255;
const int PowerupPGroup::color_b = 255;
const static int nx = 10;
const static int ny = 11;

PowerupPGroup::PowerupPGroup(LPDIRECT3DTEXTURE9 ptexParticle) {
	pEnt = NULL;
	m_partList = NULL;
	D3DXMatrixIdentity(&m_worldTransformMat);
    m_size = defaultSize; // Particle's size
    m_numToRelease = 3;
	m_releaseInterval = 0.02f; //no period release
	m_lastUpdate = 0.0f;
	m_currentTime = 0.0f;
    m_ptexParticle = ptexParticle; // Particle's texture
	m_perParticleColor = true;
	m_curColor = D3DCOLOR_XRGB(255,255,255);
	
	float prevElapsedTime = -1.0f;
}

PowerupPGroup::~PowerupPGroup() {
	while( m_partList )
    {
        Particle *pParticle = m_partList;
        m_partList = pParticle->m_pNext;
        delete pParticle;
    }
    m_partList = NULL;
	pEnt = NULL;
}

void PowerupPGroup::updateGroup() {
	D3DXMATRIX scaleMat, rotMat, transMat;
	//create scale matrix
	D3DXMatrixScaling(&scaleMat, ballScale, ballScale, ballScale);
	//rotate to direction
	//static int x = 0;
	//static int y = 0;
	//static int z = 0;
	D3DXMatrixIdentity(&rotMat);
	//D3DXMatrixRotationAxis(&rotMat, &ParticleSystem::getRandomVector(), PI/180.0f);
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
	//translate to tBeamEnt->m_start
	D3DXMatrixTranslation(&transMat, pEnt->m_pos.x, pEnt->m_pos.y, pEnt->m_pos.z);
	m_worldTransformMat = scaleMat*rotMat*transMat;
}

bool PowerupPGroup::initNewParticle(Particle * pParticle) {
	//static int x = 0;
	//static int y = 0;
	//float lon = 360 * ((x+0.5f) / nx);
	////float midpt = (y+0.5f) / ny;
	//float lat = 180 * asin(2*((y+0.5f)/ny-0.5f));
	//y +=1;
	//x += (y/ny);
	//y = y%ny;
	//pParticle->m_vCurPos.x = cos(lat)*cos(lon);
	//pParticle->m_vCurPos.y = cos(lat)*sin(lon);
	//pParticle->m_vCurPos.z = sin(lat);

	pParticle->m_color = D3DCOLOR_XRGB(rand()%256,rand()%256,rand()%256);
	D3DXVECTOR3 zeroVec(0,0,0);
	do {
	pParticle->m_vCurPos.x = ParticleSystem::getRandomMinMax(-1.0f, 1.0f);
	pParticle->m_vCurPos.y = ParticleSystem::getRandomMinMax(-1.0f, 1.0f);
	pParticle->m_vCurPos.z = ParticleSystem::getRandomMinMax(-1.0f, 1.0f);
	} while (D3DXVec3Length(&(pParticle->m_vCurPos - zeroVec)) > 1.0f);
	pParticle->m_vCurVel = pParticle->m_vCurPos;
	pParticle->m_vCurPos = zeroVec;
	D3DXVec3Normalize(&pParticle->m_vCurPos, &pParticle->m_vCurPos);
	pParticle->m_vCurPos/=3.0f;
	return true;
}

/* Update particle position/velocity.
 * Returns true if particle is still valid
 */
bool PowerupPGroup::updateParticle(Particle * pParticle, float elapsedTime) {

	// check time to live
	float age = (m_currentTime - pParticle->m_fInitTime)/ttl;
	if ( age > 1.0f )
		return false;

	//float scale = (age);
	////m_size = sqrt(defaultSize*age*5);
	////pParticle->m_vCurVel -= pParticle->m_vCurVel*0.03;
	////pParticle->m_color = D3DCOLOR_XRGB((int)(color_r*scale),(int)(color_g*scale),(int)(color_b*scale));
	//
	pParticle->m_vCurPos += pParticle->m_vCurVel*elapsedTime;
	return true;
}