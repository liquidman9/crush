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

float server::entities::tractorbeam::gravity = 5.0f;
float server::entities::tractorbeam::length = 350.0f;

S_TractorBeam::S_TractorBeam(S_Ship * ship) :
	Entity(genId(), TRACTORBEAM, D3DXVECTOR3(0,0,0), Quaternion(0,0,0,1)),
	TractorBeam(ship->m_playerNum),
	ServerEntity(1000, D3DXVECTOR3(1, 1, 1), length, 1.0),
	m_strength(0),
	m_isPulling(true),
	m_gravity(gravity),
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
	D3DXVECTOR3 range = m_end - m_start;
	D3DXVec3Normalize(&range, &range);
	return range;
}

D3DXVECTOR3 S_TractorBeam::getCurrentDistanceVector() {
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

	/* For if start point is at edge of "shield"
	//Quaternion tmpq;
	//D3DXMATRIX matRotate;
	//D3DXVECTOR3 tmp = D3DXVECTOR3(0,0,1);
	//D3DXMatrixRotationQuaternion(&matRotate, D3DXQuaternionNormalize(&tmpq, &m_ship->m_orientation));
	//D3DXVec3TransformCoord(&tmp,&tmp,&matRotate);
	//D3DXVec3Normalize( &tmp, &tmp );
	*/

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
		
		D3DXVECTOR3 force = m_strength*(m_gravity*m_ship->m_mass*m_object->m_mass)*(disV)/(pow(disL, 2));

		if(m_isPulling) {
			// Check for extreme force that pulls the object in the opposite direction
			D3DXVECTOR3 shipMomentumTest, objectMomentumTest, shipVelocityTest, objectVelocityTest, shipPositionTest, objectPositionTest;
			shipMomentumTest = m_ship->m_momentum + m_ship->t_impulse + (force*.01f);
			shipVelocityTest = shipMomentumTest * m_ship->m_mass_inverse;
			shipPositionTest = m_ship->m_pos + shipVelocityTest * (1.0/60.0f);

			objectMomentumTest = m_object->m_momentum + m_object->t_impulse + (-force*.01f);
			objectVelocityTest = objectMomentumTest * m_object->m_mass_inverse;
			objectPositionTest = m_object->m_pos + objectVelocityTest * (1.0/60.0f);

			D3DXVECTOR3 disB = getCurrentDistanceVector();
			D3DXVECTOR3 disA = objectPositionTest - shipPositionTest;
			float angleDiff = acos(D3DXVec3Dot(&disA,&disB)/(D3DXVec3Length(&disB)*D3DXVec3Length(&disA)))*180.0/3.14159265;
			

			// If at the point to be held
			if(angleDiff > 3.0 || m_isColliding || m_isHolding){
				// Resources are not held by tractor beam once they collide with the ship
				if(m_object->m_type == RESOURCE) {
					if(m_ship->interact((S_Resource *)m_object)) cout << "Resource not gathered- error?"<<endl;
					else {
						m_object = NULL;
						m_isColliding = false;
						m_isHolding = false; //make method
					}
				}
				else {
					if(!m_isHolding) m_heldDistance = disL; // Set permanent holding distance
					m_object->m_momentum = D3DXVECTOR3(0.0f,0.0f,0.0f);
					m_object->t_impulse = D3DXVECTOR3(0.0f,0.0f,0.0f); // zero out physics (position is set in the Ship's update)
					m_isHolding = true;
				}
			}
			// Apply normal pulling force
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


bool S_TractorBeam::interact(ServerEntity * entity) {
	if(m_isOn){
		
		if(entity->m_type == SHIP && m_ship == entity || 
			entity->m_type == MOTHERSHIP || 
			entity->m_type == EXTRACTOR || 
			(entity->m_type == RESOURCE && ((S_Resource *)entity)->m_carrier != NULL)) 
				return false; // tmp
		// If is already locked check if closer
		else if(isLocked()) {	
			if(getCurrentDistance() > D3DXVec3Length(&getDistanceVectorOf(entity->m_pos))){
				lockOn(entity);
			}
		}
		// nothing locked so lock on
		else {	
			lockOn(entity);
		}
	}

	return false;
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

