/*
 * Mothership.h
 */

#ifndef MOTHERSHIP_H_INCLUDED
#define MOTHERSHIP_H_INCLUDED

// Global includes
#include <memory>

// Project includes
#include <shared/game/Entity.h>
 
// Defines
#define PLAYERNUM_TYPE char

class Mothership : public virtual Entity {
private:
	static const unsigned int m_size = Entity::m_size + sizeof(PLAYERNUM_TYPE) + sizeof(int);

public:
	// Fields
	PLAYERNUM_TYPE m_playerNum;
	int m_resources;

	// Constructors
	Mothership();
	Mothership(int pNum);

	// Methods
	virtual const char* encode() const;
	virtual void decode(const char *);
	virtual const unsigned int size() const { return m_size; };

	//virtual void update(Entity * source);
	friend ostream& operator<<(ostream& os, const Mothership&);
	virtual void update(shared_ptr<Entity> source);
};


#endif MOTHERSHIP_H_INCLUDED