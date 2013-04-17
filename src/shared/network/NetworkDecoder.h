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

	void decodeGameState(GameState &g) {
		Entity* ep = NULL;
		const char* curr_head = m_head;
		for(unsigned int size = 0; size < m_size; size += ep->size() ){
			ENUM_TYPE a = *(ENUM_TYPE*) m_head;
			switch(*(ENUM_TYPE*) m_head) {
			case ENTITY:
				ep = new Entity();
				ep->decode(curr_head + size);
				g.push_back(ep);
				break;
			case SHIP:
				ep = new Ship();
				ep->decode(curr_head + size);
				g.push_back(ep);
				break;
			}
		}
	};

	void decodeEvents(vector<Event*> &g) {
		Event* ep = NULL;
		const char* curr_head = m_head;
		for(unsigned int size = 0; size < m_size; size += ep->size() ){
			/*switch(*(ENUM_TYPE*) m_head) {
			case ENTITY:*/
				ep = new InputState();
				ep->decode(curr_head);
				g.push_back(ep);
				break;
			/*}*/
		}
		
	}
	virtual ~NetworkDecoder(void) {
		delete m_head;
	}
};

