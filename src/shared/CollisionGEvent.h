/*
 * CollisionGEvent.h
 */

#ifndef COLLISIONGEVENT_H_INCLUDED
#define COLLISIONGEVENT_H_INCLUDED

// Global includes
#include <d3dx9.h>

// Project includes
#include <shared/GEvent.h>

class CollisionGEvent : public GEvent {
private:
	struct send_struct 
	{
		ENUM_TYPE type;
		int id_a, id_b;
		D3DXVECTOR3 poi;
		float impulse;
	};

protected:
	static const unsigned int m_size = sizeof(send_struct);

public:
	// Fields
	int m_idA, m_idB;
	D3DXVECTOR3 m_poi;
	float m_impulse;


	// Constructors
	CollisionGEvent();
	CollisionGEvent(int id_a, int id_b, D3DXVECTOR3 poi, float impulse);

	// Methods
	virtual unsigned int encode(char *) const;
	virtual unsigned int decode(const char *);
	virtual const unsigned int size() const { return m_size; };
};

#endif