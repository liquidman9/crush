/*
 * S_Resource.h
 */

#ifndef S_RESOURCE_H_INCLUDED
#define S_RESOURCE_H_INCLUDED

// External includes
#define WIN32_LEAN_AND_MEAN
#include <d3dx9.h>

// Project includes
#include <shared/game/Resource.h>
#include <server/game/ServerEntity.h>
#include <shared/InputState.h>


#pragma warning( push )
#pragma warning( disable : 4250 )

class S_Resource : public Resource, public ServerEntity{ //switch back to capsule

public:
	// Fields

	// Constructors
	S_Resource();
	S_Resource(D3DXVECTOR3, Quaternion);

	// Methods
	virtual D3DXVECTOR3 calculateRotationalInertia(float mass);
};

#pragma warning( pop )

#endif S_RESOURCE_H_INCLUDED