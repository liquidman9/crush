#include <server/game/ServerEntity.h>

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

