/*
 * CollisionEvent.cpp
 */

// Project includes
#include <shared/CollisionGEvent.h>


CollisionGEvent::CollisionGEvent() :
	GEvent(COLLISIONEVENT),
	m_ctype(C),
	m_idA(0),
	m_idB(0),
	m_poi(D3DXVECTOR3(0.0, 0.0, 0.0)),
	m_impulse(0.0),
	m_player1(-1),
	m_player2(-1)
{}


CollisionGEvent::CollisionGEvent(int id_a, int id_b, D3DXVECTOR3 poi, float impulse, CType ctype, int p1, int p2) :
	GEvent(COLLISIONEVENT),
	m_idA(id_a),
	m_idB(id_b),
	m_poi(poi),
	m_impulse(impulse),
	m_ctype(ctype),
	m_player1(p1),
	m_player2(p2)
{}

unsigned int CollisionGEvent::encode(char *tmp) const {
	send_struct s;
	s.type = m_type;
	s.ctype = m_ctype;
	s.id_a = m_idA;
	s.id_b = m_idB;
	s.poi = m_poi;
	s.impulse = m_impulse;
	s.m_player1 = m_player1;
	s.m_player2 = m_player2;

	//char* tmp = new char[sizeof(send_struct)];
	memcpy(tmp, (const char *) &s, sizeof(send_struct));

	return sizeof(send_struct);
}

unsigned int CollisionGEvent::decode(const char * tmp) {
	send_struct s;
	memcpy((char *) &s, tmp, sizeof(send_struct));
	m_ctype = s.ctype;
	m_idA = s.id_a;
	m_idB = s.id_b;
	m_poi = s.poi;
	m_impulse = s.impulse;
	m_player1 = s.m_player1;
	m_player2 = s.m_player2;

	return sizeof(send_struct);
}
