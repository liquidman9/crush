/*
 * S_Resource.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Resource.h>

S_Resource::S_Resource() :
	Entity(RESOURCE),
	Resource(),
	ServerEntity(10, calculateRotationalInertia(10)),
	m_carrier(NULL)
{
}

S_Resource::S_Resource(D3DXVECTOR3 pos, Quaternion orientation) :
	Entity(genId(), RESOURCE, pos, orientation),
	Resource(),
	ServerEntity(10, calculateRotationalInertia(10)),
	m_carrier(NULL)
{	
}

D3DXVECTOR3 S_Resource::calculateRotationalInertia(float mass){
	float radius_squared = 1;
	float height_squared = 1;
	return D3DXVECTOR3( (1.0f / 12.0f) * mass * (3 * radius_squared + height_squared),
						(0.5f) * mass * radius_squared,
						(1.0f / 12.0f) * mass * (3 * radius_squared + height_squared));
};

void S_Resource::update(float delta_time){
	if(m_carrier == NULL ) {
		ServerEntity::update(delta_time);
	}
	else if(m_carrier->m_type == MOTHERSHIP) {
		m_pos = m_carrier->m_pos + D3DXVECTOR3(0,m_carrier->m_radius,0);
	}
	else if(m_carrier->m_type == SHIP) {
		D3DXVECTOR3 out; 
		D3DXQUATERNION q1, q2;
		D3DXVECTOR3 off = D3DXVECTOR3(0,0,m_carrier->m_radius*1.5f);

		D3DXQuaternionConjugate( &q1, &(-m_carrier->m_orientation) );
		q2 = q1 * D3DXQUATERNION( off.x, off.y, off.z, 1.0f ) * (-m_carrier->m_orientation);
		out = D3DXVECTOR3( q2.x, q2.y, q2.z );

		m_pos = m_carrier->m_pos + out;
	}
	else {
		ServerEntity::update(delta_time);
	}
}