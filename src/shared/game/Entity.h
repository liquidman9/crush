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

enum Type { ENTITY, SHIP, BASE, ASTEROID};

#define ENUM_TYPE char

class Entity : public Sendable {
private:
	static int s_id_gen;
	int m_id;
	
protected:
	static const unsigned int m_size = sizeof(ENUM_TYPE) + sizeof(int) +  2*sizeof(D3DXVECTOR3);
	
	// THIS CONSTRUCTOR SHOULD NEVER BE DIRECTLY CALLED.
	Entity();

public:
	Type m_type;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_dir;

	// Constructors
	Entity(Type type);
	Entity(Entity const &e);
	Entity(Type type, D3DXVECTOR3 pos, D3DXVECTOR3 dir);
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