/*
 * S_Asteroid.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Asteroid.h>

int S_Asteroid::s_range = 20;
float S_Asteroid::s_start = .25;

int massConv = 250;
int radiusConv = 2;

S_Asteroid::S_Asteroid() :
	Entity(ASTEROID),
	Asteroid(),
	ServerEntity(m_mass = ((m_radius = (m_scale = (rand() % s_range) + s_start)*radiusConv) * massConv), calculateRotationalInertia(m_mass), 1.0, 1.0)
{
	m_radius = (m_scale)*radiusConv;
}

S_Asteroid::S_Asteroid(D3DXVECTOR3 pos, Quaternion orientation, float scale) :
	Entity(genId(), ASTEROID, pos, orientation),
	Asteroid(),
	ServerEntity(m_mass = ((m_radius = (m_scale = scale)*radiusConv) * massConv), calculateRotationalInertia(m_mass), 1.0, 1.0)
{	
	m_radius = (m_scale)*radiusConv;
	cout<<"Mass: "<<m_mass<<" Radius: "<<m_radius<<" Scale: "<<m_scale<<endl;
}



D3DXVECTOR3 S_Asteroid::calculateRotationalInertia(float mass){
	float radius_squared = m_radius;
	return D3DXVECTOR3( (2.0f / 5.0f) * mass * radius_squared,
						(2.0f / 5.0f) * mass * radius_squared,
						(2.0f / 5.0f) * mass * radius_squared);
};

