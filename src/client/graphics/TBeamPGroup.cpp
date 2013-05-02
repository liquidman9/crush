/*
 * TBeamPGroup.cpp
 */

// Project includes
#include <client/graphics/TBeamPGroup.h>
#include <client/graphics/ParticleSystem.h>

const float TBeamPGroup::defaultLength = 10.0f;
const float TBeamPGroup::speed = -0.5f;
const float TBeamPGroup::rotSpeed = 0.4f;

TBeamPGroup::TBeamPGroup(LPDIRECT3DTEXTURE9 ptexParticle, D3DXCOLOR color, float partSize) {
	// TODO fix to take in tBeam pointer
	m_partList = NULL;
	m_color = color;
	D3DXMatrixIdentity(&m_worldTransformMat);
    m_size = partSize; // Particle's size
    m_numToRelease = 3;
	m_releaseInterval = 1.0f/10.0f; 
	m_lastUpdate = 0.0f;
	m_currentTime = 0.0f;
    m_ptexParticle = ptexParticle; // Particle's texture

	beamEnt = NULL;
	float prevElapsedTime = -1.0f;
}

TBeamPGroup::~TBeamPGroup() {
	while( m_partList )
    {
        Particle *pParticle = m_partList;
        m_partList = pParticle->m_pNext;
        delete pParticle;
    }
    m_partList = NULL;
	beamEnt = NULL; // not mine to delete though
}

void TBeamPGroup::updateGroup() {
	// Do nothing for now, later will update matrix/color/etc
	// TODO fix to work when real tractor beams are sent from network
}

void TBeamPGroup::initNewParticle(Particle * pParticle) {
	float x, y;
	do { // get random, evenly distributed points in circle
		x = ParticleSystem::getRandomMinMax(-1.0f, 1.0f);
		y = ParticleSystem::getRandomMinMax(-1.0f, 1.0f);
	} while (sqrtf(x*x + y*y) > 1.0f);
	pParticle->m_vCurPos.x = x;
	pParticle->m_vCurPos.y = y;
	pParticle->m_vCurPos.z = defaultLength;
}

/* Update particle position/velocity.
 * Returns true if particle is still valid
 */
bool TBeamPGroup::updateParticle(Particle * pParticle, float elapsedTime) {
	pParticle->m_vCurPos.z += speed * elapsedTime;
	if (pParticle->m_vCurPos.z < 0.0f || pParticle->m_vCurPos.z > defaultLength) {
		return false;
	}
	if (prevElapsedTime != elapsedTime) { // Different rotation, build new matrix
		D3DXMatrixRotationZ(&rotMat, rotSpeed * elapsedTime);
	}
	D3DXVec3TransformCoord(&pParticle->m_vCurPos, &pParticle->m_vCurPos, &rotMat);
	return true;
}

void TBeamPGroup::initBeamToFull() {
	Particle * pParticle;
	float givenETime = 0.0f;
	bool moreToInit = true;
	while (moreToInit) {
		givenETime += m_releaseInterval;
		for (UINT i = 0; i < m_numToRelease; i++ ) {
			pParticle = new Particle;
			initNewParticle(pParticle);
			moreToInit = updateParticle(pParticle, givenETime);
			if (moreToInit) {
				pParticle->m_pNext = m_partList; // Add to particle list
				m_partList = pParticle;
			} else {
				delete pParticle;
				break;
			}
		}
	}
}