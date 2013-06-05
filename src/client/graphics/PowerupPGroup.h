//=================================================================================================
// PowerupPGroup.h - Powerup Particle Group
//=================================================================================================

#ifndef POWERUPPGROUP_H_INCLUDED
#define POWERUPPGROUP_H_INCLUDED

// Defines
#define WIN32_LEAN_AND_MEAN

// Global includes
#include <d3dx9.h>

// Project includes
#include <client/graphics/ParticleGroup.h>
#include <client/graphics/entities/C_Powerup.h>
#include <client/graphics/ParticleSystem.h> // for static functions

class PowerupPGroup : public ParticleGroup {
public:
	PowerupPGroup(LPDIRECT3DTEXTURE9 ptexParticle);
	~PowerupPGroup();

	//static const float defaultSize;
	static const float ttl;
	static const float defaultSize;
	static const float ballScale;
	static const float speed;
	static const int color_r;
	static const int color_g;
	static const int color_b;
	C_Powerup * pEnt;
	float prevElapsedTime;
	
	virtual void updateGroup();
	virtual bool initNewParticle(Particle * pParticle);
	virtual bool updateParticle(Particle * pParticle, float elapsedTime);

};

#endif // POWERUPPGROUP_H_INCLUDED
