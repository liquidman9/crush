#include <shared/game/Entity.h>
#include <server/game/ServerEntity.h>

ServerEntity::ServerEntity() :
	velocity(0.0f, 0.0f, 0.0f),
	maxVelocity(1),
	force(0.0f, 0.0f, 0.0f),
	mass(1),
	radius(1)
{ }

ServerEntity::ServerEntity(D3DXVECTOR3 velocity, float maxVelocity, D3DXVECTOR3 force, float mass):
	velocity(velocity),
	maxVelocity(maxVelocity),
	force(force),
	mass(mass),
	radius(1)
{ }


void ServerEntity::rotate(D3DXVECTOR3 rotation){
	m_dir = m_dir + rotation;
	D3DXVec3Normalize(&m_dir,&m_dir); 
}

void ServerEntity::calculate(float dt) {

	D3DXVECTOR3 dv = ((force / mass ) * dt);
	if(D3DXVec3Length(&(velocity + dv)) <= maxVelocity)
		velocity = velocity + dv;
	m_pos = m_pos + velocity;
}

