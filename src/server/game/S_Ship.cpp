/*
 * S_Ship.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/ConfigSettings.h>
#include <shared/game/Entity.h>
#include <server/game/S_Ship.h>

using namespace server::entities::ship;

S_Ship::S_Ship() :
	Entity(SHIP),
	Ship(),
	ServerEntity(),
	m_forward_thrust_force(forward_thrust_force),
	m_rotation_thrust_force(rotation_thrust_force),
	m_stabilizer_ratio(stabilizer_ratio),
	m_resource(NULL)
{
	init();
}

S_Ship::S_Ship(D3DXVECTOR3 pos, Quaternion orientation, int pNum) :
	Entity(genId(), SHIP, pos, orientation),
	Ship(pNum),
	ServerEntity(server::entities::ship::mass, calculateRotationalInertia(server::entities::ship::mass)),
	m_forward_thrust_force(forward_thrust_force),
	m_rotation_thrust_force(rotation_thrust_force),
	m_stabilizer_ratio(stabilizer_ratio),
	m_resource(NULL)
{	
	init();
}

void S_Ship::init() {
	forward_rot_thruster = D3DXVECTOR3(0, 0, 5);
	reverse_rot_thruster = D3DXVECTOR3(0, 0, -5);
}

// TODO!!!:
// This method needs to be extracted to the server/physics engine.
void S_Ship::addPlayerInput(InputState input) {
	m_tractorBeamOn = (float) input.getTractorBeam();

	// Linear thrust calculations
	D3DXVECTOR3 main_thrust_force(0, 0, (float)(input.getThrust() * m_forward_thrust_force));

	// Rotational thrust calculations
	D3DXVECTOR3 fore_rot_force((float)(-input.getTurn() * m_rotation_thrust_force), (float)(-input.getPitch() * m_rotation_thrust_force), 0);
	D3DXVECTOR3 aft_rot_force(-fore_rot_force);
	
	// Stabilizing thrust calculations
	D3DXVECTOR3 stabilizer_force(-m_angular_momentum.x * m_stabilizer_ratio, -m_angular_momentum.y * m_stabilizer_ratio, -m_angular_momentum.z * m_stabilizer_ratio);

	// Thruster positions
	D3DXVECTOR3 fore_thruster_pos_adj, aft_thruster_pos_adj;
	D3DXVECTOR3 main_thrust_adj, fore_thrust_adj, aft_thrust_adj;
	
	// Pos transforms
	D3DXVec3Rotate(&fore_thruster_pos_adj, &forward_rot_thruster, &m_orientation);
	D3DXVec3Rotate(&aft_thruster_pos_adj, &reverse_rot_thruster, &m_orientation);

	// Thrust transforms
	D3DXVec3Rotate(&main_thrust_adj, &main_thrust_force, &m_orientation);
	D3DXVec3Rotate(&fore_thrust_adj, &fore_rot_force, &m_orientation);
	D3DXVec3Rotate(&aft_thrust_adj, &aft_rot_force, &m_orientation);

	// MOVEMENT
	// Main thruster
	applyLinearImpulse(main_thrust_adj, 0.1f);

	// ROTATION
	// Forward rotation thruster
	applyImpulse(fore_thrust_adj, m_pos + fore_thruster_pos_adj, 0.1f);
	// Rear rotation thruster
	applyImpulse(aft_thrust_adj, m_pos + aft_thruster_pos_adj, 0.1f);
	
	// DAMPING
	// Rotation damping
	applyAngularImpulse(stabilizer_force, 0.1f);
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
