/*
 * S_Extractor.hpp
 */

#ifndef S_EXTRACTOR_H_INCLUDED
#define S_EXTRACTOR_H_INCLUDED

// Global includes
#include <d3dx9.h>

// Project includes
#include <shared/game/Extractor.h> 
#include <server/game/ServerEntity.h> 
#include <server/game/S_Resource.h> 

#pragma warning( push )
#pragma warning( disable : 4250 )

class S_Extractor : public Extractor, public ServerEntity {
private:
	S_Resource * m_cur_resource;
	float m_timer;
	bool respawned;

public:
	// Constructors
	S_Extractor(D3DXVECTOR3, Quaternion);

	// Methods
	S_Resource * getResource();
	void respawn();

	virtual D3DXMATRIX calculateRotationalInertia(float mass);
	virtual void update(float delta_time);
};



#pragma warning( pop )

#endif S_EXTRACTOR_H_INCLUDED