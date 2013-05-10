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

namespace server {
	namespace entities {
		namespace asteroid {
			static const string CONFIG_PREFIX = "asteroid_";

			extern float startMass;
			extern int rangeMass;
			extern float startPos;
			extern int rangePos;
			extern float scaleToRadius;
			extern float radiusToMass;
			extern int numAsteroids;

			inline void initFromConfig() {
				ConfigSettings::config->getValue(CONFIG_PREFIX + "numAsteroids", numAsteroids);
				ConfigSettings::config->getValue(CONFIG_PREFIX + "startMass", startMass);
				ConfigSettings::config->getValue(CONFIG_PREFIX + "rangeMass", rangeMass);
				ConfigSettings::config->getValue(CONFIG_PREFIX + "startPos", startPos);
				ConfigSettings::config->getValue(CONFIG_PREFIX + "rangePos", rangePos);
				ConfigSettings::config->getValue(CONFIG_PREFIX + "scaleToRadius", scaleToRadius);
				ConfigSettings::config->getValue(CONFIG_PREFIX + "radiusToMass", radiusToMass);
			}
		}
	}
}


#pragma warning( push )
#pragma warning( disable : 4250 )

class S_Asteroid : public Asteroid, public ServerEntity{ //switch back to capsule

public:

	// Fields

	// Constructors
	S_Asteroid();
	S_Asteroid(D3DXVECTOR3, Quaternion, float);

	// Methods
	virtual D3DXVECTOR3 calculateRotationalInertia(float mass);
};

#pragma warning( pop )

#endif S_ASTEROID_H_INCLUDED