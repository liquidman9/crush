
/*
 * Entity.cpp
 */

// Project includes
#include <shared/game/Entity.h>

int Entity::s_id_gen = 0;

Entity::Entity(Type type) :
	m_id(s_id_gen++),
	m_pos(Vector3(0.0, 0.0, 0.0)),
	m_dir(Vector3(0.0, 0.0, 1.0)),
	m_type(type)
{ }

Entity::Entity(Type type, Vector3 pos, Vector3 dir) : 
	m_id(s_id_gen++),
	m_pos(pos),
	m_dir(dir),
	m_type(type)
{ }