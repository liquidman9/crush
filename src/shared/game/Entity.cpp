/*
* Entity.cpp
*/

// Global includes
#include <iostream>

// Project includes
#include <shared/game/Entity.h>

// Old constructors
Entity::Entity() :
	m_id(-1),
	m_pos(D3DXVECTOR3(0.0, 0.0, 0.0)),
	m_orientation(0.0, 0.0, 0.0, 1.0),
	m_velocity(shared::utils::VEC3_ZERO),
	m_type(ENTITY)
{ }

Entity::Entity(Type type) :
	m_id(-1),
	m_pos(D3DXVECTOR3(0.0, 0.0, 0.0)),
	m_orientation(0.0, 0.0, 0.0, 1.0),
	m_velocity(shared::utils::VEC3_ZERO),
	m_type(type)
{ }
// End old constructors

Entity::Entity(int id, Type type) :
	m_id(id),
	m_pos(D3DXVECTOR3(0.0, 0.0, 0.0)),
	m_orientation(0.0, 0.0, 0.0, 1.0),
	m_velocity(shared::utils::VEC3_ZERO),
	m_type(type)
{ }

Entity::Entity(int id, Type type, D3DXVECTOR3 pos, Quaternion orientation) : 
	m_id(id),
	m_pos(pos),
	m_orientation(orientation),
	m_velocity(shared::utils::VEC3_ZERO),
	m_type(type)
{ }

Entity::Entity(Entity const &e) :
	m_id(e.m_id),
	m_type(e.m_type),
	m_pos(e.m_pos),
	m_velocity(e.m_velocity),
	m_orientation(e.m_orientation)
{ }

ostream& operator<<(ostream& os, const Entity& e) {
	os << e.getID() << " " << e.m_pos.x << " " << e.m_pos.y << " " << e.m_pos.z
		<< " " << e.m_orientation.x << " " << e.m_orientation.y << " " << e.m_orientation.z << " " << e.m_orientation.w;
	return os;
}


const char * Entity::encode() const {
	send_struct s;
	s.type = m_type;
	s.id = m_id;
	s.pos = m_pos;
	s.orientation = m_orientation;
	s.velocity = m_velocity;
	s.pBack = m_pBack;
	s.pFront = m_pFront;
	s.radius = m_radius;

	//char * tmp = new char[sizeof(Entity)];
	//// Encode type
	//*(ENUM_TYPE *) tmp = m_type;
	//// Encode id
	//*(int *) (tmp + sizeof(ENUM_TYPE)) = m_id;
	//// Encode pos
	//*(D3DXVECTOR3 *) (tmp + sizeof(m_id) + sizeof(ENUM_TYPE)) = m_pos;
	//// Encode orientation
	//*(Quaternion *) (tmp + sizeof(m_id) + sizeof(ENUM_TYPE) + sizeof(D3DXVECTOR3)) = m_orientation;

	char* tmp = new char[sizeof(send_struct)];
	memcpy(tmp, (const char *) &s, sizeof(send_struct));

	return tmp;
}

void Entity::decode(const char * tmp) {
	send_struct s;
	memcpy((char *) &s, tmp, sizeof(send_struct));
	m_id = s.id;
	m_pos = s.pos;
	m_orientation = s.orientation;
	m_velocity = s.velocity;
	m_pFront = s.pFront;
	m_pBack = s.pBack;
	m_radius = s.radius;

	// Decode Position
	/*m_id = *(int *) (tmp + sizeof(ENUM_TYPE));
	m_pos = *(D3DXVECTOR3*)(tmp + sizeof(m_id) + sizeof(ENUM_TYPE));
	m_orientation = *(Quaternion*)(tmp + sizeof(m_id) + sizeof(ENUM_TYPE) + sizeof(D3DXVECTOR3));*/
	/*memcpy(&m_pos,tmp + sizeof(m_id) + sizeof(ENUM_TYPE), sizeof(D3DXVECTOR3));
	memcpy(&m_orientation, tmp + sizeof(m_id) + sizeof(ENUM_TYPE) + sizeof(D3DXVECTOR3), sizeof(Quaternion));*/
}

void Entity::update(shared_ptr<Entity> source) {
	m_pos = source->m_pos;
	m_orientation = source->m_orientation;
	m_velocity = source->m_velocity;
}

Entity::~Entity(){
}

// Putting this here until I find a better place for it!
D3DXVECTOR3 *D3DXVec3Rotate( D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DXQUATERNION *pQ )
{
    D3DXQUATERNION q1, q2;
    D3DXQuaternionConjugate( &q1, pQ );
    q2 = q1 * D3DXQUATERNION( pV->x, pV->y, pV->z, 1.0f ) * (*pQ);

	*pOut = D3DXVECTOR3( q2.x, q2.y, q2.z );
    return pOut;
}