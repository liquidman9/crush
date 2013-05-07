/*
 * S_Asteroid.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Asteroid.h>

static int m_range = 300, m_start = 20;

S_Asteroid::S_Asteroid() :
	Entity(ASTEROID),
	Asteroid(),
	ServerEntity((m_scale = (rand() % m_range) + m_start) * 5, calculateRotationalInertia(m_scale * 5), m_scale/10, 1.0)
{

}

S_Asteroid::S_Asteroid(D3DXVECTOR3 pos, Quaternion orientation) :
	Entity(genId(), ASTEROID, pos, orientation),
	Asteroid(),
	ServerEntity((m_scale = (float)(rand() % m_range) + m_start) * 5, calculateRotationalInertia(m_scale * 5), m_scale/10, 1.0)
{	

}



D3DXVECTOR3 S_Asteroid::calculateRotationalInertia(float mass){
	float radius_squared = m_scale;
	return D3DXVECTOR3( (2.0f / 5.0f) * mass * radius_squared,
						(2.0f / 5.0f) * mass * radius_squared,
						(2.0f / 5.0f) * mass * radius_squared);
};

