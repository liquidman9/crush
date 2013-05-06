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
#include <server/game/S_Resource.h>
#include <server/game/S_Asteroid.h>
#include <server/game/S_TractorBeam.h>
#include <server/game/ServerEntity.h>

class S_TractorBeam;

#pragma warning( push )
#pragma warning( disable : 4250 )

#define PI 3.14159f

namespace server {
	namespace entities {
		namespace ship {
			static const string CONFIG_PREFIX = "ship_";

			static float mass = 1000;
			static float forward_impulse = 1000.0f;
			static float rotation_impulse = 600.0f;
			static float braking_impulse = 500.0f;

			static float max_velocity = 100.0f;
			static float max_rotation_velocity = 2.5f;

			inline void initFromConfig() {
				ConfigSettings::config->getValue(CONFIG_PREFIX + "mass", mass);
				ConfigSettings::config->getValue(CONFIG_PREFIX + "forward_impulse", forward_impulse);
				ConfigSettings::config->getValue(CONFIG_PREFIX + "rotation_impulse", rotation_impulse);
				ConfigSettings::config->getValue(CONFIG_PREFIX + "braking_impulse", braking_impulse);
			}
		}
	}
}


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
	void calcTractorBeam();

	virtual void update(float delta_time);

	virtual D3DXVECTOR3 calculateRotationalInertia(float mass);
	bool interact(S_Resource *);
	void interact(S_Asteroid *);
	void interact(S_Ship *);

	void print();

};

#pragma warning( pop )

#endif S_SHIP_H_INCLUDED