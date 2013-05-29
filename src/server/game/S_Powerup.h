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
#include <server/game/S_Ship.h>
#include <shared/InputState.h>

#define SHIP_PLAYERNUM_TYPE char

class S_Ship;

#pragma warning( push )
#pragma warning( disable : 4250 )


class S_Powerup : public Powerup, public ServerEntity{ //switch back to capsule

public:

	// Fields
	S_Ship * m_ship;
	long m_totalTimeLength;
	long m_startTime;
	float m_impulseRate; // config
	float m_maxVelocityRate; //config

	// Constructors
	S_Powerup(D3DXVECTOR3, Quaternion, PowerType type);

	// Methods
	void pickUp(S_Ship *);
	void start();
	void end();
	bool check(long);
	void pulseAll(vector<ServerEntity *> entities);

	virtual D3DXMATRIX calculateRotationalInertia(float mass);
	void update(float delta_time);
};

#pragma warning( pop )

#endif S_POWERUP_H_INCLUDED