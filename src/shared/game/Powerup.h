/*
 * Powerup.h
 */

#ifndef POWERUP_H_INCLUDED
#define POWERUP_H_INCLUDED

// Global includes
#include <memory>

// Project includes
#include <shared/game/Entity.h>
#include <shared/game/Ship.h>
 

// Defines



class Powerup : public virtual Entity {
private:
	static const unsigned int m_size = Entity::m_size + sizeof(POWER_TYPE) + sizeof(STATE_TYPE) + sizeof(SHIP_PLAYERNUM_TYPE);

public:
	PowerType m_powerType;
	StateType m_stateType;
	SHIP_PLAYERNUM_TYPE m_playerNum;

	Powerup();

	// Constructors
	Powerup(PowerType type);

	// Methods
	virtual unsigned int encode(char *) const;
	virtual unsigned int decode(const char *);
	virtual const unsigned int size() const { return m_size; };

	friend ostream& operator<<(ostream& os, const Powerup&);
	virtual void update(shared_ptr<Entity> source);
};


#endif POWERUP_H_INCLUDED