/*
 * Powerup.cpp
 */

// Project includes
#include <shared/game/Powerup.h>

Powerup::Powerup() :
	Entity(POWERUP),
	m_powerType(SPEEDUP),
	m_stateType(WAITING),
	m_playerNum(-1)
{
}


Powerup::Powerup(PowerType type) :
	Entity(POWERUP),
	m_powerType(type),
	m_stateType(WAITING),
	m_playerNum(-1)
{
}


unsigned int Powerup::encode(char *head) const {
	// Get entity encode
	unsigned int rtn = Entity::encode(head);

	*(POWER_TYPE*) (head + rtn) = m_powerType;
	rtn += sizeof(POWER_TYPE);
	*(STATE_TYPE*) (head + rtn) = m_stateType;
	rtn += sizeof(STATE_TYPE);
	*(SHIP_PLAYERNUM_TYPE*) (head + rtn) = m_playerNum;
	rtn += sizeof(SHIP_PLAYERNUM_TYPE);

	return rtn;
}

ostream& operator<<(ostream& os, const Powerup& e) {
	os << e.getID() << " " << e.m_pos.x << " " << e.m_pos.y << " " << e.m_pos.z
		<< " " << e.m_orientation.x << " " << e.m_orientation.y << " " << e.m_orientation.z << " " << e.m_orientation.w
		<< " "<<(int)e.m_powerType<<" "<<(int)e.m_stateType<<" "<<(int)e.m_playerNum;
	return os;
}

unsigned int Powerup::decode(const char *buff) {
	unsigned int rtn = Entity::decode(buff);
	m_powerType = *(PowerType*) (buff+rtn);
	rtn += sizeof(POWER_TYPE);
	m_stateType = *(StateType*) (buff+rtn);
	rtn += sizeof(STATE_TYPE);
	m_playerNum = *(SHIP_PLAYERNUM_TYPE*) (buff+rtn);
	rtn += sizeof(SHIP_PLAYERNUM_TYPE);
	return rtn;
}


void Powerup::update(shared_ptr<Entity> sp_source) {
	Entity *source = sp_source.get();
	Powerup * srcPowerup = dynamic_cast<Powerup*>(source);
	if (srcPowerup == 0) {
		//TODO figure out how to handle this case when not in debug
#ifdef _DEBUG
//TODO put this back in, make sure it doesn't break anyone's build
//		MessageBox( NULL, L"Error converting Entity to Powerup", L"CRUSH Game", MB_OK );
#endif
	} else {
		Entity::update(sp_source);
		m_powerType = srcPowerup->m_powerType;
		m_stateType = srcPowerup->m_stateType;
		m_playerNum = srcPowerup->m_playerNum;
	}
}