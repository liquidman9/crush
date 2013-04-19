/*
 * Ship.h
 */

#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

// Global includes
#include <memory>

// Project includes
#include <shared/game/Entity.h>
 
// Defines
#define SHIP_PLAYERNUM_TYPE char

/*
- Represents a Ship Entity
- 
- As this class virtually extends Entity, any subclasses MUST directly call an
- Entity constructor in their initialization list.
 */
class Ship : public virtual Entity {
private:
	static const unsigned int m_size = Entity::m_size + sizeof(SHIP_PLAYERNUM_TYPE) + sizeof(bool);

public:
	// Fields
	SHIP_PLAYERNUM_TYPE m_playerNum;
	bool m_tractorBeamOn;

	// Constructors
	Ship();
	Ship(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int pNum, bool tBeamOn);

	// Methods
	virtual const char* encode() const;
	virtual void decode(const char *);
	virtual const unsigned int size() const { return m_size; };
	//virtual void update(Entity * source);
	friend ostream& operator<<(ostream& os, const Ship&);
	virtual void update(shared_ptr<Entity> source);
};


#endif