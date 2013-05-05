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

namespace server {
	namespace entities {
		namespace ship {
			static const string CONFIG_PREFIX = "ship_";

			static float mass = 1000;
			static float forward_thrust_force = 5000.0f;
			static float rotation_thrust_force = 1000.0f;
			static float stabilizer_ratio = 0.3f;

			inline void initFromConfig() {
				ConfigSettings::config->getValue(CONFIG_PREFIX + "mass", mass);
				ConfigSettings::config->getValue(CONFIG_PREFIX + "rotation_thrust_force", rotation_thrust_force);
				ConfigSettings::config->getValue(CONFIG_PREFIX + "forward_thrust_force", forward_thrust_force);
				ConfigSettings::config->getValue(CONFIG_PREFIX + "stabilizer_ratio", stabilizer_ratio);
			}
		}
	}
}


class S_Ship : public Ship, public ServerEntity{ //switch back to capsule
private:
	float m_forward_thrust_force;
	float m_rotation_thrust_force;
	float m_stabilizer_ratio;

	// Fields
	D3DXVECTOR3 forward_rot_thruster;
	D3DXVECTOR3 reverse_rot_thruster;

public:

	S_Resource * m_resource;
	S_TractorBeam * m_tractorBeam;

	// Constructors
	S_Ship();
	S_Ship(D3DXVECTOR3, Quaternion, int);

	// Methods
	void init();
	void addPlayerInput(InputState);
	void calcTractorBeam();

	virtual D3DXVECTOR3 calculateRotationalInertia(float mass);
	bool interact(S_Resource *);
	void interact(S_Asteroid *);
	void interact(S_Ship *);

	

};

#pragma warning( pop )

#endif S_SHIP_H_INCLUDED