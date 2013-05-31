/*
 * S_Asteroid.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Asteroid.h>
#include <server/Globals.h>

using namespace shared::utils;
using namespace server::entities::asteroid;

S_Asteroid::S_Asteroid(D3DXVECTOR3 pos, Quaternion orientation, float scale) :
	Entity(genId(), ASTEROID, pos, orientation),
	Asteroid(scale),
	ServerEntity(calculateMass(m_radius = scale *scaleToRadius, density), 0.0f, calculateRotationalInertia(calculateMass(scale * scaleToRadius, density)))
{	
	m_radius = (m_scale)*scaleToRadius;
	cout << "Mass: "<< m_mass<< " Radius: " << m_radius << " Scale: " << m_scale << endl;
}

float S_Asteroid::calculateMass(float radius, float density) {
	return (float)(density * 4/3 * PI * pow(radius, 3));
}

D3DXMATRIX S_Asteroid::calculateRotationalInertia(float mass) {
	float radius_squared = m_radius * m_radius;
	return *D3DXMatrixScaling(&D3DXMATRIX(), (2.0f / 5.0f) * mass * radius_squared,
											 (2.0f / 5.0f) * mass * radius_squared,
											 (2.0f / 5.0f) * mass * radius_squared);
}

// assumes srand() call
float S_Asteroid::generateSize() {
	return shared::utils::rand_float(server::world::asteroids_scale_start, server::world::asteroids_scale_range);
}

void S_Asteroid::reCreateAsteroid(float boundaryRadius) {
	srand(GetTickCount());

	m_scale = generateSize();
	m_radius = m_scale*scaleToRadius;
	m_mass = calculateMass(m_radius, density);
	m_mass_inverse = 1/m_mass;
	D3DXVECTOR3 rot_inertia = calculateRotationalInertia(m_mass);
	m_rot_inertia = *D3DXMatrixScaling(&m_rot_inertia, rot_inertia.x, rot_inertia.y, rot_inertia.z);
	D3DXMatrixInverse(&m_rot_inertia_inverse, NULL, &m_rot_inertia);
	m_length = 0.0;

	// zeroing values
	reset();
	recalculateRelativeValues();


	// Calculate Position
	float r = boundaryRadius - m_radius;
	m_pos = Vec3RandNormalized() * r;

	// Calculate Directional Force
	D3DXVECTOR3 goTo = Vec3RandRange(10.0f, boundaryRadius / 3);
	D3DXVECTOR3 initial_rot_vel = Vec3RandRange(-server::world::asteroids_rot_vel_range/2, server::world::asteroids_rot_vel_range);
	D3DXVECTOR4 temp;
	D3DXVec3Transform(&temp, &initial_rot_vel, &m_rot_inertia);
	initial_rot_vel = D3DXVECTOR3(temp.x, temp.y, temp.z);
	
	D3DXVECTOR3 dirr = -(m_pos - goTo);
	D3DXVec3Normalize(&dirr, &dirr);
	applyLinearImpulse(dirr * rand_float(5.0f, 15.0f)* m_mass);
	applyAngularImpulse(initial_rot_vel);


}
