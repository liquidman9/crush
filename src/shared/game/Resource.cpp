/*
 * Resource.cpp
 */

// Project includes
#include <shared/game/Resource.h>

Resource::Resource() :
	Entity(RESOURCE)
{
}


const char* Resource::encode() const {
	// Declare ret
	char *rtn = new char[m_size];

	// Get entity encode
	const char *tmp = Entity::encode();

	// Copy entity encode into this ret
	memcpy(rtn, tmp, Entity::size());

	// Set up temp buffer at the end of entity encoding
	char* tmp_rtn = rtn;
	tmp_rtn += Entity::size();
	return rtn;
}

ostream& operator<<(ostream& os, const Resource& e) {
	os << e.getID() << " " << e.m_pos.x << " " << e.m_pos.y << " " << e.m_pos.z
		<< " " << e.m_orientation.x << " " << e.m_orientation.y << " " << e.m_orientation.z << " " << e.m_orientation.w;
	return os;
}

void Resource::decode(const char *buff) {
	Entity::decode(buff);
	m_type = RESOURCE;
	buff += Entity::size();
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
	}
}