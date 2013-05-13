/*
 * Entity.h
 */

#ifndef ENTITIY_H_INCLUDED
#define ENTITIY_H_INCLUDED
#pragma once

// Defines
#define WIN32_LEAN_AND_MEAN

// Global includes
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <memory>
#include <math.h>

// Project includes
#include <shared/network/Sendable.h>
#include <client/graphics/Renderable.h>
#include <shared/util/SharedUtils.h>

#define ENABLE_DELTAS

using namespace std;

typedef D3DXQUATERNION Quaternion;
// Putting this here until I find a better place for it!
D3DXVECTOR3 *D3DXVec3Rotate( D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DXQUATERNION *pQ );

#define ENUM_TYPE char
enum Type : char { ENTITY, SHIP, MOTHERSHIP, ASTEROID, RESOURCE, TRACTORBEAM, EXTRACTOR};

class Entity : public Sendable {
private:
	struct send_struct {
		ENUM_TYPE type;
		Quaternion orientation;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 velocity;		
		D3DXVECTOR3 pFront;
		D3DXVECTOR3 pBack;		
		int id;		
		float radius;
	};

protected:
	static const unsigned int m_size = sizeof(send_struct);
	// THIS CONSTRUCTOR SHOULD NEVER BE DIRECTLY CALLED.
	Entity();

public:
	// ID AND TYPE SHOULD BOTH BE CONST
	//const int m_id;
	//const Type m_type;
	int m_id;
	Type m_type;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_velocity;
	Quaternion m_orientation;
	D3DXVECTOR3 m_pFront;
	D3DXVECTOR3 m_pBack;
	float m_radius;

	// This constructors is DEPRECATED
	Entity(Type type);

	// Constructors
	Entity(int id, Type type);
	Entity(int id, Type type, D3DXVECTOR3 pos, Quaternion orientation);
	// Copy constructor
	Entity(Entity const &e);
	// Destructor
	virtual ~Entity();

	// Methods
	const int getID() const { return m_id; };
	virtual unsigned int encode(char *);
	virtual unsigned int decode(char *);
	virtual const unsigned int size() const { return m_size; };
#ifdef ENABLE_DELTA
	virtual const unsigned int deltaSize() const { 
		return this->m_size + (int) ceil((float) this->m_size/(sizeof(char)*8)) + sizeof(BITFIELD_CONTAINER);
	}
#endif
	virtual void update(shared_ptr<Entity> source);
	//virtual void update(Entity* source);

	friend ostream& operator<<(ostream& os, const Entity& e);	
};


#endif