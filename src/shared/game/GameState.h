#pragma once

// Global includes
#include <vector>

// Project includes
#include <shared/game/Entity.h>

//#include"Network.h"
class GameState
{
public:

	GameState(void):m_entities() {
	};

	GameState(GameState const & g): m_entities(g.getEntities()){
	};

	vector<Entity*> getEntities() const {
		return m_entities;
	};

	void push_back(Entity *e) {
		m_entities.push_back(e);
	};

	unsigned int size() {
		return m_entities.size();
	}

	Entity*& operator[](unsigned int i) {
		return m_entities[i];
	}

	void clear() {
		m_entities.clear();
	}

	void erase() {
		for(unsigned int i = 0; i < m_entities.size(); i++){
			if(m_entities[i])
				delete m_entities[i];
		}
		m_entities.clear();
	};

	virtual ~GameState(void) {
		erase();
	};

private:
	vector<Entity*> m_entities;
	friend class NetworkingDecoder;

};

