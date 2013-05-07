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
	ServerEntity((m_scale = (rand() % m_range) + m_start)*.05, calculateRotationalInertia(m_scale*5), 1.0, 1.0)
{

}

S_Asteroid::S_Asteroid(D3DXVECTOR3 pos, Quaternion orientation) :
	Entity(genId(), ASTEROID, pos, orientation),
	Asteroid(),
	ServerEntity((m_scale = (float)(rand() % m_range) + m_start)*5, calculateRotationalInertia(m_scale*5), 1.0, 1.0)
{	

}



D3DXVECTOR3 S_Asteroid::calculateRotationalInertia(float mass){
	float radius_squared = m_scale;
	float height_squared = m_scale;
	return D3DXVECTOR3( (1.0f / 12.0f) * mass * (3 * radius_squared + height_squared),
						(0.5f) * mass * radius_squared,
						(1.0f / 12.0f) * mass * (3 * radius_squared + height_squared));
};

