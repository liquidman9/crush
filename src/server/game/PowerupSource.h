/*
 * PowerupSource.hpp
 */

#ifndef POWERUPSOURCE_H_INCLUDED
#define POWERUPSOURCE_H_INCLUDED

// Global includes
#include <d3dx9.h>
#include <map>

// Project includes
#include <server/game/ServerEntity.h>
#include <server/game/S_Powerup.h>

#define SPAWN_MIN_TIME 30000

#pragma warning( push )
#pragma warning( disable : 4250 )

class PowerupSource {
public:

	vector<pair<S_Powerup *, D3DXVECTOR3>> m_spawnPoints;
	vector<long long> m_startTimes;

	vector<S_Powerup *> m_powerups;

	
	PowerupSource(vector<D3DXVECTOR3>, long long);
	void update(long long);
};

#pragma warning( pop )

#endif POWERUPSOURCE_H_INCLUDED