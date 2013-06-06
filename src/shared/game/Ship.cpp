/*
 * Ship.cpp
 */

// Project includes
#include <shared/game/Ship.h>

Ship::Ship() :
	Entity(SHIP),
	m_playerNum(0),
	m_thruster(0),
	m_hasPowerup(false),
	m_powerupType(SPEEDUP),
	m_powerupStateType(WAITING),
	m_isLockOnTarget(false),
	m_reverse(false)
{
}

Ship::Ship(int pNum) :
	Entity(SHIP),
	m_playerNum(pNum),
	m_playerName("Player " + to_string((long long) pNum)),
	m_thruster(0),
	m_hasPowerup(false),
	m_powerupType(SPEEDUP),
	m_powerupStateType(WAITING),
	m_isLockOnTarget(false),
	m_reverse(false)
{}

void Ship::setPlayerName(const string &s) {
	m_playerName = s;
	m_playerName.resize(MAX_PLAYERNAME_SIZE);
}


unsigned int Ship::encode(char *head) const {
	// Get entity encode
	unsigned int rtn = Entity::encode(head);

	*(bool*) (head + rtn) = m_hasPowerup;
	rtn += sizeof(bool);

	*(PowerType*) (head + rtn) = m_powerupType;
	rtn += sizeof(PowerType);

	*(StateType*) (head + rtn) = m_powerupStateType;
	rtn += sizeof(StateType);
	
	// Encode playernum
	*(SHIP_PLAYERNUM_TYPE*) (head + rtn) = m_playerNum;
	rtn += sizeof(SHIP_PLAYERNUM_TYPE);
	
	// Encode tractor beam
	*(double *) (head + rtn) = m_thruster;
	rtn += sizeof(double);

	memcpy(head + rtn, m_playerName.c_str(), MAX_PLAYERNAME_SIZE);
	rtn += MAX_PLAYERNAME_SIZE;

	*(bool*) (head + rtn) = m_useAltSprite;
	rtn += sizeof(bool);

	*(bool*) (head + rtn) = m_isLockOnTarget;
	rtn += sizeof(bool);

	*(bool*) (head + rtn) = m_reverse;
	rtn += sizeof(bool);

	return rtn;
}

ostream& operator<<(ostream& os, const Ship& e) {
	os << e.getID() << " " << e.m_pos.x << " " << e.m_pos.y << " " << e.m_pos.z
		<< " " << e.m_orientation.x << " " << e.m_orientation.y << " " << e.m_orientation.z << " " << e.m_orientation.w
		<< " " << e.m_playerNum << " " << e.m_thruster;
	return os;
}

unsigned int Ship::decode(const char *buff) {
	unsigned int rtn = Entity::decode(buff);
	//m_type = SHIP;
	m_hasPowerup = *(bool*) (buff+rtn);
	rtn += sizeof(bool);
	m_powerupType = *(PowerType*) (buff+rtn);
	rtn += sizeof(PowerType);
	m_powerupStateType = *(StateType*) (buff+rtn);
	rtn += sizeof(StateType);
	m_playerNum = *(SHIP_PLAYERNUM_TYPE*) (buff+rtn);
	rtn += sizeof(SHIP_PLAYERNUM_TYPE);
	m_thruster = *(double *) (buff+rtn);
	rtn += sizeof(double);
	m_playerName = string(buff+rtn, MAX_PLAYERNAME_SIZE);
	rtn += MAX_PLAYERNAME_SIZE;
	m_useAltSprite = *(bool*) (buff + rtn);
	rtn += sizeof(bool);
	m_isLockOnTarget = *(bool*) (buff + rtn);
	rtn += sizeof(bool);
	m_reverse = *(bool*) (buff + rtn);
	rtn += sizeof(bool);

	return rtn;
}

//void Ship::update(Entity * source) {
//	Ship * srcShip = dynamic_cast<Ship*>(source);
//	if (srcShip == 0) {
//#ifdef _DEBUG
////		MessageBox( NULL, L"Error converting Entity to Ship", L"CRUSH Game", MB_OK );
//#endif
//	} else {
//		m_pos = srcShip->m_pos;
//		m_dir = srcShip->m_dir;
//		m_tractorBeamOn = srcShip->m_tractorBeamOn;
//		m_playerNum = srcShip -> m_playerNum;
//	}
//}

void Ship::update(shared_ptr<Entity> sp_source) {
	Entity *source = sp_source.get();
	Ship * srcShip = dynamic_cast<Ship*>(source);
	if (srcShip == 0) {
		//TODO figure out how to handle this case when not in debug
#ifdef _DEBUG
//TODO put this back in, make sure it doesn't break anyone's build
//		MessageBox( NULL, L"Error converting Entity to Ship", L"CRUSH Game", MB_OK );
#endif
	} else {
		Entity::update(sp_source);
		m_thruster = srcShip->m_thruster;
		m_playerNum = srcShip -> m_playerNum;
		m_hasPowerup = srcShip->m_hasPowerup;
		m_powerupType = srcShip->m_powerupType;
		m_powerupStateType = srcShip->m_powerupStateType;
		m_useAltSprite = srcShip->m_useAltSprite;
		m_isLockOnTarget = srcShip->m_isLockOnTarget;
	}
}