/*
 * S_Ship.h
 */

#ifndef S_SHIP_H_INCLUDED
#define S_SHIP_H_INCLUDED

// External includes
#define WIN32_LEAN_AND_MEAN
#include <d3dx9.h>

// Project includes
#include <shared/game/Ship.h>
#include <server/game/ServerEntity.h>
#include <shared/InputState.h>

#pragma warning( push )
#pragma warning( disable : 4250 )

class S_Ship : public Ship, public ServerEntity{ //switch back to capsule

public:
	// Fields
	float m_thrust;
	D3DXVECTOR3 forward_rot_thruster;
	D3DXVECTOR3 reverse_rot_thruster;

	// Constructors
	S_Ship();
	S_Ship(D3DXVECTOR3, Quaternion, int);

	// Methods
	void init();
	void rotate(D3DXVECTOR3);
	void calculate(float);
	void addPlayerInput(InputState);

	virtual D3DXVECTOR3 calculateRotationalInertia(float mass);

};

#pragma warning( pop )

#endif S_SHIP_H_INCLUDED