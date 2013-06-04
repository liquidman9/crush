/*
 * CollisionGEvent.h
 */

#ifndef COLLISIONGEVENT_H_INCLUDED
#define COLLISIONGEVENT_H_INCLUDED

// Global includes
#include <d3dx9.h>

// Project includes
#include <shared/GEvent.h>

enum CType : char {C, SR, MS, PP, RM, RR, SA, SE, SP, SS, T, AA, AM, AE};

class CollisionGEvent : public GEvent {
private:
	struct send_struct 
	{
		ENUM_TYPE type;
		CType ctype;
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
	CType m_ctype;


	// Constructors
	CollisionGEvent();
	CollisionGEvent(int id_a, int id_b, D3DXVECTOR3 poi, float impulse, CType ctype);

	// Methods
	virtual unsigned int encode(char *) const;
	virtual unsigned int decode(const char *);
	virtual const unsigned int size() const { return m_size; };
};

#endif