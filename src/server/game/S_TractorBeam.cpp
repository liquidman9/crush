/*
 * S_TractorBeam.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <server/game/S_TractorBeam.h>
//#include <server/game/S_Ship.h>

using namespace server::entities::tractorbeam;

//static float s_gravitationalConstant =  gravity;
//static float s_defaultLength = length;

using namespace server::entities::tractorbeam;

S_TractorBeam::S_TractorBeam(S_Ship * ship) :
	Entity(genId(), TRACTORBEAM, D3DXVECTOR3(0,0,0), Quaternion(0,0,0,1)),
	TractorBeam(ship->m_playerNum),
	ServerEntity(1000, length, calculateRotationalInertia(1)),
	m_strength(0),
	m_isPulling(true),
	m_power(power),
	m_isColliding(false),
	m_isHolding(false),
	m_heldDistance(0.0f)
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
	if(m_object != entity) {
		m_isHolding = false; //tmppp
		m_isColliding = false;
	}
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
	if(!isLocked()) return length;

	return D3DXVec3Length(&(getCurrentDistanceVector()));
}

void S_TractorBeam::setStartPoint() {
	//Quaternion tmpq;
	//D3DXMATRIX matRotate;
	//D3DXVECTOR3 tmp = D3DXVECTOR3(0,0,1);
	//D3DXMatrixRotationQuaternion(&matRotate, D3DXQuaternionNormalize(&tmpq, &m_ship->m_orientation));
	//D3DXVec3TransformCoord(&tmp,&tmp,&matRotate);
	//D3DXVec3Normalize( &tmp, &tmp );
	m_start = m_ship->m_pos;// + tmp*m_ship->m_radius;
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
		m_end = m_ship->m_pos + tmp * m_length;
	}
	else m_end = m_object->m_pos;

	m_pBack = m_end;
}

void S_TractorBeam::calculateForce() {
	// 1 is Ship, 2 is Object, thus pull - object, push + object
	float dis = getCurrentDistance();

	if(isLocked()) {
		D3DXVECTOR3 disV = getCurrentDirection();
		float disL = getCurrentDistance();
		
		D3DXVECTOR3 force = m_strength*(m_power*m_ship->m_mass*m_object->m_mass)*(disV)/(pow(disL, 2));

		if(m_isPulling) {
				// Check for Over
				D3DXVECTOR3 shipMomentumTest, objectMomentumTest, shipVelocityTest, objectVelocityTest, shipPositionTest, objectPositionTest;
				shipMomentumTest = m_ship->m_momentum + m_ship->t_impulse + (force*.01f);
				shipVelocityTest = shipMomentumTest * m_ship->m_mass_inverse;
				shipPositionTest = m_ship->m_pos + shipVelocityTest * (1.0f/60.0f);

				objectMomentumTest = m_object->m_momentum + m_object->t_impulse + (-force*.01f);
				objectVelocityTest = objectMomentumTest * m_object->m_mass_inverse;
				objectPositionTest = m_object->m_pos + objectVelocityTest * (1.0f/60.0f);

			D3DXVECTOR3 disB = getCurrentDistanceVector();
			D3DXVECTOR3 disA = objectPositionTest - shipPositionTest;
			float a = acos(D3DXVec3Dot(&disA,&disB)/(D3DXVec3Length(&disB)*D3DXVec3Length(&disA)))*(float)(180.0/3.14159265);
			

			// reaching threshold
			if(a > 3.0 || m_isColliding || m_isHolding){
				if(m_object->m_type == RESOURCE) {
					if(m_ship->interact((S_Resource *)m_object)) cout << "Resource not gathered- error?"<<endl;
					else {
						m_object = NULL;
						m_isColliding = false;
						m_isHolding = false; //make method
					}
				}
				else {
					if(!m_isHolding) m_heldDistance = disL;
					m_object->m_momentum = D3DXVECTOR3(0.0f,0.0f,0.0f);
					m_object->t_impulse = D3DXVECTOR3(0.0f,0.0f,0.0f);
					m_isHolding = true;
				}
			}
			else {
				m_ship->applyLinearImpulse(force * .01f);
				m_object->applyLinearImpulse(-force * .01f);

			}
		}
		// Push
		else {
				m_ship->applyLinearImpulse(-force * .01f);
				m_object->applyLinearImpulse(force * .01f);
				m_object = NULL;
				m_isColliding = false;
				m_isHolding = false; // make unlock/reset method
		}
	}

}


void S_TractorBeam::updateData() {
	setStartPoint();
	setEndPoint();
	//cout << "dis: "<<getCurrentDistance()<<" Dir: "<<getCurrentDirection().x<<" "<<getCurrentDirection().y<<" "<<getCurrentDirection().z<<endl;
	if(m_isOn){
		calculateForce();
		m_isColliding = false;
	}
	else {
		m_object = NULL;
		m_isColliding = false;
		m_isHolding = false;
	}
}

void S_TractorBeam::setIsPulling(bool isPull){
	if(isPull) {
		m_isPulling = isPull;
	}
	else {
		m_isPulling = isPull;
		m_strength = 1.0;
		m_isHolding = false;
	}
}

bool S_TractorBeam::getIsPulling(){
	return m_isPulling;
}

void S_TractorBeam::setIsOn(bool isOn){
	if(isOn) {
		m_isOn = isOn;
	}
	else {
		m_isOn = isOn;
	}
}
void S_TractorBeam::update(float delta_time) {
	// do nothing during regular update cycle (not affected by physics)
}

D3DXMATRIX S_TractorBeam::calculateRotationalInertia(float mass){
	return *D3DXMatrixIdentity(&D3DXMATRIX());
};

