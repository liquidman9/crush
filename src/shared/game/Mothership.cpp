/*
 * Mothership.cpp
 */

// Project includes
#include <shared/game/Mothership.h>


Mothership::Mothership(int pNum) :
	Entity(MOTHERSHIP),
	m_playerNum(pNum),
	m_resources(0)
{
}


const char* Mothership::encode() const {
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
	*(PLAYERNUM_TYPE *) (tmp_rtn) = m_playerNum;
	tmp_rtn += sizeof(PLAYERNUM_TYPE);

	*(int *) (tmp_rtn) = m_resources;

	return rtn;
}

ostream& operator<<(ostream& os, const Mothership& e) {
	os << e.getID() << " " << e.m_pos.x << " " << e.m_pos.y << " " << e.m_pos.z
		<< " " << e.m_orientation.x << " " << e.m_orientation.y << " " << e.m_orientation.z << " " << e.m_orientation.w
		<< " " << e.m_playerNum << " " << e.m_resources;
	return os;
}

void Mothership::decode(const char *buff) {
	Entity::decode(buff);
	m_type = MOTHERSHIP;
	buff += Entity::size();
	m_playerNum = *(PLAYERNUM_TYPE*) buff;
	buff += sizeof(PLAYERNUM_TYPE);
	m_resources = *(int*) buff;
}


void Mothership::update(shared_ptr<Entity> sp_source) {
	Entity *source = sp_source.get();
	Mothership * srcMothership = dynamic_cast<Mothership*>(source);
	if (srcMothership == 0) {
		//TODO figure out how to handle this case when not in debug
#ifdef _DEBUG
//TODO put this back in, make sure it doesn't break anyone's build
//		MessageBox( NULL, L"Error converting Entity to Ship", L"CRUSH Game", MB_OK );
#endif
	} else {
		Entity::update(sp_source);
		m_playerNum = srcMothership->m_playerNum;
		m_resources = srcMothership->m_resources;
	}
}