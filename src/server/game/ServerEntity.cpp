#include <shared/game/Entity.h>
#include <server/game/ServerEntity.h>
#include <shared/util/SharedUtils.h>
#include <server/game/S_Ship.h>
#include <server/game/S_TractorBeam.h>
float FP_ZERO = 0.000001f;
D3DXVECTOR3 zero_vec(0.0f, 0.0f, 0.0f);

int ServerEntity::s_id_gen = 0;

ServerEntity::ServerEntity() :
	m_mass(1),
	m_mass_inverse(1),
	m_rot_inertia(*D3DXMatrixIdentity(&D3DXMATRIX())),
	m_rot_inertia_inverse(*D3DXMatrixInverse(&m_rot_inertia_inverse, NULL, &m_rot_inertia)),
	m_immovable(false),
	m_resourceSpots(0),
	m_holder(NULL),
	
	// zeroing values
	m_angular_velocity(shared::utils::VEC3_ZERO),
	m_orientation_delta(0.0f, 0.0f, 0.0f, 0.0f),
	m_momentum(shared::utils::VEC3_ZERO),
	m_angular_momentum(shared::utils::VEC3_ZERO),
	t_impulse(shared::utils::VEC3_ZERO),
	t_angular_impulse(shared::utils::VEC3_ZERO)
{ }


ServerEntity::ServerEntity(float mass, D3DXMATRIX rot_inertia) :
	m_mass(mass),
	m_mass_inverse(1/mass),
	m_rot_inertia(rot_inertia),
	m_rot_inertia_inverse(*D3DXMatrixInverse(&m_rot_inertia_inverse, NULL, &m_rot_inertia)),
	m_immovable(false),
	m_resourceSpots(0),
	m_holder(NULL),

	// zeroing values
	m_angular_velocity(shared::utils::VEC3_ZERO),
	m_orientation_delta(0.0f, 0.0f, 0.0f, 0.0f),
	m_momentum(shared::utils::VEC3_ZERO),
	m_angular_momentum(shared::utils::VEC3_ZERO),
	t_impulse(shared::utils::VEC3_ZERO),
	t_angular_impulse(shared::utils::VEC3_ZERO)
{
	recalculateRelativeValues();
}


ServerEntity::ServerEntity(float mass, float length, D3DXMATRIX rot_inertia) :
	m_mass(mass),
	m_mass_inverse(1/mass),
	m_rot_inertia(rot_inertia),
	m_rot_inertia_inverse(*D3DXMatrixInverse(&m_rot_inertia_inverse, NULL, &m_rot_inertia)),
	m_length(length),
	m_immovable(false),
	m_resourceSpots(0),
	m_destroy(false),
	m_holder(NULL),
	// zeroing values
	m_angular_velocity(shared::utils::VEC3_ZERO),
	m_orientation_delta(0.0f, 0.0f, 0.0f, 0.0f),
	m_momentum(shared::utils::VEC3_ZERO),
	m_angular_momentum(shared::utils::VEC3_ZERO),
	t_impulse(shared::utils::VEC3_ZERO),
	t_angular_impulse(shared::utils::VEC3_ZERO)
{ 
	recalculateRelativeValues();
}

int ServerEntity::genId() {
	return s_id_gen++;
}

// Physics Methods
// Applies a force to this entity's center of mass (will not cause rotation)
void ServerEntity::applyLinearImpulse(D3DXVECTOR3 linear_impulse) {
	// Linear calculation
	t_impulse += linear_impulse;
}

// Applies a rotational force to this entity (will not cause linear motion)
void ServerEntity::applyAngularImpulse(D3DXVECTOR3 angular_impulse) {
	// Linear calculation
	t_angular_impulse += angular_impulse;
}

// Applies a force to this entity at the given point
void ServerEntity::applyImpulse(D3DXVECTOR3 impulse, D3DXVECTOR3 point) {
	// Linear calculation
	t_impulse += impulse;

	// Rotational calcultation
	D3DXVECTOR3 angular_impulse, vector_to_point = point - m_pos;
	D3DXVec3Cross(&angular_impulse, &vector_to_point, &impulse); // Cross product finds torque
	t_angular_impulse += angular_impulse;
	if (DEBUG) {
		cout << "Pos: " << m_pos << endl;
		cout << "Point: " << point << endl;
		cout << "Angular impulse: " << angular_impulse << endl;
	}
}

void ServerEntity::recalculateRelativeValues() {
	D3DXVECTOR4 temp;
	D3DXMATRIX mat_rotate;
	// Generate rotation matrix
	D3DXMatrixRotationQuaternion(&mat_rotate, &m_orientation);

	// calculate front/back points	
	D3DXVECTOR3 front(0.0f, 0.0f, m_length), 
				back(0.0f, 0.0f, -m_length);

	D3DXVec3Transform(&temp, &front, &mat_rotate);
	m_pFront = D3DXVECTOR3(temp.x, temp.y, temp.z);
	m_pFront += m_pos;

	D3DXVec3Transform(&temp, &back, &mat_rotate);
	m_pBack = D3DXVECTOR3(temp.x, temp.y, temp.z);
	m_pBack += m_pos;

	/*
	// calculate oriented moment of inertia
	//mat_transform = mat_rotate * mat_translate;
	m_rot_inertia = mat_rotate * base_rotational_inertia * mat_rotate;
	//D3DXMatrixInverse(&m_rot_inertia_inverse, NULL, &m_rot_inertia);
	m_rot_inertia_inverse = mat_rotate * base_rotational_inertia_inverse * mat_rotate;
	*/
}

// Applies physics to entity
void ServerEntity::update(float delta_time) {
	// Apply current frame's physics
	// Apply impulse
	m_momentum += t_impulse;
	m_angular_momentum += t_angular_impulse;
	
	// Reset temporary values
	t_impulse = shared::utils::VEC3_ZERO;
	t_angular_impulse = shared::utils::VEC3_ZERO;

	// Calculate velocity/angular velocity (second-order dependent values)
	m_velocity = m_momentum * m_mass_inverse;

	D3DXVECTOR4 temp;
	D3DXVec3Transform(&temp, &m_angular_momentum, &m_rot_inertia_inverse);
	m_angular_velocity = D3DXVECTOR3(temp.x, temp.y, temp.z);

	if (D3DXVec3LengthSq(&m_angular_velocity) > FP_ZERO) {
		m_orientation_delta = 0.5 *(m_orientation * Quaternion(m_angular_velocity.x, m_angular_velocity.y, m_angular_velocity.z, 0.0f));
	} else {
		m_orientation_delta = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	// Moves according to last frame's values
	m_pos += m_velocity * delta_time;
	m_orientation += (m_orientation_delta * delta_time);
	D3DXQuaternionNormalize(&m_orientation, &m_orientation); // Orientation must always be normalized!

	recalculateRelativeValues();
}

D3DXVECTOR3 ServerEntity::getVelocity() {
	return m_velocity;
}


void ServerEntity::rotate(float angle, D3DXVECTOR3 axis){
	D3DXQuaternionRotationAxis(&m_orientation, &axis, angle);
}

void ServerEntity::move(D3DXVECTOR3 vector){
	m_pos += vector;
}

void ServerEntity::reset() {
	// Derived Variables
	m_velocity = zero_vec;
	m_angular_velocity = zero_vec;
	m_orientation_delta = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);

	// Calculated Values
	m_momentum = zero_vec;
	m_angular_momentum = zero_vec;

	// Accumulator Values
	t_impulse = zero_vec;
	t_angular_impulse = zero_vec;

	m_destroy = false;
}

void const ServerEntity::print() {
	cout << "Type: " << (int)m_type << endl;
	cout << "Pos: " << m_pos << endl;
	cout << "Velocity: " << D3DXVec3Length(&m_velocity) << endl;
	cout << "Rotation: " << D3DXVec3Length(&m_angular_velocity) << endl;
}
