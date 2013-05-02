//=================================================================================================
// Particle.h - Sprite handling class
//=================================================================================================

#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

// Defines
#define WIN32_LEAN_AND_MEAN

// Global includes
#include <d3dx9.h>

struct Particle
{
	D3DXVECTOR3 m_vCurPos;    // Current position of particle
    D3DXVECTOR3 m_vCurVel;    // Current velocity of particle
    float       m_fInitTime;  // Time of creation of particle

    Particle   *m_pNext;      // Next particle in list
};

#endif // PARTICLE_H_INCLUDED
