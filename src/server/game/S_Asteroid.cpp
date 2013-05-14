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
	ServerEntity(calculateMass(m_radius = scale *scaleToRadius, density), m_radius, calculateRotationalInertia(calculateMass(scale * scaleToRadius, density)))
{	
	m_radius = (m_scale)*scaleToRadius;
	cout << "Mass: "<< m_mass<< " Radius: " << m_radius << " Scale: " << m_scale << endl;
	//cout << m_rot_inertia << endl;
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
	m_rot_inertia_inverse = *D3DXMatrixInverse(&m_rot_inertia_inverse, NULL, &m_rot_inertia);
	m_length = 1.0;

	// zeroing values
	m_angular_velocity = shared::utils::VEC3_ZERO;
	m_orientation_delta = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
	m_momentum = shared::utils::VEC3_ZERO;
	m_angular_momentum =shared::utils::VEC3_ZERO;
	t_impulse = shared::utils::VEC3_ZERO;
	t_angular_impulse = shared::utils::VEC3_ZERO;
	recalculateRelativeValues();


	// Calculate Position
	float r = boundaryRadius - 1.0f;
	float x, y, z, tmp1, tmp2;
	z = shared::utils::rand_float(-1.0f, 2.0f);
	tmp1 = sqrt(1 - z*z);
	tmp2 = (((float)rand())/((float)RAND_MAX))*2*3.14159265359;
	x = tmp1 * cos(tmp2);
	y = tmp1 * sin(tmp2);

	m_pos = D3DXVECTOR3(x*r, y*r, z*r);

	// Calculate Directional Force
	 D3DXVECTOR3 goTo = D3DXVECTOR3(rand()%(int)(boundaryRadius/2),
		 rand()%(int)(boundaryRadius/2),
		rand()%(int)(boundaryRadius/2));

	
	D3DXVECTOR3 dirr = -(m_pos - goTo);
	D3DXVec3Normalize(&dirr, &dirr);
	applyLinearImpulse(dirr*(rand() % 20000*2)*25* .01f);


}
