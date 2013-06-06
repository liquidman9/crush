/*
 * S_Powerup.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Powerup.h>


using namespace server::entities::powerup;

S_Powerup::S_Powerup(D3DXVECTOR3 pos, Quaternion orientation, PowerType type) :
	Entity(genId(), POWERUP, pos, orientation, 20),
	Powerup(type),
	ServerEntity(10, 0.0, calculateRotationalInertia(10)),
	m_ship(NULL),
	m_totalTimeLength(0),
	m_startTime(0),
	m_impulseRate(impulse_rate),
	m_maxVelocityRate(max_velocity_rate)
{
}

D3DXMATRIX S_Powerup::calculateRotationalInertia(float mass){
	float radius_squared = 1;
	return *D3DXMatrixScaling(&D3DXMATRIX(), (2.0f / 5.0f) * mass * radius_squared,
											 (2.0f / 5.0f) * mass * radius_squared,
											 (2.0f / 5.0f) * mass * radius_squared);
};

bool S_Powerup::check(long time) {
	bool timeUp = false;
	switch(m_powerType){
	case SPEEDUP:
		if(time - m_startTime >= m_totalTimeLength) timeUp = true;
		else timeUp = false;
		break;
	case PULSE:
		if(time - m_startTime >= m_totalTimeLength) timeUp = true;
		else timeUp = false;
		break;
	case SHIELD:
		if(time - m_startTime >= m_totalTimeLength) timeUp = true;
		else timeUp = false;
		break;
	default:
		break;
	}
	return timeUp;
}

void S_Powerup::pickUp(S_Ship * ship) {
	m_pos = D3DXVECTOR3(1000000,0,0);//tmp
	m_ship = ship;
	m_playerNum = ship->m_playerNum;
	m_stateType = HOLDING;
	m_ship->m_hasPowerup = true;
	m_ship->m_powerupType = m_powerType;
	m_ship->m_powerupStateType = m_stateType;

	cout<<"Player "<<(int)m_playerNum<<" picked up a type "<<(int)m_powerType<<" powerup."<<endl;
}

void S_Powerup::start() {
	m_startTime = GetTickCount();
	m_playerNum = m_ship->m_playerNum;
	m_stateType = CONSUMED;
	m_ship->m_powerupStateType = m_stateType;

	cout<<"Player "<<(int)m_playerNum<<" used a type "<<(int)m_powerType<<" powerup."<<endl;

	switch(m_powerType){
	case SPEEDUP:
		m_totalTimeLength = speedup_time;
		m_ship->setFowardImpulse( m_ship->getForwardImpulse()*m_impulseRate);
		m_ship->setMaxVelocity(m_ship->getMaxVelocity()*m_maxVelocityRate);
		break;
	case PULSE:
		m_totalTimeLength = pulse_time;
		m_ship->m_pulseOn = true;
		break;
	case SHIELD:
		m_totalTimeLength = shield_time;
		m_ship->m_shieldOn = true;
		m_ship->m_radius = 7; //hardcoded same as shield on client
		m_ship->m_length = 0;
		m_ship->recalculateRelativeValues();
		break;
	default:
		break;
	}

	cout<<m_totalTimeLength<<endl;
}

void S_Powerup::end() {
	cout<<"Player "<<(int)m_playerNum<<"'s "<<(int)m_powerType<<" powerup ended."<<endl;

	switch(m_powerType){
	case SPEEDUP:
		m_ship->setFowardImpulse(m_ship->getForwardImpulse()/m_impulseRate);
		m_ship->setMaxVelocity(m_ship->getMaxVelocity()/m_maxVelocityRate);
		break;
	case PULSE:
		m_ship->m_pulseOn = false;
		break;
	case SHIELD:
		m_ship->m_shieldOn = false;
		m_ship->m_radius = m_ship->m_baseRadius;
		m_ship->m_length = m_ship->m_baseLength;
		m_ship->recalculateRelativeValues();
		break;
	default:
		break;
	}

	m_totalTimeLength = 0;
	m_ship->m_hasPowerup = false;
	m_ship->m_powerup = NULL;
	m_ship = NULL;
	m_destroy = true;
	m_playerNum = -1;
	m_startTime = GetTickCount();
	m_stateType = WAITING;

}

void S_Powerup::update(float delta_time){
	
	ServerEntity::update(delta_time);

}


void S_Powerup::pulseAll(vector<ServerEntity *> entities) {
	for(unsigned i = 0; i < entities.size(); i++)
	{	
		if(entities[i]->m_type == EXTRACTOR || 
		   entities[i]->m_type == POWERUP ||
		   entities[i]->m_type == RESOURCE ||
		   entities[i]->m_type == MOTHERSHIP ||
		   entities[i] == m_ship)
			continue;


		D3DXVECTOR3 disV;
		D3DXVECTOR3 range = entities[i]->m_pos - m_ship->m_pos;
		D3DXVec3Normalize(&disV, &range);
		float disL = D3DXVec3Length(&range);

		if(disL > pulse_range) continue;
		
		D3DXVECTOR3 force = (pulse_rate*m_ship->m_mass*entities[i]->m_mass)*(disV)/(pow(disL, 2));
		entities[i]->applyLinearImpulse(force);

		if(entities[i]->m_type == SHIP) {
			((S_Ship *)entities[i])->dropResource(2000);
		}
	}
}
