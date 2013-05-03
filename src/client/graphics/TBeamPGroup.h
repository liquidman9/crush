//=================================================================================================
// TBeamPGroup.h - Sprite handling class
//=================================================================================================

#ifndef TBEAMPGROUP_H_INCLUDED
#define TBEAMPGROUP_H_INCLUDED

// Defines
#define WIN32_LEAN_AND_MEAN

// Global includes
#include <d3dx9.h>

// Project includes
#include <client/graphics/ParticleGroup.h>
#include <client/graphics/entities/C_TractorBeam.h>
#include <client/graphics/ParticleSystem.h> // for static functions

class TBeamPGroup : public ParticleGroup {
public:
	TBeamPGroup(LPDIRECT3DTEXTURE9 ptexParticle);
	~TBeamPGroup();

	static const float defaultSize;
	static const float defaultLength;
	static const float speed;
	static const float rotSpeed;

	C_TractorBeam * tBeamEnt;
	float prevElapsedTime;
	D3DXMATRIX rotMat;
	
	virtual void updateGroup();
	virtual void initNewParticle(Particle * pParticle);
	virtual bool updateParticle(Particle * pParticle, float elapsedTime);
	virtual void initBeamToFull();

};

#endif // TBEAMPGROUP_H_INCLUDED
