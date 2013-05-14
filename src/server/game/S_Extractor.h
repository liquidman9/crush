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

class S_Extractor : public Extractor, public ServerEntity{
public:
	static const int s_msReset = 30000; 

	// Fields
	long long m_timer;
	S_Resource *m_ore;

	// Constructors
	S_Extractor(D3DXVECTOR3, Quaternion);

	// Methods
	void setStart(long long);
	bool checkRespawn(long long);
	S_Resource * respawn();
	S_Resource * transfer();

	virtual D3DXMATRIX calculateRotationalInertia(float mass);
	void update(float delta_time);
};



#pragma warning( pop )

#endif S_EXTRACTOR_H_INCLUDED