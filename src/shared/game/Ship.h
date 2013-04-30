/*
 * Ship.h
 */

#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

// Global includes
#include <memory>
#include <string>

// Project includes
#include <shared/game/Entity.h>
 
// Defines
#define SHIP_PLAYERNUM_TYPE char
#define MAX_PLAYERNAME_SIZE 15

class Ship : public virtual Entity {
private:
	static const unsigned int m_size = Entity::m_size + sizeof(SHIP_PLAYERNUM_TYPE) + sizeof(bool) + MAX_PLAYERNAME_SIZE;

public:
	// Fields
	SHIP_PLAYERNUM_TYPE m_playerNum;
	bool m_tractorBeamOn;
	std::string m_playerName;

	// This constructors are deprecated
	Ship();

	// Constructors
	Ship(int pNum);

	// Methods
	virtual void setPlayerName(const string &);
	virtual const char* encode() const;
	virtual void decode(const char *);
	virtual const unsigned int size() const { return m_size; };

	//virtual void update(Entity * source);
	friend ostream& operator<<(ostream& os, const Ship&);
	virtual void update(shared_ptr<Entity> source);
};


#endif