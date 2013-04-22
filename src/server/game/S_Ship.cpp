/*
 * S_Ship.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Ship.h>

static float ROTATION_SCALE = 500000;

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
	m_thrust = 0;
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
	D3DXVECTOR3 mov = D3DXVECTOR3();
	D3DXVec3Normalize(&mov, &velocity);
	if(m_thrust != 0) force = m_thrust*m_dir;
	else if(D3DXVec3Length(&velocity) > 0.00) {
		velocity -= velocity/10; // change to apply friction
		force = 0*m_dir;
	}
	else force = 0*m_dir;
	// add brake button
	ServerEntity::calculate(dt);
}

void S_Ship::addPlayerInput(InputState input) {
	m_thrust = input.thrust; //ship are facing in the opposite direction
	m_tractorBeamOn = input.tractBeam;

	float x = ((float)input.turn) / ROTATION_SCALE;
	float y = ((float)input.pitch) / ROTATION_SCALE;
	D3DXQUATERNION quat = D3DXQUATERNION ();
	D3DXQuaternionRotationYawPitchRoll(&quat, x,y,0);
	D3DXQUATERNION q1, q2,q3;
    D3DXQuaternionConjugate( &q1, &quat );
	q3 = D3DXQUATERNION( m_dir.x, m_dir.y, m_dir.z, 1.0f );
    q2 = q1 * q3 * quat;
    m_dir = D3DXVECTOR3( q2.x, q2.y, q2.z );
	D3DXVec3Normalize(&m_dir,&m_dir);

}