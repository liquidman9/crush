/*
 * S_TractorBeam.h
 */

#ifndef S_TRACTORBEAM_H_INCLUDED
#define S_TRACTORBEAM_H_INCLUDED

// External includes
#define WIN32_LEAN_AND_MEAN
#include <d3dx9.h>

// Project includes
#include <shared/game/Entity.h>
#include <shared/game/TractorBeam.h>
#include <server/game/ServerEntity.h>
#include <server/game/S_Ship.h>
#include <shared/InputState.h>

class S_Ship;

#pragma warning( push )
#pragma warning( disable : 4250 )

class S_TractorBeam : public TractorBeam, public ServerEntity{ 

public:
	// Fields
	ServerEntity * m_object;
	S_Ship * m_ship; 

	// Constructors
	S_TractorBeam(int);
	S_TractorBeam(D3DXVECTOR3, Quaternion,int); 

	// Methods
	bool isLocked();
	float getCurrentDistance();
	D3DXVECTOR3 getCurrentDirection();
	D3DXVECTOR3 getCurrentDistanceVector();
	D3DXVECTOR3 getDistanceVectorOf(D3DXVECTOR3);
	void lockOn(ServerEntity * entity);
	void calculateForce();
	void setOrientation();
	void setPosition();
	virtual D3DXVECTOR3 calculateRotationalInertia(float mass);
};

#pragma warning( pop )

#endif S_TRACTORBEAM_H_INCLUDED