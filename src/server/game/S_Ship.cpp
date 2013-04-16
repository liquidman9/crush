/*
 * S_Ship.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <server/game/S_Ship.h>


S_Ship::S_Ship() {
	Ship();
	init();
}

S_Ship::S_Ship(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int pNum, bool tBeamOn) :
	Ship(pos, dir, pNum)
{
	Ship(pos, dir, pNum);
	init();
}

void S_Ship::init() {
	thrusterForce = 100;
	force = 0;
	mass = 100;
	velocity = 0;
	maxVelocity = 100;
	// In the future, this method will load from a config file.
}

void S_Ship::rotate(D3DXVECTOR3 rotation){
	m_dir = m_dir + rotation;
	//D3DXVec3Normalize(&m_dir,&m_dir); - linker error
	float tmp = abs(sqrt((m_dir.x * m_dir.x) + (m_dir.y * m_dir.y) + (m_dir.z * m_dir.z)));
	m_dir = D3DXVECTOR3(m_dir.x/tmp,m_dir.y/tmp,m_dir.z/tmp);
}


void S_Ship::calculate(float dt){
	// Calculate applied force
	if(m_thrustersOn) force = thrusterForce;
	else if(velocity != 0) force = -thrusterForce;
	else force = 0;


	float dv = ((force / mass ) * dt);
	if(velocity + dv <= maxVelocity)
		velocity = velocity + dv;
	m_pos = m_pos + velocity*m_dir;
}

