/*
 * CollisionEvent.cpp
 */

// Project includes
#include <shared/CollisionGEvent.h>

CollisionGEvent::CollisionGEvent() :
	GEvent(COLLISIONEVENT),
	m_idA(0),
	m_idB(0),
	m_poi(D3DXVECTOR3(0.0, 0.0, 0.0))
{}

CollisionGEvent::CollisionGEvent(int id_a, int id_b, D3DXVECTOR3 poi) :
	GEvent(COLLISIONEVENT),
	m_idA(id_a),
	m_idB(id_b),
	m_poi(poi)
{}

unsigned int CollisionGEvent::encode(char *tmp) const {
	send_struct s;
	s.type = m_type;
	s.id_a = m_idA;
	s.id_b = m_idB;
	s.poi = m_poi;

	//char* tmp = new char[sizeof(send_struct)];
	memcpy(tmp, (const char *) &s, sizeof(send_struct));

	return sizeof(send_struct);
}

unsigned int CollisionGEvent::decode(const char * tmp) {
	send_struct s;
	memcpy((char *) &s, tmp, sizeof(send_struct));
	m_idA = s.id_a;
	m_idB = s.id_b;
	m_poi = s.poi;

	return sizeof(send_struct);
}