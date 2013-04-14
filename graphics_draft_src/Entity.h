/*
 * Entity.h
 */

#ifndef ENTITIY_H_INCLUDED
#define ENTITIY_H_INCLUDED

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

class Entity {
private:
	static int s_id_gen;

public:
	const int m_id;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_dir;

	Entity();
	Entity(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
};

#endif