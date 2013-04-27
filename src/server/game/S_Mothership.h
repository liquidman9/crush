/*
 * S_Mothership.h
 */

#ifndef S_MOTHERSHIP_H_INCLUDED
#define S_MOTHERSHIP_H_INCLUDED

// External includes
#define WIN32_LEAN_AND_MEAN
#include <d3dx9.h>

// Project includes
#include <shared/game/Mothership.h>
#include <server/game/ServerEntity.h>
#include <shared/InputState.h>


#pragma warning( push )
#pragma warning( disable : 4250 )

class S_Mothership : public Mothership, public ServerEntity{ //switch back to capsule

public:
	// Fields

	// Constructors
	S_Mothership(int);
	S_Mothership(D3DXVECTOR3, Quaternion, int);

	// Methods
	virtual D3DXVECTOR3 calculateRotationalInertia(float mass);
};

#pragma warning( pop )

#endif S_MOTHERSHIP_H_INCLUDED