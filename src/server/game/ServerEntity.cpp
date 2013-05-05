#include <shared/game/Entity.h>
#include <server/game/ServerEntity.h>

float FP_ZERO = 0.000001f;
D3DXVECTOR3 zero_vec(0.0f, 0.0f, 0.0f);

int ServerEntity::s_id_gen = 0;

ServerEntity::ServerEntity() :

	m_mass(1),
	m_mass_inverse(1),
	m_radius(1),
	m_immovable(false)
{ 
	reset();
}


ServerEntity::ServerEntity(float mass, D3DXVECTOR3 rot_inertia) :
	m_mass(mass),
	m_mass_inverse(1/mass),
	m_radius(1),
	m_immovable(false)
{ 
	reset();
}


ServerEntity::ServerEntity(float mass, D3DXVECTOR3 rot_inertia, float length, float elastic) :
	m_mass(mass),
	m_mass_inverse(1/mass),
	m_rot_inertia(rot_inertia),
	m_rot_inertia_inverse(1/rot_inertia.x, 1/rot_inertia.y, 1/rot_inertia.z),
	m_radius(3),
	m_length(length),
	// zeroing values
	m_angular_velocity(shared::utils::VEC3_ZERO),
	m_orientation_delta(0.0f, 0.0f, 0.0f, 0.0f),
	m_momentum(shared::utils::VEC3_ZERO),
	m_angular_momentum(shared::utils::VEC3_ZERO),
	t_impulse(shared::utils::VEC3_ZERO),
	t_angular_impulse(shared::utils::VEC3_ZERO),
	m_pFront(m_pos.x, m_pos.y, m_pos.z + length),
	m_pBack(m_pos.x, m_pos.y, m_pos.z - length),
	m_elastic(elastic),
	m_immovable(false)
{ }

int ServerEntity::genId() {
	return s_id_gen++;
}

// Physics Methods
// Applies a force to this entity's center of mass for the given amount of time(will not cause rotation)
void ServerEntity::applyLinearImpulse(D3DXVECTOR3 force, float duration) {
	// Linear calculation
	t_impulse += force * duration;
}

// Applies a rotational force to this entity (will not cause linear motion)
void ServerEntity::applyAngularImpulse(D3DXVECTOR3 torque, float duration) {
	// Linear calculation
	t_angular_impulse += torque * duration;
}

// Applies a force to this entity at the given point for the given amount of time
void ServerEntity::applyImpulse(D3DXVECTOR3 force, D3DXVECTOR3 point, float duration) {
	// Linear calculation
	t_impulse += force * duration;

	// Rotational calcultation
	D3DXVECTOR3 torque, vector_to_point = point - m_pos;
	D3DXVec3Cross(&torque, &force, &vector_to_point); // Cross product finds torque
	t_angular_impulse += torque * duration;
}

// Applies current acceleration to Entity
void ServerEntity::update(float delta_time) {
	// Move according to last known values for half time
	float half_time = (delta_time / 2);
	m_pos += m_velocity * half_time;
	m_orientation += m_orientation_delta * half_time;
	D3DXQuaternionNormalize(&m_orientation, &m_orientation);

	// Apply impulse
	m_momentum += t_impulse;
	m_angular_momentum += t_angular_impulse;
	
	// Reset temporary values
	t_impulse = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	t_angular_impulse = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Calculate velocity/angular velocity
	m_velocity = m_momentum * m_mass_inverse;
	m_angular_velocity = D3DXVECTOR3(m_angular_momentum.x * m_rot_inertia_inverse.x, 
									 m_angular_momentum.y * m_rot_inertia_inverse.y,
									 m_angular_momentum.z * m_rot_inertia_inverse.z);
	if (D3DXVec3LengthSq(&m_angular_velocity) > FP_ZERO) {
		m_orientation_delta = 0.5 *(m_orientation * Quaternion(m_angular_velocity.x, m_angular_velocity.y, m_angular_velocity.z, 0.0f));
	} else {
		m_orientation_delta = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
	}

	m_pos += m_velocity * half_time;
	m_orientation += (m_orientation_delta * half_time);
	D3DXQuaternionNormalize(&m_orientation, &m_orientation);
	D3DXQuaternionNormalize(&m_orientation, &m_orientation);

	D3DXMATRIX mat_rotate;
	D3DXVECTOR4 temp;
	D3DXVECTOR3 front(0.0f, 0.0f, m_length), back(0.0f, 0.0f, -m_length);
	D3DXMatrixRotationQuaternion(&mat_rotate, D3DXQuaternionNormalize(&m_orientation, &m_orientation));

	// calculate front/back points
	D3DXVec3Transform(&temp, &front, &mat_rotate);
	m_pFront = D3DXVECTOR3(temp.x, temp.y, temp.z);
	m_pFront += m_pos;

	D3DXVec3Transform(&temp, &back, &mat_rotate);
	m_pBack = D3DXVECTOR3(temp.x, temp.y, temp.z);
	m_pBack += m_pos;
}

// Getters/Setters
void ServerEntity::setVelocity(D3DXVECTOR3 velocity) {
	m_velocity = velocity;
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
	cout << "Resetting!" << endl;

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
}
