//=================================================================================================
// ParticleGroup.h - Sprite handling class
//=================================================================================================

#ifndef PARTICLEGROUP_H_INCLUDED
#define PARTICLEGROUP_H_INCLUDED

// Defines
#define WIN32_LEAN_AND_MEAN

// Global includes
#include <d3dx9.h>

// Project includes
#include <client/graphics/Particle.h>

// TODO probably change times to use ulonglongs or something, not floats
class ParticleGroup {
public:
	Particle *	m_partList;
	D3DXMATRIX	m_worldTransformMat;
    float		m_size; // Particle's size
    DWORD       m_numToRelease;
    float       m_releaseInterval;
	float       m_lastUpdate;
	float		m_currentTime;
    LPDIRECT3DTEXTURE9 m_ptexParticle; // Particle's texture
	
	virtual void updateGroup() = 0;
	virtual bool initNewParticle(Particle * pParticle) = 0;
	virtual bool updateParticle(Particle * pParticle, float elapsedTime) = 0;

};

#endif // PARTICLEGROUP_H_INCLUDED
