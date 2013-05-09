/*
 * S_Ship.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/util/SharedUtils.h>
#include <shared/ConfigSettings.h>
#include <shared/game/Entity.h>
#include <server/game/S_Ship.h>
//#include <server/game/S_TractorBeam.h>

using namespace shared::utils;
using namespace server::entities::ship;

mass = 10000.0f;
forward_impulse = 5000.0f;
rotation_impulse = 3000.0f;
braking_impulse = 2500.0f;
max_velocity = 50.0f;
max_rotation_velocity = 2.5f;

S_Ship::S_Ship() :
	Entity(SHIP),
	Ship(),
	ServerEntity(),
	m_forward_impulse(forward_impulse),
	m_rotation_impulse(rotation_impulse),
	m_max_velocity(max_velocity),
	m_max_rotation_velocity(max_rotation_velocity),
	m_resource(NULL)
{
	init();
}

S_Ship::S_Ship(D3DXVECTOR3 pos, Quaternion orientation, int pNum) :
	Entity(genId(), SHIP, pos, orientation),
	Ship(pNum),
	ServerEntity(mass, calculateRotationalInertia(mass), 5.0, 1.0),
	m_forward_impulse(forward_impulse),
	m_rotation_impulse(rotation_impulse),
	m_braking_impulse(braking_impulse),
	m_max_velocity(max_velocity),
	m_max_rotation_velocity(max_rotation_velocity),
	m_resource(NULL)
{	
	init();
}

void S_Ship::init() {
	forward_rot_thruster = D3DXVECTOR3(0, 0, 5);
	reverse_rot_thruster = D3DXVECTOR3(0, 0, -5);
	m_resourceSpots = 1;
}

// TODO!!!:
// This method needs to be extracted to the server/physics engine.
void S_Ship::addPlayerInput(InputState input) {
	
	//m_tractorBeamOn = (float) input.getTractorBeam();
	m_tractorBeam->m_isOn = (input.getTractorBeam() != 0 || input.getPushBurst()); //tmp

	if(input.getPushBurst()) {
		m_tractorBeam->m_isPulling = false;
		m_tractorBeam->m_strength = 1.0f;
	}
	else {
		m_tractorBeam->m_isPulling = true;
		m_tractorBeam->m_strength = (float) input.getTractorBeam();
	}

	m_thruster = input.getThrust();
	// Linear thrust calculations

	if (abs(input.getThrust()) > FP_ZERO) {
		m_thrusting = true;
		D3DXVECTOR3 main_thrust_force(0, 0, (float)m_thruster), 
					main_thrust_adj;
		D3DXVec3Rotate(&main_thrust_adj, &main_thrust_force, &m_orientation);
		
		applyLinearImpulse(main_thrust_adj * m_forward_impulse);
	}

	// Rotational thrust calculations
	if (abs(input.getPitch()) > FP_ZERO || abs(input.getTurn()) > FP_ZERO) {
		m_rotating = true;
		D3DXVECTOR3 rot_thrust_adj((float)input.getPitch(), (float)input.getTurn(), 0);
		D3DXVec3Normalize(&rot_thrust_adj, &rot_thrust_adj);
		D3DXVec3Rotate(&rot_thrust_adj, &rot_thrust_adj, &m_orientation);

		applyAngularImpulse(rot_thrust_adj * m_rotation_impulse);
	}
}

void S_Ship::applyDamping() {

	// Linear damping
	float mag_velocity = D3DXVec3Length(&m_velocity), mag_angular_velocity = D3DXVec3Length(&m_angular_velocity);
	if (mag_velocity > FP_ZERO) {
		// Linear stabilizer
		D3DXVECTOR3 lin_stabilizer_vec(-m_momentum);
		D3DXVec3Normalize(&lin_stabilizer_vec, &lin_stabilizer_vec);

		if (m_thrusting) {
			// Thrusting, we only want to reduce the impulse
			D3DXVECTOR3 lin_stabilizer_force = lin_stabilizer_vec * m_forward_impulse;
			float damping_factor = (mag_velocity / m_max_velocity);
			applyLinearImpulse(lin_stabilizer_force * damping_factor);
		} else {
			// Not thrusting, we need to slow down as quickly as possible
			D3DXVECTOR3 lin_stabilizer_force = lin_stabilizer_vec * m_braking_impulse;
			applyLinearImpulse(Vec3ComponentAbsMin(lin_stabilizer_force, -m_momentum));
		}
	}

	// Rotation damping
	if (mag_angular_velocity > FP_ZERO) {
		// Rotation stabilizer
		D3DXVECTOR3 rot_stabilizer_vec(-m_angular_momentum);
		D3DXVec3Normalize(&rot_stabilizer_vec, &rot_stabilizer_vec);

		D3DXVECTOR3 rot_stabilizer_force = rot_stabilizer_vec * m_rotation_impulse;
		
		if (m_rotating) {
			// Rotating, we only want to reduce the impulse
			float damping_factor = (mag_angular_velocity / m_max_rotation_velocity);
			applyAngularImpulse(rot_stabilizer_force * damping_factor);
		} else {
			// Not rotating, we need to slow down as quickly as possible
			applyAngularImpulse(Vec3ComponentAbsMin(rot_stabilizer_force, -m_angular_momentum));
		}
	}

	m_thrusting = false;
	m_rotating = false;

}

void S_Ship::update(float delta_time) {
	applyDamping();
	ServerEntity::update(delta_time);
}

void S_Ship::calcTractorBeam() {

	m_tractorBeam->updateData();
}

D3DXVECTOR3 S_Ship::calculateRotationalInertia(float mass){
	float radius_squared = 5 * 5;
	float height_squared = (2 * 5.0f) * (2 * 5.0f);
	return D3DXVECTOR3( (1.0f / 12.0f) * mass * (3 * radius_squared + height_squared),
						(1.0f / 12.0f) * mass * (3 * radius_squared + height_squared),
						(0.5f) * mass * radius_squared);
};


bool S_Ship::interact(S_Resource * res) {
	if(m_resource == NULL && res->m_carrier == NULL) {
		if(((res->m_droppedFrom != m_playerNum && res->m_onDropTimeout) || res->m_droppedFrom != m_playerNum)) {
			m_resource = res;
			res->m_carrier = this;
			res->m_onDropTimeout = false;
			res->m_dropTimeoutStart = 0;
			res->m_droppedFrom = -1;
			res->m_spot = 0;
			res->m_travelFrames = 0;
			cout<<"Gathered"<<endl;
		}
		return true;
	}

	return false;
}

void S_Ship::interact(S_Asteroid * asteroid) {
	if(m_resource != NULL) {
		S_Resource * tmp = m_resource;
		m_resource = NULL;
		tmp->m_carrier = NULL;
		tmp->m_onDropTimeout = true;
		tmp->m_dropTimeoutStart = GetTickCount();
		tmp->m_droppedFrom = m_playerNum;
		tmp->m_spot = -1;
		tmp->reset(); // temporary to stop resources from moving far far away when dropped
	}
}

void S_Ship::interact(S_Ship * ship) {
	if(m_resource != NULL) {
		S_Resource * tmp = m_resource;
		m_resource = NULL;
		tmp->m_carrier = NULL;
		tmp->m_onDropTimeout = true;
		tmp->m_dropTimeoutStart = GetTickCount();
		tmp->m_droppedFrom = m_playerNum;
		tmp->m_spot = -1;
		tmp->reset(); // temporary to stop resources from moving far far away when dropped
	}
}

void S_Ship::print() {
	cout << "Pos: "; printVec(m_pos); cout << endl;
	cout << "Velocity: " << D3DXVec3Length(&m_velocity) << endl;
	cout << "Rotation: " << D3DXVec3Length(&m_angular_velocity) << endl;
}
