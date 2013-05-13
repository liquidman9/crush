/*
 * Mothership.cpp
 */

// Project includes
#include <shared/game/Mothership.h>

Mothership::Mothership() :
	Entity(MOTHERSHIP),
	m_playerNum(0),
	m_resources(0)
{
}

Mothership::Mothership(int pNum) :
	Entity(MOTHERSHIP),
	m_playerNum(pNum),
	m_resources(0)
{
}


unsigned int Mothership::encode(char *head) const {
	// Get entity encode
	unsigned int rtn = Entity::encode(head);

	// Encode Scale
	*(PLAYERNUM_TYPE *) (head + rtn) = m_playerNum;
	rtn += sizeof(PLAYERNUM_TYPE);

	*(int *) (head + rtn) = m_resources;
	rtn += sizeof(int);

	return rtn;
}

ostream& operator<<(ostream& os, const Mothership& e) {
	os << e.getID() << " " << e.m_pos.x << " " << e.m_pos.y << " " << e.m_pos.z
		<< " " << e.m_orientation.x << " " << e.m_orientation.y << " " << e.m_orientation.z << " " << e.m_orientation.w
		<< " " << e.m_playerNum << " " << e.m_resources;
	return os;
}

unsigned int Mothership::decode(const char *buff) {
	unsigned int rtn = Entity::decode(buff);
	m_playerNum = *(PLAYERNUM_TYPE*) (buff+rtn);
	rtn += sizeof(PLAYERNUM_TYPE);
	m_resources = *(int*) (buff+rtn);
	rtn += sizeof(int);
	return rtn;
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