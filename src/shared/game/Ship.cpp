/*
 * Ship.cpp
 */

// Project includes
#include <shared/game/Ship.h>

Ship::Ship() :
	Entity(SHIP),
	m_playerNum(0),
	m_tractorBeamOn(false)
{
}

Ship::Ship(int pNum) :
	Entity(SHIP),
	m_playerNum(pNum),
	m_tractorBeamOn(false),
	m_playerName("Player " + to_string((long long) pNum))
{}

void Ship::setPlayerName(const string &s) {
	m_playerName = s;
	m_playerName.resize(MAX_PLAYERNAME_SIZE);
}


const char* Ship::encode() const {
	// Declare ret
	char *rtn = new char[m_size];

	// Get entity encode
	const char *tmp = Entity::encode();

	// Copy entity encode into this ret
	memcpy(rtn, tmp, Entity::size());

	//*(ENUM_TYPE*) rtn = SHIP; THIS SHOULD BE SET IN ENTITY

	// Set up temp buffer at the end of entity encoding
	char* tmp_rtn = rtn;
	tmp_rtn += Entity::size();

	// Encode playernum
	*(SHIP_PLAYERNUM_TYPE*) (tmp_rtn) = m_playerNum;
	tmp_rtn += sizeof(SHIP_PLAYERNUM_TYPE);
	
	// Encode tractor beam
	*(bool *) (tmp_rtn) = m_tractorBeamOn;
	tmp_rtn += sizeof(bool);

	memcpy(tmp_rtn, m_playerName.c_str(), MAX_PLAYERNAME_SIZE);

	// Delete temp pointer
	delete []tmp;

	return rtn;
}

ostream& operator<<(ostream& os, const Ship& e) {
	os << e.getID() << " " << e.m_pos.x << " " << e.m_pos.y << " " << e.m_pos.z
		<< " " << e.m_orientation.x << " " << e.m_orientation.y << " " << e.m_orientation.z << " " << e.m_orientation.w
		<< " " << e.m_playerNum << " " << e.m_tractorBeamOn;
	return os;
}

void Ship::decode(const char *buff) {
	Entity::decode(buff);
	//m_type = SHIP;
	buff += Entity::size();
	m_playerNum = *(SHIP_PLAYERNUM_TYPE*) buff;
	buff += sizeof(SHIP_PLAYERNUM_TYPE);
	m_tractorBeamOn = *(bool *) buff;
	buff += sizeof(bool);
	m_playerName = string(buff, MAX_PLAYERNAME_SIZE);
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
		m_tractorBeamOn = srcShip->m_tractorBeamOn;
		m_playerNum = srcShip -> m_playerNum;
	}
}