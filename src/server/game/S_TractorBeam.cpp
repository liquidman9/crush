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
	m_heldDistance(0.0f),
	m_totalPulling(0,0,0),
	m_shipLastCorrection(0.0,0.0,0.0),
	m_objectLastCorrection(0.0,0.0,0.0),
	disableStart(-1),
	disableLength(5000)
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
		lockOff();
		entity->m_heldBy = m_ship;
	}
	m_object = entity;
}

void S_TractorBeam::lockOff() {
	if(m_object != NULL) {
		m_object->m_heldBy = NULL;
		m_object = NULL;
	}
	m_isColliding = false;
	m_isHolding = false; 
	m_shipLastCorrection = shared::utils::VEC3_ZERO;
	m_objectLastCorrection = shared::utils::VEC3_ZERO;
	m_totalPulling = shared::utils::VEC3_ZERO;
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
		
		D3DXVECTOR3 force = m_strength*(m_power*m_ship->m_mass*m_object->m_mass)*(disV)/(pow(disL, 2));

		if(m_isPulling) {
			// Check for extreme force that pulls the object in the opposite direction
			D3DXVECTOR3 shipMomentumTest, objectMomentumTest, shipVelocityTest, objectVelocityTest, shipPositionTest, objectPositionTest;
			shipMomentumTest = m_ship->m_momentum + m_ship->t_impulse + (force*.01f);
			shipVelocityTest = shipMomentumTest * m_ship->m_mass_inverse;
			shipPositionTest = m_ship->m_pos + shipVelocityTest * (1.0f/60.0f);

			objectMomentumTest = m_object->m_momentum + m_object->t_impulse + (-force*.01f);
			objectVelocityTest = objectMomentumTest * m_object->m_mass_inverse;
			objectPositionTest = m_object->m_pos + objectVelocityTest * (1.0f/60.0f);

			D3DXVECTOR3 disB = getCurrentDistanceVector();
			D3DXVECTOR3 disA = objectPositionTest - shipPositionTest;
			float angleDiff = acos(D3DXVec3Dot(&disA,&disB)/(D3DXVec3Length(&disB)*D3DXVec3Length(&disA)))*180.0f/PI;

			// If at the point to be held
			if(angleDiff > 2 || m_isColliding || m_isHolding){
				// Resources are not held by tractor beam once they collide with the ship
				if(m_object->m_type == RESOURCE) {
					if(m_ship->interact((S_Resource *)m_object)) cout << "Resource not gathered- error?"<<endl;
					else {
						lockOff();
					}
				}
				else if(m_object->m_type == SHIP) {
					
				}
				else { 

					if(!m_isHolding) {
						m_heldDistance = (m_object->m_length > m_object->m_radius? m_object->m_length:m_object->m_radius) + (m_ship->m_length > m_ship->m_radius? m_ship->m_length:m_ship->m_radius);//disL; // Set permanent holding distance
						m_shipLastCorrection = shared::utils::VEC3_ZERO;
						m_objectLastCorrection = shared::utils::VEC3_ZERO;

						// Equal out momentums (lock)
						//D3DXVECTOR3 vel = ((m_ship->t_impulse + m_ship->m_momentum) + (m_object->m_momentum  + m_object->t_impulse))/(m_ship->m_mass + m_object->m_mass);
						//m_object->m_momentum = vel*m_object->m_mass;
						//m_ship->m_momentum = vel*m_ship->m_mass;


						// Zero outs probably temporary
					//	m_object->m_momentum = shared::utils::VEC3_ZERO;
					//	m_ship->m_momentum =shared::utils::VEC3_ZERO;
					//	m_object->t_impulse =shared::utils::VEC3_ZERO;
					//	m_ship->t_impulse = shared::utils::VEC3_ZERO;
					}
					
						float deltaTime = (float) 1.0/60.0f;

						float totalMass = m_ship->m_mass + m_object->m_mass;
						D3DXVECTOR3 center = ((m_object->m_mass/totalMass)*(m_object->m_pos-m_ship->m_pos)) + m_ship->m_pos;
						float shipMassLength = m_heldDistance*(m_object->m_mass/(m_ship->m_mass + m_object->m_mass)) ;
						float objectMassLength = m_heldDistance*(m_ship->m_mass/(m_ship->m_mass + m_object->m_mass)) ;
						D3DXVECTOR3 rShip = center - m_ship->m_pos;
						D3DXVECTOR3 rObj = m_object->m_pos -center;

						//m_ship->m_momentum -= m_shipLastCorrection;
						//m_object->m_momentum -= m_objectLastCorrection; 

						D3DXVECTOR3 shipImp = m_ship->getDamping();
					    D3DXVECTOR3 velShip = ((shipImp)*m_ship->m_mass_inverse);
					    D3DXVECTOR3 velObject =  ((m_object->t_impulse)*m_object->m_mass_inverse);

						// Predicting Position
						D3DXVECTOR3 posShip = m_ship->m_pos + (m_ship->m_momentum + (velShip*m_ship->m_mass))*m_ship->m_mass_inverse*deltaTime;
						D3DXVECTOR3 posObject = m_object->m_pos + (m_object->m_momentum+ (velObject*m_object->m_mass))*m_object->m_mass_inverse*deltaTime;

						D3DXVECTOR3 newDis = posObject-posShip;
						D3DXVECTOR3 newDir = posObject - posShip;
						D3DXVec3Normalize(&newDir, &newDir);
				

						// Calculate new center of mass
						center += ((m_object->m_momentum + (velObject*m_object->m_mass)*m_object->m_mass) + (m_ship->m_momentum + (velShip*m_ship->m_mass)*m_ship->m_mass))/pow(totalMass,2); 
						//center += ((m_object->m_momentum + (velObject*m_object->m_mass))*m_object->m_mass + (m_ship->m_momentum + (velShip*m_ship->m_mass)*m_ship->m_mass))/pow(totalMass,2); 
						//center += ((m_object->m_momentum + (velObject*m_object->m_mass))*m_object->m_mass + (m_ship->m_momentum + (velShip*m_ship->m_mass))*m_ship->m_mass)/pow(totalMass,2); 
					
						D3DXVECTOR3 newRShip, newRObject;
						D3DXVec3Normalize(&newRObject,&(posObject - posShip));
						newRShip = - newRObject;
					
						// Desired Position
						D3DXVECTOR3 dPosShip = center + (newRShip)*shipMassLength; 
						D3DXVECTOR3 dPosObject = center + (newRObject)*objectMassLength;


						D3DXVECTOR3 adjustShip = (dPosShip - posShip)/deltaTime;
						D3DXVECTOR3 adjustObj = (dPosObject - posObject)/deltaTime;

						m_ship->applyLinearImpulse(adjustShip*m_ship->m_mass);
						m_object->applyLinearImpulse(adjustObj*m_object->m_mass);
						
						//m_shipLastCorrection = adjustShip*m_ship->m_mass;
						//m_objectLastCorrection = adjustObj*m_object->m_mass;

				
				

					m_isHolding = true;
				}

				
			}
			// Apply normal pulling force
			else {
				m_ship->applyLinearImpulse(force * .01f);
				m_object->applyLinearImpulse(-force * .01f);



				m_totalPulling = m_totalPulling + force;
			}
					
		}
		// Push
		else {
				m_ship->applyLinearImpulse(-force * .01f);
				m_object->applyLinearImpulse(force * .01f);

				m_isHolding = false; 
		}
	}

	m_isColliding = false;
}

void S_TractorBeam::disable() {
	lockOff();

	m_isOn = false;
	timeout();
}
void S_TractorBeam::timeout() {
	disableStart = GetTickCount();
}

void S_TractorBeam::updateData() {
	setStartPoint();
	setEndPoint();

	if(disableStart != -1) {
		long time = GetTickCount();
		m_isOn = false;
		if(time - disableStart > disableLength) {
			disableStart = -1;
		}
	}
	else if(m_isOn){
		calculateForce();
		m_isColliding = false;
	}
	else {
		lockOff();
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
		lockOff();
	}
}


bool S_TractorBeam::interact(ServerEntity * entity) {
	if(m_isOn){

		if(entity->m_type == SHIP && m_ship == entity || 
			entity->m_type == MOTHERSHIP || 
			entity->m_type == EXTRACTOR || 
			entity->m_type == POWERUP || // can not tractorbeam powerups
			(entity->m_type == RESOURCE && (((S_Resource *)entity)->m_carrier != NULL && ((S_Resource *)entity)->m_carrier->m_type != EXTRACTOR)) )
				return false; 
		// If is already locked check if closer
		else if(isLocked()) {	
			if(entity != m_object && m_object->m_type != RESOURCE && // temp not sure what the rules should be when switching objects
				!m_isHolding && 
				D3DXVec3Length(&getDistanceVectorOf(m_object->m_pos)) > D3DXVec3Length(&getDistanceVectorOf(entity->m_pos)))
			{
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

D3DXMATRIX S_TractorBeam::calculateRotationalInertia(float mass){
	return *D3DXMatrixIdentity(&D3DXMATRIX());
};

