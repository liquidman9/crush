/*
 * S_Resource.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Resource.h>

int S_Resource::s_maxTravelFrames = 500;

S_Resource::S_Resource() :
	Entity(RESOURCE),
	Resource(),
	ServerEntity(10, 0.0, calculateRotationalInertia(10)),
	m_carrier(NULL),
	m_dropTimeoutStart(0),
	m_onDropTimeout(false),
	m_droppedFrom(-1),
	m_travelFrames(-1),
	m_spot(-1),
	m_dropTimeoutLength(0)
{
	m_radius = 3;
}

S_Resource::S_Resource(D3DXVECTOR3 pos, Quaternion orientation) :
	Entity(genId(), RESOURCE, pos, orientation, 3),
	Resource(),
	ServerEntity(10, 0.0, calculateRotationalInertia(10)),
	m_carrier(NULL),
	m_dropTimeoutStart(0),
	m_onDropTimeout(false),
	m_droppedFrom(-1),
	m_travelFrames(-1),
	m_spot(-1),
	m_dropTimeoutLength(0)
{	
}

D3DXMATRIX S_Resource::calculateRotationalInertia(float mass){
	float radius_squared = 1;
	return *D3DXMatrixScaling(&D3DXMATRIX(), (2.0f / 5.0f) * mass * radius_squared,
											 (2.0f / 5.0f) * mass * radius_squared,
											 (2.0f / 5.0f) * mass * radius_squared);
};



void S_Resource::travel() {
	if(m_carrier->m_type == MOTHERSHIP){
		// ASSUMING the mothership is immovable
		float x = 0, y = 0, z = 0;
		if(m_spot < m_carrier->m_resourceSpots/2) x -= m_carrier->m_radius/2;
		else x+= m_carrier->m_radius/2;
		y = m_carrier->m_radius*.25f;
		z = m_carrier->m_length/4 - (m_spot%(m_carrier->m_resourceSpots/2))*(m_carrier->m_length/8);
		//cout<<"X: "<<x<<"Z: "<<z<<endl;
		//x = 0; z = 0;//tmp
		D3DXVECTOR3 lockPos = m_carrier->m_pos+ D3DXVECTOR3(x,y,z);

		if(m_travelFrames >= s_maxTravelFrames || m_travelFrames == -1) {
			m_pos = lockPos;
			m_travelFrames = -1;
			m_orientation = m_carrier->m_orientation;
		}
		else {
			D3DXVECTOR3 dis = lockPos - m_pos;
			m_pos += dis*(((float)m_travelFrames)/s_maxTravelFrames);
			m_travelFrames++;
		}
	}
	else if(m_carrier->m_type == SHIP) {
		Quaternion q1, q2;
		D3DXQuaternionConjugate( &q1, &(-m_carrier->m_orientation)  );
		q2 = q1 * Quaternion(0,m_carrier->m_radius*.75f,0,1.0f) * (-m_carrier->m_orientation);
		D3DXVECTOR3 lockPos = m_carrier->m_pos+ D3DXVECTOR3(q2.x,q2.y,q2.z);

		if(m_travelFrames >= s_maxTravelFrames || m_travelFrames == -1) {
			m_pos = lockPos;
			m_travelFrames = -1;
			m_orientation = m_carrier->m_orientation;
		}
		else {
			D3DXVECTOR3 dis = lockPos - m_pos;
			m_pos += dis*(((float)m_travelFrames)/s_maxTravelFrames);
			m_travelFrames++;
		}
	}

	recalculateRelativeValues();
}

void S_Resource::update(float delta_time){
	if(m_onDropTimeout) {
		if(GetTickCount() - m_dropTimeoutStart > m_dropTimeoutLength) {
			m_dropTimeoutStart = 0;
			m_onDropTimeout = 0;
			m_droppedFrom = -1;
		}
	}

	if(m_carrier == NULL || m_carrier->m_type == EXTRACTOR) {
		ServerEntity::update(delta_time);
	}
	else if(m_carrier->m_type == MOTHERSHIP || m_carrier->m_type == SHIP) {
		travel();
	}
	else {
		ServerEntity::update(delta_time);
	}
}