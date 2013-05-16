/*
 * S_Ship.h
 */

#ifndef S_SHIP_H_INCLUDED
#define S_SHIP_H_INCLUDED

// External includes
#define WIN32_LEAN_AND_MEAN
#include <d3dx9.h>

// Project includes
#include <shared/InputState.h>
#include <shared/ConfigSettings.h>
#include <shared/game/Ship.h>
#include <server/game/S_TractorBeam.h>
#include <server/game/S_Resource.h>
#include <server/game/S_Asteroid.h>
#include <server/game/ServerEntity.h>
#include <server/Globals.h>

class S_TractorBeam;

#pragma warning( push )
#pragma warning( disable : 4250 )

class S_Ship : public Ship, public ServerEntity{ //switch back to capsule
private:
	float m_forward_impulse;
	float m_rotation_impulse;
	float m_braking_impulse;
	
	float m_max_velocity;
	float m_max_rotation_velocity;

	// Fields
	D3DXVECTOR3 forward_rot_thruster;
	D3DXVECTOR3 reverse_rot_thruster;

	bool m_thrusting;
	bool m_rotating;

public:

	S_Resource * m_resource;
	S_TractorBeam * m_tractorBeam;

	// Constructors
	S_Ship();
	S_Ship(D3DXVECTOR3, Quaternion, int);

	// Methods
	void init();
	void addPlayerInput(InputState);
	void applyDamping();
	D3DXVECTOR3 getDamping();
	void calcTractorBeam();
	void updateHeldObject();

	virtual void update(float delta_time);

	virtual D3DXMATRIX calculateRotationalInertia(float mass);

	//todo switch interacts to true->success instead of true->collsion response
	bool interact(S_Resource *);
	bool interact(S_Asteroid *);
	bool interact(S_Ship *);

};

#pragma warning( pop )

#endif S_SHIP_H_INCLUDED