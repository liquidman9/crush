/*
 * Asteroid.cpp
 */

// Project includes
#include <shared/game/Asteroid.h>

Asteroid::Asteroid() :
	Entity(ASTEROID),
	m_scale(0)
{
}

Asteroid::Asteroid(float scale) :
	Entity(ASTEROID),
	m_scale(scale)
{
}


unsigned int Asteroid::encode(char *head) {
	// Get entity encode
	unsigned int rtn = Entity::encode(head);

	// Encode Scale
	*(float *) (head+rtn) = m_scale;
	rtn += sizeof(float);

#ifdef ENABLE_DELTA
	char tmp [m_size + MAX_ENTITY_SIZE];
	rtn = encodeDelta(tmp, head, rtn);
	memcpy(head, tmp, rtn);
#endif

	return rtn;
}

ostream& operator<<(ostream& os, const Asteroid& e) {
	os << e.getID() << " " << e.m_pos.x << " " << e.m_pos.y << " " << e.m_pos.z
		<< " " << e.m_orientation.x << " " << e.m_orientation.y << " " << e.m_orientation.z << " " << e.m_orientation.w
		<< " " << e.m_scale;
	return os;
}

unsigned int Asteroid::decode(char *buff) {
	unsigned int actual_rtn = Entity::decode(buff);
#ifdef ENABLE_DELTA
	unsigned int rtn = Entity::size();
	buff = m_oldState;
#else
	unsigned int rtn = actual_rtn;
#endif

	m_scale = *(float*) (buff+rtn);
	rtn += sizeof(float);


#ifdef ENABLE_DELTA
	return actual_rtn;
#else
	return rtn;
#endif

}


void Asteroid::update(shared_ptr<Entity> sp_source) {
	Entity *source = sp_source.get();
	Asteroid * srcAsteroid = dynamic_cast<Asteroid*>(source);
	if (srcAsteroid == 0) {
		//TODO figure out how to handle this case when not in debug
#ifdef _DEBUG
//TODO put this back in, make sure it doesn't break anyone's build
//		MessageBox( NULL, L"Error converting Entity to Ship", L"CRUSH Game", MB_OK );
#endif
	} else {
		Entity::update(sp_source);
		m_scale = srcAsteroid->m_scale;
	}
}