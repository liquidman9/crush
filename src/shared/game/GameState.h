#pragma once


// Global includes
#include <vector>
#include<memory>

// Project includes
#include <shared/game/Entity.h>

//#include"Network.h"
class GameState
{
public:

	GameState(void):m_entities() {
	};

	GameState(GameState const & g): m_entities(g.m_entities) {
	};

	/*vector<shared_prt<Entity> > getEntities() const {
		return m_entities;
	};*/

	void push_back(Entity *e) {
		m_entities.push_back(shared_ptr<Entity>(e));
	};

	unsigned int size() const {
		return m_entities.size();
	}

	shared_ptr<Entity> operator[](unsigned int i) {
		return m_entities[i];
	}

	shared_ptr<const Entity> operator[](unsigned int i) const {
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
	};
	
private:
	vector<shared_ptr<Entity> > m_entities;
	friend class NetworkingDecoder;

};

