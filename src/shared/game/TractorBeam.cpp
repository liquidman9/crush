/*
 * TractorBeam.cpp
 */

// Project includes
#include <shared/game/TractorBeam.h>

TractorBeam::TractorBeam(int pNum) :
	Entity(TRACTORBEAM),
	m_playerNum(pNum)
{
}


const char* TractorBeam::encode() const {
	// Declare ret
	char *rtn = new char[m_size];

	// Get entity encode
	const char *tmp = Entity::encode();

	// Copy entity encode into this ret
	memcpy(rtn, tmp, Entity::size());

	//always delete the encode buffer
	delete []tmp;

	// Set up temp buffer at the end of entity encoding
	char* tmp_rtn = rtn;
	tmp_rtn += Entity::size();

	// Encode Scale
	*(SHIP_PLAYERNUM_TYPE *) (tmp_rtn) = m_playerNum;
	tmp_rtn += sizeof(SHIP_PLAYERNUM_TYPE);

	*(bool *) (tmp_rtn) = m_mode;
	tmp_rtn += sizeof(bool);

	*(int *) (tmp_rtn) = m_strength;

	return rtn;
}

ostream& operator<<(ostream& os, const TractorBeam& e) {
	os << e.getID() << " " << e.m_pos.x << " " << e.m_pos.y << " " << e.m_pos.z
		<< " " << e.m_orientation.x << " " << e.m_orientation.y << " " << e.m_orientation.z << " " << e.m_orientation.w
		<< " " << e.m_playerNum << " " << e.m_mode << " " << e.m_strength;
	return os;
}

void TractorBeam::decode(const char *buff) {
	Entity::decode(buff);
	m_type = ASTEROID;
	buff += Entity::size();
	m_playerNum = *(SHIP_PLAYERNUM_TYPE*) buff;
	buff += sizeof(SHIP_PLAYERNUM_TYPE);
	m_mode = *(bool*) buff;
	buff += sizeof(bool);
	m_strength = *(int*) buff;
}


void TractorBeam::update(shared_ptr<Entity> sp_source) {
	Entity *source = sp_source.get();
	TractorBeam * srcTractorBeam = dynamic_cast<TractorBeam*>(source);
	if (srcTractorBeam == 0) {
		//TODO figure out how to handle this case when not in debug
#ifdef _DEBUG
//TODO put this back in, make sure it doesn't break anyone's build
//		MessageBox( NULL, L"Error converting Entity to Ship", L"CRUSH Game", MB_OK );
#endif
	} else {
		Entity::update(sp_source);
		m_playerNum = srcTractorBeam->m_playerNum;
		m_mode = srcTractorBeam->m_mode;
		m_strength = srcTractorBeam->m_strength;
	}
}