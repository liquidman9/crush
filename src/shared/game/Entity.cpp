/*
* Entity.cpp
*/

// Global includes
#include <iostream>

// Project includes
#include <shared/game/Entity.h>

int Entity::s_id_gen = 0;

Entity::Entity() :
m_id(s_id_gen++),
	m_pos(D3DXVECTOR3(0.0, 0.0, 0.0)),
	m_dir(D3DXVECTOR3(0.0, 0.0, 1.0)),
	m_type(ENTITY)
{
}

Entity::Entity(Entity const &e)  {
	m_type = ENTITY;
	m_id = e.m_id;
	memcpy(m_pos, e.m_pos, sizeof(D3DXVECTOR3));
	memcpy(m_dir, e.m_dir, sizeof(D3DXVECTOR3));
}

Entity::Entity(D3DXVECTOR3 pos, D3DXVECTOR3 dir) : 
m_id(s_id_gen++),
	m_pos(pos),
	m_dir(dir),
	m_type(ENTITY)
{
}

ostream& operator<<(ostream& os, const Entity& e) {
	os << e.getID() << " " << e.m_pos.x << " " << e.m_pos.y << " " << e.m_pos.z
		<< " " << e.m_dir.x << " " << e.m_dir.y << " " << e.m_dir.z;
	return os;
}


const char * Entity::encode() const {
	char * tmp = new char[sizeof(Entity)];
	*(ENUM_TYPE *) tmp = ENTITY;
	*(int *) (tmp + sizeof(ENUM_TYPE)) = m_id;
	*(D3DXVECTOR3 *) (tmp + sizeof(m_id) + sizeof(ENUM_TYPE)) = m_pos;
	*(D3DXVECTOR3 *) (tmp + sizeof(D3DXVECTOR3) + sizeof(m_id) + sizeof(ENUM_TYPE)) = m_dir;
	return tmp;
}

void Entity::decode(const char * tmp) {
	memcpy(&m_pos,tmp+sizeof(m_id) + sizeof(ENUM_TYPE), sizeof(D3DXVECTOR3));
	memcpy(&m_dir,tmp+sizeof(m_id)+sizeof(D3DXVECTOR3) + sizeof(ENUM_TYPE), sizeof(D3DXVECTOR3));
	m_id = *(int *) (tmp + sizeof(ENUM_TYPE));
}

void Entity::draw() {

}

void Entity::update(shared_ptr<Entity> source) {
	m_pos = source->m_pos;
	m_dir = source->m_pos;
}

Entity::~Entity(){
}