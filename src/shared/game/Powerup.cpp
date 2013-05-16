/*
 * Powerup.cpp
 */

// Project includes
#include <shared/game/Powerup.h>

Powerup::Powerup() :
	Entity(POWERUP)
{
}


unsigned int Powerup::encode(char *head) const {
	// Get entity encode
	unsigned int rtn = Entity::encode(head);

	return rtn;
}

ostream& operator<<(ostream& os, const Powerup& e) {
	os << e.getID() << " " << e.m_pos.x << " " << e.m_pos.y << " " << e.m_pos.z
		<< " " << e.m_orientation.x << " " << e.m_orientation.y << " " << e.m_orientation.z << " " << e.m_orientation.w;
	return os;
}

unsigned int Powerup::decode(const char *buff) {
	return Entity::decode(buff);
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
	}
}