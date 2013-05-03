/*
 * TractorBeam.h
 */

#ifndef TRACTORBEAM_H_INCLUDED
#define TRACTORBEAM_H_INCLUDED

// Global includes
#include <memory>

// Project includes
#include <shared/game/Entity.h>
 
// Defines
#define SHIP_PLAYERNUM_TYPE char

class TractorBeam : public virtual Entity {
private:
	static const unsigned int m_size = Entity::m_size + sizeof(SHIP_PLAYERNUM_TYPE) + sizeof(bool) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3) + sizeof(float);

public:
	// Fields
	SHIP_PLAYERNUM_TYPE m_playerNum;
	bool m_isOn;
	D3DXVECTOR3 m_start;
	D3DXVECTOR3 m_end;
	float m_sentRadius; 

	// Constructors
	TractorBeam();
	TractorBeam(int pNum);

	// Methods
	virtual const char* encode() const;
	virtual void decode(const char *);
	virtual const unsigned int size() const { return m_size; };

	//virtual void update(Entity * source);
	friend ostream& operator<<(ostream& os, const TractorBeam&);
	virtual void update(shared_ptr<Entity> source);
};


#endif TRACTORBEAM_H_INCLUDED