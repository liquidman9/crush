/*
 * Entity.cpp
 */

#include "entity.h"

int Entity::s_id_gen = 0;

Entity::Entity() :
	m_id(s_id_gen++),
	m_pos(D3DXVECTOR3(0.0, 0.0, 0.0)),
	m_dir(D3DXVECTOR3(0.0, 0.0, 1.0))
{
}

Entity::Entity(D3DXVECTOR3 pos, D3DXVECTOR3 dir) : 
	m_id(s_id_gen++),
	m_pos(pos),
	m_dir(dir)
{
}