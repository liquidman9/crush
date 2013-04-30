/*
 * S_Ship.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Ship.h>

static float ROTATION_SCALE = 50;
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
	ServerEntity(100, 1, 100, calculateRotationalInertia(100), 5.0, 1.0)
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

	m_resource = NULL;
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

// This method needs to be extracted to the server/physics engine.
void S_Ship::addPlayerInput(InputState input) {
	m_thrust = input.thrust;
	m_tractorBeamOn = input.tractBeam;

	// Rotational thrust calculations
	float x = -input.getTurn() * ROTATION_SCALE;
	float y = -input.getPitch() * ROTATION_SCALE;
	D3DXVECTOR3 fore_rot_force(x, y, 0);
	D3DXVECTOR3 aft_rot_force(-x, -y, 0);
	
	//D3DXVECTOR3 stabilizer_force(-m_angular_momentum.x / 10, -m_angular_momentum.y / 10, -m_angular_momentum.z / 10); To be implemented

	D3DXVECTOR3 main_thrust_force(0, 0, input.getThrust() * THRUST_FACTOR);
	
	D3DXMATRIX mat_rotate;
	D3DXVECTOR4 temp;
	D3DXMatrixRotationQuaternion(&mat_rotate, D3DXQuaternionNormalize(&m_orientation, &m_orientation));

	// Fore thrust rocket pos transform
	D3DXVec3Transform(&temp, &forward_rot_thruster, &mat_rotate);
	D3DXVECTOR3 fore_thruster_pos_adj(temp.x, temp.y, temp.z);

	// Fore thrust transform
	D3DXVec3Transform(&temp, &fore_rot_force, &mat_rotate);
	D3DXVECTOR3 fore_thrust_adj(temp.x, temp.y, temp.z);

	// Aft thrust rocket pos transform
	D3DXVec3Transform(&temp, &reverse_rot_thruster, &mat_rotate);
	D3DXVECTOR3 aft_thruster_pos_adj(temp.x, temp.y, temp.z);

	// Aft thrust transform
	D3DXVec3Transform(&temp, &aft_rot_force, &mat_rotate);
	D3DXVECTOR3 aft_thrust_adj(temp.x, temp.y, temp.z);

	// Main rocket force transform
	D3DXVec3Transform(&temp, &main_thrust_force, &mat_rotate);
	D3DXVECTOR3 main_thrust_adj(temp.x, temp.y, temp.z);

	// Forward thrust calculations
	D3DXVECTOR3 thrust_force(0, 0, input.thrust * THRUST_FACTOR);
	D3DXVec3Rotate(&main_thrust_adj, &thrust_force, &m_orientation);

	//cout << "Aft thrust vectors: " << aft_thrust_adj.x << ", " << aft_thrust_adj.y << ", " << aft_thrust_adj.z << endl;

	// Forward rotation thruster
	applyImpulse(fore_thrust_adj, m_pos + fore_thruster_pos_adj, 0.1f);
	// Rear rotation thruster
	applyImpulse(aft_thrust_adj, m_pos + aft_thruster_pos_adj, 0.1f);
	// Main thruster
	applyImpulse(main_thrust_adj, 0.1f);

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


bool S_Ship::gatherResource(S_Resource * res) {
	if(m_resource != res && m_resource == NULL && res->m_carrier == NULL){
			m_resource = res;
			res->m_carrier = this;
			cout<<"Gathered"<<endl;
			return true;
	}

	return false;
}