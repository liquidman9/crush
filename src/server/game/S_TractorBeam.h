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


namespace server {
	namespace entities {
		namespace tractorbeam {
			static const string CONFIG_PREFIX = "tractorbeam_";

			extern float gravity;
			extern float length;

			inline void initFromConfig() {
				ConfigSettings::config->getValue(CONFIG_PREFIX + "gravity", gravity);
				ConfigSettings::config->getValue(CONFIG_PREFIX + "length", length);
			}
		}
	}
}


class S_TractorBeam : public TractorBeam, public ServerEntity{ 

public:
	// Fields
	float m_gravity;

	ServerEntity * m_object;
	S_Ship * m_ship; 
	float m_strength;
	bool m_isPulling;

	// Constructors
	S_TractorBeam(S_Ship *);

	// Methods
	bool isLocked();
	float getCurrentDistance();
	D3DXVECTOR3 getCurrentDirection();
	D3DXVECTOR3 getCurrentDistanceVector();
	D3DXVECTOR3 getDistanceVectorOf(D3DXVECTOR3);
	void setStartPoint();
	void setEndPoint();
	void lockOn(ServerEntity * entity);
	
	// For applying impulses, updating data, etc
	void updateData();

	void calculateForce();
	
	// Overwrite, since doesnt do anything during normal cycle
	void update(float delta_time);

	virtual D3DXVECTOR3 calculateRotationalInertia(float mass);
};

#pragma warning( pop )

#endif S_TRACTORBEAM_H_INCLUDED