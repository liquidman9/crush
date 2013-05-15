/*
 * CollisionEvent.h
 */

#ifndef COLLISIONEVENT_H_INCLUDED
#define COLLISIONEVENT_H_INCLUDED

// Global includes
#include <d3dx9.h>

// Project includes
#include <shared/Event.h>

class CollisionEvent : public Event {
private:
	struct send_struct 
	{
		ENUM_TYPE type;
		int id_a, id_b;
		D3DXVECTOR3 poi;
	};

protected:
	static const unsigned int m_size = sizeof(send_struct);

public:
	// Fields
	int m_idA, m_idB;
	D3DXVECTOR3 m_poi;


	// Constructors
	CollisionEvent();
	CollisionEvent(int id_a, int id_b, D3DXVECTOR3 poi);

	// Methods
	virtual unsigned int encode(char *) const;
	virtual unsigned int decode(const char *);
};

#endif