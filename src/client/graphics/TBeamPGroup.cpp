/*
 * TBeamPGroup.cpp
 */

// Project includes
#include <client/graphics/TBeamPGroup.h>
#include <client/graphics/ParticleSystem.h>


const float TBeamPGroup::speed = -0.5f;
const float TBeamPGroup::rotSpeed = 0.01f;

TBeamPGroup::TBeamPGroup(LPDIRECT3DTEXTURE9 * pptexParticle, D3DXCOLOR color, float partSize) {
	// TODO fix to take in tBeam pointer
	m_partList = NULL;
	m_color = color;
	D3DXMatrixIdentity(&m_worldTransformMat);
    m_size = partSize; // Particle's size
    m_numToRelease = 1;
	m_releaseInterval = 1000.0f/61.0f; // once per frame
	m_lastUpdate = 0.0f;
	m_currentTime = 0.0f;
    m_pptexParticle = pptexParticle; // Particle's texture

	beamEnt = NULL;
	float prevElapsedTime = -1.0f;
}

TBeamPGroup::~TBeamPGroup() {

}

void TBeamPGroup::updateGroup() {
	// Do nothing for now
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
	pParticle->m_vCurPos.z = 0.999f;
}

bool TBeamPGroup::updateParticle(Particle * pParticle, float elapsedTime) {
	pParticle->m_vCurPos.z += speed * elapsedTime;
	if (pParticle->m_vCurPos.z < 0.0f || pParticle->m_vCurPos.z > 1.0f) {
		return false;
	}
	if (prevElapsedTime != elapsedTime) { // Different rotation, build new matrix
		D3DXMatrixRotationZ(&rotMat, rotSpeed * elapsedTime);
	}
	D3DXVec3TransformCoord(&pParticle->m_vCurPos, &pParticle->m_vCurPos, &rotMat);
	return true;
}