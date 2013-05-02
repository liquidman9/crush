/*
* GameState.h
*/

#pragma once


// Global includes
#include <vector>
#include <memory>
#include <assert.h>

// Project includes
#include <shared/game/Entity.h>
#include <shared/game/Ship.h>
#include <shared/game/TractorBeam.h>
#include <shared/game/Mothership.h>
#include <shared/game/Resource.h>
#include <shared/game/Asteroid.h>



template <class E>
class GameState
{
public:

	GameState(void):m_entities(){
		m_meta.size = sizeof(gameStateMeta);
	};

	GameState(GameState const & g): m_entities(g.m_entities), m_meta(g.m_meta) {
	};

	void push_back(E *e) {
		m_meta.size += e->size();
		m_entities.push_back(shared_ptr<E>(e));
	};

	unsigned int size() const {
		return m_entities.size();
	}

	unsigned int sendSize() const {
		return m_meta.size;
	}

	shared_ptr<E> operator[](unsigned int i) {
		return m_entities[i];
	}

	shared_ptr<const E> operator[](unsigned int i) const {
		return m_entities[i];
	}

	void clear() {
		m_entities.clear();
		m_meta.size = sizeof(gameStateMeta);
	}

	virtual ~GameState(void) {
		m_entities.clear();
	};

private:
	void decode(const char *head, const unsigned int size) {
		Entity* ep = NULL;		
		const char* cur_head = head + sizeof(m_meta);
		for(unsigned int cur_size = 0; cur_size < size; cur_size += m_meta.size){
			memcpy((char* ) &m_meta, head, sizeof(m_meta));
			unsigned int size1 = m_meta.size;
			clear();
			for(unsigned int cur_size = 0; cur_size < size1 - sizeof(m_meta); cur_size += ep->size() ){
				ep = NULL;
				Type a = *(Type*) (cur_head + cur_size);
				switch(a) {
				case SHIP: 
					ep = new C_Ship();
					ep->decode(cur_head + cur_size);
					this->push_back(ep);
					break;
				case ASTEROID:
					ep = new C_Asteroid();
					ep->decode(cur_head + cur_size);
					this->push_back(ep);
					break;
				case TRACTORBEAM:
					ep = new C_TractorBeam();
					ep->decode(cur_head + cur_size);
					this->push_back(ep);
					break;
				case MOTHERSHIP:
					ep = new C_Mothership();
					ep->decode(cur_head + cur_size);
					this->push_back(ep);
					break;
				case RESOURCE:
					ep = new C_Resource();
					ep->decode(cur_head + cur_size);
					this->push_back(ep);
					break;
				default:
					cerr << "ERROR decoding entity state. Unknown type: " << (int) (*(ENUM_TYPE*)( cur_head + cur_size)) << endl;
					break;
				}
			}
		}
	}

	const char * getSendBuff() const {
		char *send_buff = new char[m_meta.size];
		memcpy(send_buff,(char*) &m_meta, sizeof(m_meta));
		int total_size = sizeof(m_meta);
		for(unsigned int i = 0; i < m_entities.size(); i++) {
			const char* tmp = m_entities[i]->encode();
			memcpy(send_buff + total_size, tmp, m_entities[i]->size());
			total_size += m_entities[i]->size();
			delete []tmp;
		}		
		return send_buff;
	}

	struct gameStateMeta {
		unsigned int size;
	} m_meta;

	vector<shared_ptr<E> > m_entities;
	friend class NetworkServer;
	friend class NetworkClient;

};

