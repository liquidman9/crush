/*
 * S_Asteroid.h
 */

#ifndef S_ASTEROID_H_INCLUDED
#define S_ASTEROID_H_INCLUDED

// External includes
#define WIN32_LEAN_AND_MEAN
#include <d3dx9.h>

// Project includes
#include <shared/game/Asteroid.h>
#include <server/game/ServerEntity.h>
#include <shared/InputState.h>
#include <shared/ConfigSettings.h>

#pragma warning( push )
#pragma warning( disable : 4250 )

class S_Asteroid : public Asteroid, public ServerEntity{ //switch back to capsule

public:

	// Fields

	// Constructors
	S_Asteroid(D3DXVECTOR3, Quaternion, float);

	// Methods
	static float calculateMass(float radius, float density);
	virtual D3DXMATRIX calculateRotationalInertia(float mass);
};

#pragma warning( pop )

#endif S_ASTEROID_H_INCLUDED