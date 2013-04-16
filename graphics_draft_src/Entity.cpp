
/*
* Entity.cpp
*/

#include "Entity.h"
#include <iostream>

int Entity::s_id_gen = 0;
const unsigned int Entity::size = sizeof(/*m_type*/int) + 
	sizeof(/*m_id*/int) +  
	2*sizeof(D3DXVECTOR3);

Entity::Entity() :
m_id(s_id_gen++),
	m_pos(D3DXVECTOR3(0.0, 0.0, 0.0)),
	m_dir(D3DXVECTOR3(0.0, 0.0, 1.0)),
	m_type(ENTITY)
{
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
	//std::cerr << sizeof(Entity) << endl;
	*(int *) tmp = ENTITY;
	*(int *) (tmp + sizeof(ENTITY)) = m_id;
	*(D3DXVECTOR3 *) (tmp + sizeof(m_id) + sizeof(ENTITY)) = m_pos;
	*(D3DXVECTOR3 *) (tmp + sizeof(D3DXVECTOR3) + sizeof(m_id) + sizeof(ENTITY)) = m_dir;
	return tmp;
}

Entity Entity::decode(const char * tmp) const {
	//const char* tmp = encoded.c_str();
	D3DXVECTOR3 m_pos_new;
	D3DXVECTOR3 m_dir_new;
	memcpy(m_pos_new,tmp+sizeof(m_id) + sizeof(ENTITY), sizeof(D3DXVECTOR3));
	memcpy(m_dir_new,tmp+sizeof(m_id)+sizeof(D3DXVECTOR3) + sizeof(ENTITY), sizeof(D3DXVECTOR3));
	/*D3DXVECTOR3 m_pos_new( *(float*) (tmp + sizeof(m_id)), *(float*) (tmp + sizeof(m_id) + sizeof(float)), *(float*) (tmp + sizeof(m_id) + 2*sizeof(float)));
	int total = sizeof(m_id) + sizeof(D3DXVECTOR3);
	D3DXVECTOR3 m_dir_new( *(float*) (tmp + total), *(float*) (tmp + total+sizeof(float)), *(float*) (tmp + total + 2*sizeof(float)));*/
	Entity rtn(m_pos_new, m_dir_new);
	rtn.m_id = *(int *) (tmp + sizeof(ENTITY));
	return rtn;
}

Entity::~Entity(){

}