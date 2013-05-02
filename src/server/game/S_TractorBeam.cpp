/*
 * S_TractorBeam.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <server/game/S_TractorBeam.h>
//#include <server/game/S_Ship.h>

static float s_gravitationalConstant =  0.01f;

S_TractorBeam::S_TractorBeam(int pNum) :
	Entity(TRACTORBEAM),
	TractorBeam(pNum),
	ServerEntity(1000, D3DXVECTOR3(1, 1, 1), 100000.0f, 1.0),// infinity
	m_strength(0)
{
	m_radius = 2.0;
	m_object = NULL;
	m_ship = NULL;
	m_pFront = D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z + m_length);
	m_pBack = D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z);
}

	
S_TractorBeam::S_TractorBeam(D3DXVECTOR3 pos, Quaternion orientation, int pNum) :
	Entity(genId(), TRACTORBEAM, pos, orientation),
	TractorBeam(pNum),
	ServerEntity(1000, D3DXVECTOR3(1, 1, 1), 100000.0f, 1.0),
	m_strength(0)
{	
	m_radius = 2.0;
	m_object = NULL;
	m_ship = NULL;
}

bool S_TractorBeam::isLocked() {
	if(m_object != NULL) return true;
	else return false;
}

void S_TractorBeam::lockOn(ServerEntity * entity) {
	m_object = entity;
	m_orientation = m_ship->m_orientation; //tmp
}

D3DXVECTOR3 S_TractorBeam::getCurrentDirection() {
	if(!isLocked()) return m_orientation;
	D3DXVECTOR3 range = m_object->m_pos - m_ship->m_pos;
	D3DXVec3Normalize(&range, &range);
	return range;
}

D3DXVECTOR3 S_TractorBeam::getCurrentDistanceVector() {
	if(!isLocked()) return m_orientation;
	D3DXVECTOR3 range = m_object->m_pos - m_ship->m_pos;
	D3DXVec3Normalize(&range, &range);
	return range;
}



D3DXVECTOR3 S_TractorBeam::getDistanceVectorOf(D3DXVECTOR3 b) {
	D3DXVECTOR3 range = b - m_ship->m_pos;
	D3DXVec3Normalize(&range, &range);
	return range;
}

float S_TractorBeam::getCurrentDistance() {
	if(!isLocked()) return 100000.0f;

	return D3DXVec3Length(&(m_object->m_pos - m_ship->m_pos));
}

void S_TractorBeam::calculateForce() {
	// 1 is Ship, 2 is Object, thus pull - object, push + object
	if(isLocked()) {
		D3DXVECTOR3 disV = getCurrentDistanceVector();
		float disL = D3DXVec3Length(&disV);
		disV = getCurrentDirection();
		D3DXVECTOR3 force = m_strength*(s_gravitationalConstant*m_ship->m_mass*m_object->m_mass)*(disV)/(pow(disL, 2));

		cout << "Type: "<<(int)m_object->m_type<<" Dis: "<<getCurrentDistance()<<" Force: "<<force.x <<" "<< force.y << " "<<force.z<< endl;
		
		if(m_isPulling) {
			m_ship->applyLinearImpulse(force, .01f);
			m_object->applyLinearImpulse(-force, .01f);
		}
		// Push
		else {
			m_ship->applyLinearImpulse(-force, .01f);
			m_object->applyLinearImpulse(force, .01f);
		}
	}
	else {

	}

}
/*
void S_TractorBeam::setOrientation() {
	//m_orientation = m_ship->m_orientation;
	D3DXVECTOR3 a;
	if(isLocked()) D3DXVec3Cross(&a, &getCurrentRange(), &D3DXVECTOR3(0,1,0));
	else {
		m_orientation = m_ship->m_orientation;
		return;
	}
	Quaternion q = Quaternion(a.x,a.y,a.z,1);
	q.w = sqrt(pow(D3DXVec3Length(&getCurrentRange()),2) * pow(D3DXVec3Length(&D3DXVECTOR3(0,1,0)),2)) + D3DXVec3Dot(&getCurrentRange(), &D3DXVECTOR3(0,1,0));

	m_orientation = q;
	D3DXQuaternionNormalize(&m_orientation, &m_orientation);

	cout << m_object->m_id << " "<<(int)m_object->m_type<<endl;
}*/
/*
void S_TractorBeam::setPosition() {
	m_pos = m_ship->m_pos;// + (getCurrentRange()*m_length/2);
}*/

D3DXVECTOR3 S_TractorBeam::calculateRotationalInertia(float mass){
	float radius_squared = 50;
	float height_squared = 50;
	return D3DXVECTOR3( (1.0f / 12.0f) * mass * (3 * radius_squared + height_squared),
						(0.5f) * mass * radius_squared,
						(1.0f / 12.0f) * mass * (3 * radius_squared + height_squared));
};

