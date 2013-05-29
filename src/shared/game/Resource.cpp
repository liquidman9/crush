/*
 * Resource.cpp
 */

// Project includes
#include <shared/game/Resource.h>

Resource::Resource() :
	Entity(RESOURCE)
{
}


unsigned int Resource::encode(char *head) const {
	// Get entity encode
	unsigned int rtn = Entity::encode(head);
	*(bool*) (head + rtn) = m_enableIdentifiers;
	rtn += sizeof(bool);


	return rtn;
}

ostream& operator<<(ostream& os, const Resource& e) {
	os << e.getID() << " " << e.m_pos.x << " " << e.m_pos.y << " " << e.m_pos.z
		<< " " << e.m_orientation.x << " " << e.m_orientation.y << " " << e.m_orientation.z << " " << e.m_orientation.w;
	return os;
}

unsigned int Resource::decode(const char *buff) {
	auto rtn = Entity::decode(buff);
	m_enableIdentifiers = *(bool*) (buff + rtn);
	rtn += sizeof(bool);
	return rtn;
}


void Resource::update(shared_ptr<Entity> sp_source) {
	Entity *source = sp_source.get();
	Resource * srcResource = dynamic_cast<Resource*>(source);
	if (srcResource == 0) {
		//TODO figure out how to handle this case when not in debug
#ifdef _DEBUG
//TODO put this back in, make sure it doesn't break anyone's build
//		MessageBox( NULL, L"Error converting Entity to Ship", L"CRUSH Game", MB_OK );
#endif
	} else {
		Entity::update(sp_source);
		m_enableIdentifiers = srcResource->m_enableIdentifiers;
	}
}