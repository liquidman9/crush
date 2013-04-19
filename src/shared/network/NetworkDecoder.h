#pragma once

// Global includes
#include <vector>

// Project includes
#include <shared/game/Entity.h>
#include <shared/game/Ship.h>
#include <shared/network/Network.h>
#include <shared/InputState.h>

typedef Sendable Event;

class NetworkDecoder
{
private:
	unsigned int m_size;
	const char *m_head;
	//const char *m_curr_head;

public:
	NetworkDecoder(const char *head, unsigned int size) {
		char * tmp = new char[size];
		m_head = tmp;
		m_size = size;
		//m_curr_head = m_head;
		memcpy(tmp, head, size);
	};
	
	//this is where the magic happens
	void decodeGameState(GameState &g) {
		Entity* ep = NULL;
		const char* curr_head = m_head;
		for(unsigned int size = 0; size < m_size; size += ep->size() ){
			ENUM_TYPE a = *(ENUM_TYPE*) m_head;
			switch(*(ENUM_TYPE*) m_head) {
			// We should not be sending any "plain entities".
			/*case ENTITY:
				ep = new Entity();
				ep->decode(curr_head + size);
				g.push_back(ep);
				break;*/
			case SHIP:
				ep = new Ship();
				ep->decode(curr_head + size);
				g.push_back(ep);
				break;
			}
		}
	};

	//this is where the magic happens
	void decodeEvents(map<unsigned int, shared_ptr<Event> > &g, unsigned int client) {
		Event* ep = NULL;
		const char* curr_head = m_head;
		for(unsigned int size = 0; size < m_size; size += ep->size() ){
			/*switch(*(ENUM_TYPE*) m_head) {
			case ENTITY:*/
				ep = new InputState();
				ep->decode(curr_head);
				g[client] = shared_ptr<Event>(ep);
				break;
			/*}*/
		}	
	}
	virtual ~NetworkDecoder(void) {
		delete m_head;
	}
};

