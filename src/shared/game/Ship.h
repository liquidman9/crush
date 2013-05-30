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
// Defines



class Ship : public virtual Entity {
private:
	static const unsigned int m_size = Entity::m_size + 2*sizeof(bool) + sizeof(POWER_TYPE) + sizeof(STATE_TYPE) + sizeof(SHIP_PLAYERNUM_TYPE) + sizeof(double) + MAX_PLAYERNAME_SIZE;

public:
	// Fields
	SHIP_PLAYERNUM_TYPE m_playerNum;
	double m_thruster;
	std::string m_playerName;
	bool m_hasPowerup;
	PowerType m_powerupType;
	StateType m_powerupStateType;

	// This constructors are deprecated
	Ship();

	// Constructors
	Ship(int pNum);

	// Methods
	virtual void setPlayerName(const string &);
	virtual unsigned int encode(char *) const;
	virtual unsigned int decode(const char *);
	virtual const unsigned int size() const { return m_size; };

	//virtual void update(Entity * source);
	friend ostream& operator<<(ostream& os, const Ship&);
	virtual void update(shared_ptr<Entity> source);
};


#endif