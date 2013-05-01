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
#include <shared/game/TractorBeam.h>
#include <client/graphics/ParticleSystem.h> // for static functions

class TBeamPGroup : public ParticleGroup {
	
	TBeamPGroup(LPDIRECT3DTEXTURE9 * pptexParticle, D3DXCOLOR color, float partSize);
	~TBeamPGroup();

	static const float speed;
	static const float rotSpeed;

	TractorBeam * beamEnt;
	float prevElapsedTime;
	D3DXMATRIX rotMat;
	
	virtual void updateGroup();
	virtual void initNewParticle(Particle * pParticle);
	virtual bool updateParticle(Particle * pParticle, float elapsedTime);

};

#endif // TBEAMPGROUP_H_INCLUDED
