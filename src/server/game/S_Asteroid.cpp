/*
 * S_Asteroid.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Asteroid.h>


using namespace server::entities::asteroid;
int server::entities::asteroid::numAsteroids = 50;
float server::entities::asteroid::startMass = 0.25f;
int server::entities::asteroid::rangeMass = 20;
float server::entities::asteroid::startPos = -200.0f;
int server::entities::asteroid::rangePos = 400;
float server::entities::asteroid::scaleToRadius = 2.0f;
float server::entities::asteroid::radiusToMass = 250.0f;



S_Asteroid::S_Asteroid() :
	Entity(ASTEROID),
	Asteroid(),
	ServerEntity(m_mass = ((m_radius = (m_scale = (rand() % rangeMass) + startMass)*scaleToRadius) * radiusToMass), calculateRotationalInertia(m_mass), 1.0, 1.0)
{
	m_radius = (m_scale)*scaleToRadius;
}

S_Asteroid::S_Asteroid(D3DXVECTOR3 pos, Quaternion orientation, float scale) :
	Entity(genId(), ASTEROID, pos, orientation),
	Asteroid(),
	ServerEntity(m_mass = ((m_radius = (m_scale = scale)*scaleToRadius) * radiusToMass), calculateRotationalInertia(m_mass), 1.0, 1.0)
{	
	m_radius = (m_scale)*scaleToRadius;
	cout<<"Mass: "<<m_mass<<" Radius: "<<m_radius<<" Scale: "<<m_scale<<endl;
}

// assumes srand() call
float S_Asteroid::generateSize() {
	return (rand() % server::entities::asteroid::rangeMass) + server::entities::asteroid::startMass;
}

void S_Asteroid::reCreateAsteroid(float boundaryRadius) {
	srand(GetTickCount());

	cout<<"Recreating"<<endl;
	m_scale = generateSize();
	m_radius = m_scale*scaleToRadius;
	m_mass = m_radius*radiusToMass;
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
	float r = boundaryRadius - m_radius*1.5;
	float x, y, z, tmp1, tmp2;
	z = (rand()%3)-1;
	tmp1 = sqrt(1 - z*z);
	tmp2 = (((float)rand())/((float)RAND_MAX))*2*3.14159265359;
	x = tmp1 * cos(tmp2);
	y = tmp2 * sin(tmp2);

	m_pos = D3DXVECTOR3(x*r, y*r, z*r);

	// Calculate Directional Force
	float x0, y0, z0, tmp10, tmp20;
	z0 = (rand()%3)-1;
	tmp10 = sqrt(1 - z0*z0);
	tmp20 = (((float)rand())/((float)RAND_MAX))*2*3.14159265359;
	x0 = tmp1 * cos(tmp2);
	y0 = tmp2 * sin(tmp2);
	
	D3DXVECTOR3 dirr = D3DXVECTOR3(x0,y0,z0)*(boundaryRadius/2);
	D3DXVec3Normalize(&dirr, &(m_pos-dirr));
	applyLinearImpulse(dirr*((rand() % 20000*2)-20000)*25);


}

D3DXVECTOR3 S_Asteroid::calculateRotationalInertia(float mass){
	float radius_squared = m_radius;
	return D3DXVECTOR3( (2.0f / 5.0f) * mass * radius_squared,
						(2.0f / 5.0f) * mass * radius_squared,
						(2.0f / 5.0f) * mass * radius_squared);
};

