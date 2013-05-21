/*
 * PowerupSource.cpp
 */
 
// Project includes
#include <server/game/PowerupSource.h>

PowerupSource::PowerupSource(vector<D3DXVECTOR3> points, long long time) 
{	
	srand((unsigned int)time);
	D3DXVECTOR3 m_pos1(10000000,0,0); // tmp
	Quaternion m_dir1(0.0, 0.0, 0.0, 1.0);
	unsigned int i;
	for(i = 0; i < points.size(); i++) {
		m_powerups.push_back(new S_Powerup(m_pos1, m_dir1, SPEEDUP));//(PowerType)(rand()%4)));
		m_spawnPoints.push_back(pair<S_Powerup *, D3DXVECTOR3>(m_powerups[i],points[i]));
		m_startTimes.push_back(time);

		m_powerups[i]->m_stateType = SPAWNED;
		m_powerups[i]->m_pos = m_spawnPoints[i].second;
	}

	for(int j = 0; j < 2; j++) {
		m_powerups.push_back(new S_Powerup(m_pos1, m_dir1, SPEEDUP));// (PowerType)(rand()%4)));
		m_powerups[i+j]->m_stateType = WAITING;
	}

	// add extra powerups if more entities allowed than spawn points
}


void PowerupSource::update(long long time) {
	bool found = false;
	for(unsigned int i = 0; i < m_spawnPoints.size(); i++) {
		if(m_spawnPoints[i].first != NULL && m_spawnPoints[i].first->m_stateType != SPAWNED) {
			cout<< "Powerup Released"<<endl;
			m_startTimes[i] = time; // reset timer
			m_spawnPoints[i].first = NULL;
		}
		else if(m_spawnPoints[i].first == NULL && time - m_startTimes[i] >= SPAWN_MIN_TIME){
			for(unsigned int j = 0; j < m_powerups.size(); j++) {
				switch(m_powerups[j]->m_stateType){
					case WAITING:
						cout<< "Powerup Replaced"<<endl;
						m_spawnPoints[i].first = m_powerups[j];
						m_powerups[j]->m_pos = m_spawnPoints[i].second;
						m_powerups[j]->m_stateType = SPAWNED;

						found = true;
						break;
					default :
						break;
				}

				if(found) break;
			}
		}
	}
	
}
