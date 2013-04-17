/*
 * Ship.cpp
 */

#include <shared/game/Ship.h>

Ship::Ship() : 
	m_playerNum(0),
	m_tractorBeamOn(false)
{
}

Ship::Ship(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int pNum, bool tBeamOn) :
	Entity(pos, dir),
	m_playerNum(pNum),
	m_tractorBeamOn(tBeamOn)
{
}

const char* Ship::encode() const {
	char *rtn = new char[m_size];
	const char *tmp = Entity::encode();
	memcpy(rtn, tmp, Entity::size());
	*(ENUM_TYPE*) rtn = SHIP;
	char* tmp_rtn = rtn;
	tmp_rtn += Entity::size();
	*(SHIP_PLAYERNUM_TYPE*) (tmp_rtn) = m_playerNum;
	tmp_rtn += sizeof(SHIP_PLAYERNUM_TYPE);
	*(bool *) (tmp_rtn) = m_tractorBeamOn;
	delete tmp;
	return rtn;
}

ostream& operator<<(ostream& os, const Ship& e) {
	os << e.getID() << " " << e.m_pos.x << " " << e.m_pos.y << " " << e.m_pos.z
		<< " " << e.m_dir.x << " " << e.m_dir.y << " " << e.m_dir.z
		<< " " << e.m_playerNum << " " << e.m_tractorBeamOn;
	return os;
}

void Ship::decode(const char *buff) {
//	Entity e;
	Entity::decode(buff);
	m_type = SHIP;
	buff += Entity::size();
	m_playerNum = *(SHIP_PLAYERNUM_TYPE*) buff;
	buff += sizeof(SHIP_PLAYERNUM_TYPE);
	m_tractorBeamOn = *(bool *) buff;
}

void Ship::update(Entity * source) {
	Ship * srcShip = dynamic_cast<Ship*>(source);
	if (srcShip == 0) {
#ifdef _DEBUG
		MessageBox( NULL, L"Error converting Entity to Ship", L"CRUSH Game", MB_OK );
#endif
	} else {
		m_pos = srcShip->m_pos;
		m_dir = srcShip->m_dir;
		m_tractorBeamOn = srcShip->m_tractorBeamOn;
		m_playerNum = srcShip -> m_playerNum;
	}
}