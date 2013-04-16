/*
 * Entity.hpp
 */

/*#include "D3DX10math.h"

enum Type { ENTITY, SHIP, BASE, ASTEROID};

class Entity {
protected:
	static int id_gen;

public:
	// Identification
	int id;
	Type type;

	// Physics
	D3DXVECTOR3 pos;
	D3DXVECTOR3 dir;
	float velocity;
	float maxVelocity;
	float force;
	float mass;
	bool isThrustOn;

	Entity();
	Entity(D3DXVECTOR3, D3DXVECTOR3);
	void init();

	void rotate(D3DXVECTOR3);
	void toggleThrust();
	void calculate(float);

};*/


/*
 * Entity.h
 */

#ifndef ENTITIY_H_INCLUDED
#define ENTITIY_H_INCLUDED

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include<iostream>
using namespace std;

enum Type { ENTITY, SHIP, BASE, ASTEROID};

class Entity {
private:
	static int s_id_gen;
	int m_id;

public:
	Type m_type;
	static const unsigned int size;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_dir;

	Entity();
	Entity(Entity const &e) {
		m_type = ENTITY;
		m_id = e.m_id;
		memcpy(m_pos, e.m_pos, sizeof(D3DXVECTOR3));
		memcpy(m_dir, e.m_dir, sizeof(D3DXVECTOR3));
	}

	Entity(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
	const int getID() const { return m_id; };
	virtual ~Entity();
	virtual const char * encode() const;
	virtual Entity decode(const char *) const;
	friend ostream& operator<<(ostream& os, const Entity& e);
	
};


#endif