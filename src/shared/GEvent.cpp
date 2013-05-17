/*
* Event.cpp
*/

// Project includes
#include <shared/GEvent.h>

GEvent::GEvent(Type type) :
	m_type(type)
{ }

unsigned int GEvent::encode(char *tmp) const {
	send_struct s;
	s.type = m_type;

	//char* tmp = new char[sizeof(send_struct)];
	memcpy(tmp, (const char *) &s, sizeof(send_struct));

	return sizeof(send_struct);
}

unsigned int GEvent::decode(const char * tmp) {
	send_struct s;
	memcpy((char *) &s, tmp, sizeof(send_struct));

	return sizeof(send_struct);
}

GEvent::~GEvent(){
}