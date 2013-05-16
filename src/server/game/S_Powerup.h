/*
 * S_Powerup.h
 */

#ifndef S_POWERUP_H_INCLUDED
#define S_POWERUP_H_INCLUDED

// External includes
#define WIN32_LEAN_AND_MEAN
#include <d3dx9.h>

// Project includes
#include <shared/game/Powerup.h>
#include <server/game/ServerEntity.h>
#include <shared/InputState.h>

#define SHIP_PLAYERNUM_TYPE char

#pragma warning( push )
#pragma warning( disable : 4250 )

class S_Powerup : public Powerup, public ServerEntity{ //switch back to capsule

public:

	// Fields
	
	// Constructors
	S_Powerup(D3DXVECTOR3, Quaternion);

	// Methods
	virtual D3DXMATRIX calculateRotationalInertia(float mass);
	void update(float delta_time);
};

#pragma warning( pop )

#endif S_POWERUP_H_INCLUDED