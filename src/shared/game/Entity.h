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

// Project includes
#include <shared/network/Sendable.h>
#include <client/graphics/Renderable.h>

using namespace std;

typedef D3DXQUATERNION Quaternion;
// Putting this here until I find a better place for it!
D3DXVECTOR3 *D3DXVec3Rotate( D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DXQUATERNION *pQ );

#define ENUM_TYPE char
enum Type : char { ENTITY, SHIP, BASE, ASTEROID};

class Entity : public Sendable {	
protected:
	static const unsigned int m_size = sizeof(ENUM_TYPE) + sizeof(int) + sizeof(D3DXVECTOR3) + sizeof(Quaternion);
	
	// THIS CONSTRUCTOR SHOULD NEVER BE DIRECTLY CALLED.
	Entity();

public:
	// ID AND TYPE SHOULD BOTH BE CONST
	//const int m_id;
	//const Type m_type;
	int m_id;
	Type m_type;

	D3DXVECTOR3 m_pos;
	Quaternion m_orientation;

	// These two constructors are DEPRECATED
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
	virtual const char* encode() const;
	virtual void decode(const char *);
	virtual const unsigned int size() const { return m_size; };
	virtual void update(shared_ptr<Entity> source);
	//virtual void update(Entity* source);

	friend ostream& operator<<(ostream& os, const Entity& e);	
};


#endif