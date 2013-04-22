/*
 * S_Ship.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Ship.h>

static float ROTATION_SCALE = 1;
static float THRUST_FACTOR = 1;

S_Ship::S_Ship() :
	Entity(SHIP),
	Ship()
{
	init();
}

S_Ship::S_Ship(D3DXVECTOR3 pos, Quaternion orientation, int pNum) :
	Entity(genId(), SHIP, pos, orientation),
	Ship(pNum),
	ServerEntity(100, 1, 100, calculateRotationalInertia(100))
{	
	init();
}

void S_Ship::init() {
	m_thrust = 0;
	m_mass = 100;
	m_radius = 2.5;
	//p1 = m_pos + D3DXVECTOR3(5.0, 0, 0);  // m_pos is center of ship, p1 and p2 are the ends minus radius
	//p2 = m_pos - D3DXVECTOR3(5.0, 0, 0);
	// In the future, this method will load from a config file.

	forward_rot_thruster = D3DXVECTOR3(0, 0, 5);
	reverse_rot_thruster = D3DXVECTOR3(0, 0, -5);
}


void S_Ship::calculate(float dt){
	// Calculate applied force
	/*
	D3DXVECTOR3 mov = D3DXVECTOR3();
	D3DXVec3Normalize(&mov, &m_velocity);
	if(m_thrust != 0) force = m_thrust*m_dir;
	else if(D3DXVec3Length(&velocity) > 0.00) {
		velocity -= velocity/10; // change to apply friction
		force = 0*m_dir;
	}
	else force = 0*m_dir;
	// add brake button
	ServerEntity::calculate(dt);
	*/
}

void S_Ship::addPlayerInput(InputState input) {
	m_thrust = input.thrust;
	m_tractorBeamOn = input.tractBeam;

	float x = ((float)input.turn) / ROTATION_SCALE;
	float y = ((float)input.pitch) / ROTATION_SCALE;
	D3DXVECTOR3 rot_force(x, y, 0);
	D3DXVECTOR3 thrust_force(0, 0, input.thrust);

	// Forward rotation thruster
	applyImpulse(rot_force, m_pos + forward_rot_thruster, 0.1f);
	// Rear rotation thruster
	applyImpulse(-rot_force, m_pos + reverse_rot_thruster, 0.1f);
	// Main thruster
	applyImpulse(thrust_force * THRUST_FACTOR, 0.1f);

	/*
	D3DXQUATERNION quat = D3DXQUATERNION ();
	D3DXQuaternionRotationYawPitchRoll(&quat, x,y,0);
	D3DXQUATERNION q1, q2,q3;
    D3DXQuaternionConjugate( &q1, &quat );
	q3 = D3DXQUATERNION( m_dir.x, m_dir.y, m_dir.z, 1.0f );
    q2 = q1 * q3 * quat;
    m_dir = D3DXVECTOR3( q2.x, q2.y, q2.z );
	D3DXVec3Normalize(&m_dir,&m_dir);
	*/

}

D3DXVECTOR3 S_Ship::calculateRotationalInertia(float mass){
	float radius_squared = 25;
	float height_squared = 100;
	return D3DXVECTOR3( (1.0f / 12.0f) * mass * (3 * radius_squared + height_squared),
						(0.5f) * mass * radius_squared,
						(1.0f / 12.0f) * mass * (3 * radius_squared + height_squared));
};
