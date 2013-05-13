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
