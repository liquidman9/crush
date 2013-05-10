/*
 * Extractor.cpp
 */
 
// Project includes
#include <shared/game/Extractor.h>

Extractor::Extractor() :
	Entity(EXTRACTOR)
{	
}


const char* Extractor::encode() const {
	// Declare ret
	char *rtn = new char[m_size];

	// Get entity encode
	const char *tmp = Entity::encode();

	// Copy entity encode into this ret
	memcpy(rtn, tmp, Entity::size());

	//always delete the encode buffer
	delete []tmp;


	return rtn;
}

ostream& operator<<(ostream& os, const Extractor& e) {
	os << e.getID() << " " << e.m_pos.x << " " << e.m_pos.y << " " << e.m_pos.z
		<< " " << e.m_orientation.x << " " << e.m_orientation.y << " " << e.m_orientation.z << " " << e.m_orientation.w;
	return os;
}

void Extractor::decode(const char *buff) {
	Entity::decode(buff);
	m_type = EXTRACTOR;
}


void Extractor::update(shared_ptr<Entity> sp_source) {
	Entity *source = sp_source.get();
	Extractor * srcExtractor = dynamic_cast<Extractor*>(source);
	if (srcExtractor == 0) {
		//TODO figure out how to handle this case when not in debug
#ifdef _DEBUG
//TODO put this back in, make sure it doesn't break anyone's build
//		MessageBox( NULL, L"Error converting Entity to Ship", L"CRUSH Game", MB_OK );
#endif
	} else {
		Entity::update(sp_source);
	}
}