/*
 * Ship.h
 */

#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED
#include<memory>
#include <shared/game/Entity.h>
#define SHIP_PLAYERNUM_TYPE char

class Ship : public Entity {
private:
	static const unsigned int m_size = Entity::m_size + sizeof(SHIP_PLAYERNUM_TYPE) + sizeof(bool);

public:
	SHIP_PLAYERNUM_TYPE m_playerNum;
	bool m_tractorBeamOn;
	Ship();
	Ship(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int pNum, bool tBeamOn);
	virtual const char* encode() const;
	virtual void decode(const char *);
	virtual const unsigned int size() const { return m_size; };
	//virtual void update(Entity * source);
	friend ostream& operator<<(ostream& os, const Ship&);
	virtual void update(shared_ptr<Entity> source);
};


#endif