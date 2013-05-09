/*
 * S_TractorBeam.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <server/game/S_TractorBeam.h>
//#include <server/game/S_Ship.h>

using namespace server::entities::tractorbeam;

static float s_gravitationalConstant =  gravity;
static float s_defaultLength = length;

using namespace server::entities::tractorbeam;

float server::entities::tractorbeam::gravity = 5.0f;
float server::entities::tractorbeam::length = 350.0f;

S_TractorBeam::S_TractorBeam(S_Ship * ship) :
	Entity(TRACTORBEAM),
	TractorBeam(ship->m_playerNum),
	ServerEntity(1000, D3DXVECTOR3(1, 1, 1), length, 1.0),// infinity
	m_strength(0),
	m_isPulling(true)
{
	m_radius = m_sentRadius;
	m_object = NULL;
	m_ship = ship;
	m_pFront = m_start;
	m_pBack = m_end;
}



bool S_TractorBeam::isLocked() {
	if(m_object != NULL) return true;
	else return false;
}

void S_TractorBeam::lockOn(ServerEntity * entity) {
	m_object = entity;
}

D3DXVECTOR3 S_TractorBeam::getCurrentDirection() {
//	if(!isLocked()) return D3DXVECTOR3(m_orientation.x, m_orientation.y, m_orientation.z); // use ships direction

	D3DXVECTOR3 range = m_end - m_start;
	D3DXVec3Normalize(&range, &range);
	return range;
}

D3DXVECTOR3 S_TractorBeam::getCurrentDistanceVector() {
//	if(!isLocked()) return D3DXVECTOR3(m_orientation.x, m_orientation.y, m_orientation.z); // use ships direction
	D3DXVECTOR3 range = m_end - m_start;
	return range;
}



D3DXVECTOR3 S_TractorBeam::getDistanceVectorOf(D3DXVECTOR3 b) {
	D3DXVECTOR3 range = b - m_start;
	return range;
}

float S_TractorBeam::getCurrentDistance() {
	if(!isLocked()) return s_defaultLength;

	return D3DXVec3Length(&(getCurrentDistanceVector()));
}

void S_TractorBeam::setStartPoint() {
	m_start = m_ship->m_pos;
	m_pFront = m_start;
}

void S_TractorBeam::setEndPoint() {
	if(!isLocked()) {
		Quaternion tmpq;
		D3DXMATRIX matRotate;
		D3DXVECTOR3 tmp = D3DXVECTOR3(0,0,1);
		D3DXMatrixRotationQuaternion(&matRotate, D3DXQuaternionNormalize(&tmpq, &m_ship->m_orientation));
		D3DXVec3TransformCoord(&tmp,&tmp,&matRotate);
		D3DXVec3Normalize( &tmp, &tmp );
		m_end = m_ship->m_pos + tmp * s_defaultLength;
	}
	else m_end = m_object->m_pos;

	m_pBack = m_end;
}

void S_TractorBeam::calculateForce() {
	// 1 is Ship, 2 is Object, thus pull - object, push + object
	if(isLocked()) {
		D3DXVECTOR3 disV = getCurrentDirection();
		float disL = getCurrentDistance();
		
		D3DXVECTOR3 force = m_strength*(s_gravitationalConstant*m_ship->m_mass*m_object->m_mass)*(disV)/(pow(disL, 2));
	
		if(m_isPulling) {
			m_ship->applyLinearImpulse(force * .01f);
			m_object->applyLinearImpulse(-force * .01f);
		}
		// Push
		else {
			m_ship->applyLinearImpulse(-force * .01f);
			m_object->applyLinearImpulse(force * .01f);
		}
	}

}


void S_TractorBeam::updateData() {
	setStartPoint();
	setEndPoint();
	//cout << "dis: "<<getCurrentDistance()<<" Dir: "<<getCurrentDirection().x<<" "<<getCurrentDirection().y<<" "<<getCurrentDirection().z<<endl;
	if(m_isOn){
		calculateForce();
	}
	else {
		m_object = NULL;
	}
}

void S_TractorBeam::update(float delta_time) {
	// do nothing during regular update cycle (not affected by physics)
}

D3DXVECTOR3 S_TractorBeam::calculateRotationalInertia(float mass){
	float radius_squared = 50;
	float height_squared = 50;
	return D3DXVECTOR3( (1.0f / 12.0f) * mass * (3 * radius_squared + height_squared),
						(0.5f) * mass * radius_squared,
						(1.0f / 12.0f) * mass * (3 * radius_squared + height_squared));
};

