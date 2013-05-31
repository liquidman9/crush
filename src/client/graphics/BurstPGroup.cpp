/*
 * EngineBeamPGroup.cpp
 */

// Project includes
#include <client/graphics/BurstPGroup.h>
#include <client/graphics/ParticleSystem.h>

const float BurstPGroup::ttl = 2.0f;
const float BurstPGroup::defaultSize = 2.0f;
const float BurstPGroup::speed = 60.0f;
const int BurstPGroup::color_r = 255;
const int BurstPGroup::color_g = 255;
const int BurstPGroup::color_b = 255;

BurstPGroup::BurstPGroup(LPDIRECT3DTEXTURE9 ptexParticle) {
	m_partList = NULL;
	D3DXMatrixIdentity(&m_worldTransformMat);
    m_size = defaultSize; // Particle's size
    m_numToRelease = 1500;
	m_releaseInterval = 0.0f; //no period release
	m_lastUpdate = 0.0f;
	m_currentTime = 0.0f;
    m_ptexParticle = ptexParticle; // Particle's texture
	m_perParticleColor = TRUE;
	
	releasePos.x = 0;
	releasePos.y = 0;
	releasePos.z = 0;
	float prevElapsedTime = -1.0f;
}

BurstPGroup::~BurstPGroup() {
	while( m_partList )
    {
        Particle *pParticle = m_partList;
        m_partList = pParticle->m_pNext;
        delete pParticle;
    }
    m_partList = NULL;
}

void BurstPGroup::updateGroup() {
}

bool BurstPGroup::initNewParticle(Particle * pParticle) {
	//float x, y;
	//do { // get random, evenly distributed points in circle
	//	x = ParticleSystem::getRandomMinMax(-1.0f, 1.0f);
	//	y = ParticleSystem::getRandomMinMax(-1.0f, 1.0f);
	//} while (sqrtf(x*x + y*y) > 1.0f);

	pParticle->m_color = D3DCOLOR_XRGB(color_r,color_g,color_b);
	pParticle->m_vCurPos.x = releasePos.x;
	pParticle->m_vCurPos.y = releasePos.y;
	pParticle->m_vCurPos.z = releasePos.z;
	D3DXVECTOR3 randDir(rand()-RAND_MAX/2, rand()-RAND_MAX/2, rand()-RAND_MAX/2);
	D3DXVec3Normalize(&randDir, &randDir);
	randDir*=speed;
	pParticle->m_vCurVel.x = randDir.x;
	pParticle->m_vCurVel.y = randDir.y;
	pParticle->m_vCurVel.z = randDir.z;
	return true;
}

/* Update particle position/velocity.
 * Returns true if particle is still valid
 */
bool BurstPGroup::updateParticle(Particle * pParticle, float elapsedTime) {

	// check time to live
	float age = (m_currentTime - pParticle->m_fInitTime)/ttl;
	if ( age > 1.0f )
		return false;

	float scale = (age);
	//m_size = sqrt(defaultSize*age*5);
	//pParticle->m_vCurVel -= pParticle->m_vCurVel*0.03;
	//pParticle->m_color = D3DCOLOR_XRGB((int)(color_r*scale),(int)(color_g*scale),(int)(color_b*scale));
	
	pParticle->m_vCurPos += pParticle->m_vCurVel*elapsedTime;
	return true;
}