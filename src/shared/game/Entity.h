/*
 * Entity.h
 */

#ifndef ENTITIY_H_INCLUDED
#define ENTITIY_H_INCLUDED
#pragma once

#include <WinSock2.h>
//#include <windows.h>
#define WIN32_LEAN_AND_MEAN
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <shared/network/Sendable.h>

using namespace std;

enum Type { ENTITY, SHIP, BASE, ASTEROID};

#define ENUM_TYPE char

class Entity : public Sendable {
private:
	static int s_id_gen;
	int m_id;

protected:
	static const unsigned int m_size = sizeof(ENUM_TYPE) + sizeof(int) +  2*sizeof(D3DXVECTOR3);

public:
	Type m_type;
	
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_dir;

	Entity();
	Entity(Entity const &e);
	Entity(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
	const int getID() const { return m_id; };
	virtual ~Entity();
	virtual const char* encode() const;
	virtual void decode(const char *);
	virtual const unsigned int size() const { return m_size; };

	friend ostream& operator<<(ostream& os, const Entity& e);	
};


#endif