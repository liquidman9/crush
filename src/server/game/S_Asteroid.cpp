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



D3DXVECTOR3 S_Asteroid::calculateRotationalInertia(float mass){
	float radius_squared = m_radius;
	return D3DXVECTOR3( (2.0f / 5.0f) * mass * radius_squared,
						(2.0f / 5.0f) * mass * radius_squared,
						(2.0f / 5.0f) * mass * radius_squared);
};

