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
	Entity(genId(), POWERUP, pos, orientation, 3),
	Powerup(type),
	ServerEntity(10, 1.0, calculateRotationalInertia(10)),
	m_holder(NULL),
	m_totalTimeLength(7000),
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
	if(time - m_startTime >= m_totalTimeLength) return true;
	else return false;
}

void S_Powerup::pickUp(S_Ship * ship) {
	m_pos = D3DXVECTOR3(1000000,0,0);//tmp
	m_holder = ship;
	m_playerNum = ship->m_playerNum;
	m_stateType = HOLDING;

	cout<<"Player "<<(int)m_playerNum<<" picked up a type "<<(int)m_powerType<<" powerup."<<endl;
}

void S_Powerup::start() {
	m_startTime = GetTickCount();
	m_playerNum = m_holder->m_playerNum;
	m_stateType = CONSUMED;

	cout<<"Player "<<(int)m_playerNum<<" used a type "<<(int)m_powerType<<" powerup."<<endl;

	switch(m_powerType){
	case SPEEDUP:
		m_holder->setFowardImpulse( m_holder->getForwardImpulse()*m_impulseRate);
		m_holder->setMaxVelocity(m_holder->getMaxVelocity()*m_maxVelocityRate);
		break;
	case PULSE:
		break;
	case FIELD:
		break;
	case SHIELD:
		break;
	default:
		break;
	}
}

void S_Powerup::end() {
	cout<<"Player "<<(int)m_playerNum<<"'s "<<(int)m_powerType<<" powerup ended."<<endl;

	switch(m_powerType){
	case SPEEDUP:
		m_holder->setFowardImpulse(m_holder->getForwardImpulse()/m_impulseRate);
		m_holder->setMaxVelocity(m_holder->getMaxVelocity()/m_maxVelocityRate);
		break;
	case PULSE:
		break;
	case FIELD:
		break;
	case SHIELD:
		break;
	default:
		break;
	}

	m_holder->m_powerup = NULL;
	m_holder = NULL;
	m_destroy = true;
	m_playerNum = -1;
	m_startTime = GetTickCount();
	m_stateType = WAITING;
}

void S_Powerup::update(float delta_time){
	
	ServerEntity::update(delta_time);

}