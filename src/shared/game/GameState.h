/*
 * GameState.h
 */

#pragma once


// Global includes
#include <vector>
#include <memory>

// Project includes
#include <shared/game/Entity.h>

template <class E>
class GameState
{
public:

	GameState(void):m_entities(), m_sendSize(0) {
	};

	GameState(GameState const & g): m_entities(g.m_entities), m_sendSize(0) {
	};

	void push_back(E *e) {
		m_sendSize += e->size();
		m_entities.push_back(shared_ptr<Entity>(e));
	};

	unsigned int size() const {
		return m_entities.size();
	}

	unsigned int sendSize() const {
		return m_sendSize;
	}

	shared_ptr<E> operator[](unsigned int i) {
		return m_entities[i];
	}

	shared_ptr<const E> operator[](unsigned int i) const {
		return m_entities[i];
	}

	void clear() {
		m_entities.clear();
	}

	/*void clear() {
		for(unsigned int i = 0; i < m_entities.size(); i++){
			if(m_entities[i])
				delete m_entities[i];
		}
		m_entities.clear();
	};*/

	virtual ~GameState(void) {
		m_entities.clear();
	};
	
private:
	const char * getSendBuff() const {
		char *send_buff = new char[m_sendSize];
		int total_size = 0;
		for(unsigned int i = 0; i < m_entities.size(); i++) {
			const char* tmp = m_entities[i]->encode();
			memcpy(send_buff + total_size, tmp, m_entities[i]->size());
			total_size += m_entities[i]->size();
			delete []tmp;
		}
		return send_buff;
	}

	vector<shared_ptr<E> > m_entities;
	unsigned int m_sendSize;
	friend class NetworkServer;

};

