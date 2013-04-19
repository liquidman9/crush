/*
 * S_Ship.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Ship.h>


S_Ship::S_Ship() :
	Entity(SHIP),
	Ship()
{
	init();
}

S_Ship::S_Ship(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int pNum, bool tBeamOn) :
	Entity(SHIP, pos,dir),
	Ship(pos, dir, pNum, tBeamOn)
{	
	init();
}

void S_Ship::init() {
	m_acceleratorOn = false;
	thrusterForce = 100;
	force = D3DXVECTOR3(0,0,0);
	mass = 100;
	velocity = D3DXVECTOR3(0,0,0);
	maxVelocity = 100;
	radius = 2.5;
	//p1 = m_pos + D3DXVECTOR3(5.0, 0, 0);  // m_pos is center of ship, p1 and p2 are the ends minus radius
	//p2 = m_pos - D3DXVECTOR3(5.0, 0, 0);
	// In the future, this method will load from a config file.
}


void S_Ship::calculate(float dt){
	// Calculate applied force
	
	if(m_acceleratorOn) force = thrusterForce*m_dir;
	else if(D3DXVec3Length(&velocity) > 0) force = -thrusterForce*m_dir; // change to apply friction
	else force = 0*m_dir;
	// add brake button

	ServerEntity::calculate(dt);
}

