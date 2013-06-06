/*
 * S_Resource.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Resource.h>
#include <server/Globals.h>

using namespace shared::utils;
using namespace server::entities::resource;

S_Resource::S_Resource() :
	Entity(RESOURCE),
	Resource(),
	ServerEntity(1, 0.0, calculateRotationalInertia(10)),
	m_carrier(NULL),
	m_dropTimeoutStart(0),
	m_onDropTimeout(false),
	m_droppedFrom(-1),
	m_startTravelTime(-1),
	m_spot(-1),
	m_dropTimeoutLength(0)
{
	m_radius = 3;
}

S_Resource::S_Resource(D3DXVECTOR3 pos, Quaternion orientation) :
	Entity(genId(), RESOURCE, pos, orientation, 3),
	Resource(),
	ServerEntity(1, 0.0, calculateRotationalInertia(10)),
	m_carrier(NULL),
	m_dropTimeoutStart(0),
	m_onDropTimeout(false),
	m_droppedFrom(-1),
	m_startTravelTime(-1),
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
		y = m_carrier->m_radius*.5f;
		z = m_carrier->m_length/4 - (m_spot%(m_carrier->m_resourceSpots/2))*(m_carrier->m_length/8);

		D3DXVECTOR3 lockPos = m_carrier->m_pos+ D3DXVECTOR3(x,y,z);

		long currDiff = GetTickCount() - m_startTravelTime;
		D3DXVECTOR3 dis = lockPos - m_pos;
		Quaternion qDiff = m_carrier->m_orientation - m_orientation;

		if(currDiff >= max_travel_time || m_startTravelTime == -1 || D3DXVec3Length(&dis) < 0.5f) {
			m_pos = lockPos;
			m_startTravelTime = -1;
			m_orientation = m_carrier->m_orientation;
		}
		else {	
			m_pos += dis*(((float)currDiff)/max_travel_time);
			m_orientation += qDiff*(((float)currDiff)/max_travel_time);
		}
	}
	else if(m_carrier->m_type == SHIP) {
		Quaternion q1, q2;
		D3DXQuaternionConjugate( &q1, &(-m_carrier->m_orientation)  );
		q2 = q1 * Quaternion(0,m_carrier->m_radius*.75f,0,1.0f) * (-m_carrier->m_orientation);
		D3DXVECTOR3 lockPos = m_carrier->m_pos+ D3DXVECTOR3(q2.x,q2.y,q2.z);

		long currDiff = GetTickCount() - m_startTravelTime;
		D3DXVECTOR3 dis = lockPos - m_pos;
		Quaternion qDiff = m_carrier->m_orientation - m_orientation;

		if(currDiff >= max_travel_time || m_startTravelTime == -1 || D3DXVec3Length(&dis) < 0.5f) {
			m_pos = lockPos;
			m_startTravelTime = -1;
			m_orientation = m_carrier->m_orientation;
		}
		else {
			m_pos += dis*(((float)currDiff)/max_travel_time);
			m_orientation += qDiff*(((float)currDiff)/max_travel_time);
		}
	}

	recalculateRelativeValues();
}

void S_Resource::update(float delta_time){
	m_enableIdentifiers = m_carrier == NULL;
	if(m_onDropTimeout) {
		if(GetTickCount() - m_dropTimeoutStart > m_dropTimeoutLength) {
			m_dropTimeoutStart = 0;
			m_onDropTimeout = 0;
			m_dropTimeoutLength = 0;
			m_droppedFrom = -1;
		}
	}

	if(m_carrier == NULL  && m_heldBy.size() > 0) {
		ServerEntity::update(delta_time);
	}
	else if(m_carrier == NULL) {

		float mag_velocity = D3DXVec3Length(&m_velocity), mag_angular_velocity = D3DXVec3Length(&m_angular_velocity);

		if (mag_velocity > FP_ZERO) {
		D3DXVECTOR3 lin_stabilizer_vec(-m_momentum);
		D3DXVec3Normalize(&lin_stabilizer_vec, &lin_stabilizer_vec);
		
		D3DXVECTOR3 lin_stabilizer_force = lin_stabilizer_vec * 100;
		applyLinearImpulse(Vec3ComponentAbsMin(lin_stabilizer_force, -m_momentum));
		}
		
		if (mag_angular_velocity > FP_ZERO) {
		D3DXVECTOR3 rot_stabilizer_vec(-m_angular_momentum);
		D3DXVec3Normalize(&rot_stabilizer_vec, &rot_stabilizer_vec);

		D3DXVECTOR3 rot_stabilizer_force = rot_stabilizer_vec * 100;
		applyAngularImpulse(Vec3ComponentAbsMin(rot_stabilizer_force, -m_angular_momentum));
		}

		ServerEntity::update(delta_time);

	}
	else if(m_carrier->m_type == EXTRACTOR) {
		if(D3DXVec3Length(&(m_pos-m_carrier->m_pos)) > ((S_Extractor *)m_carrier)->m_hubRadius){
			m_carrier = NULL;
		}
		ServerEntity::update(delta_time);
	}
	else if(m_carrier->m_type == MOTHERSHIP || m_carrier->m_type == SHIP) {
		travel();
	}
	else {
		ServerEntity::update(delta_time);
	}
}