/*
 * S_Mothership.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Mothership.h>

using namespace server::entities::mothership;

S_Mothership::S_Mothership(int pNum) :
	Entity(MOTHERSHIP),
	Mothership(pNum),
	ServerEntity(mass, calculateRotationalInertia(mass), 5.0f, 1.0f)
{
	m_immovable = true;
	m_radius = 5.0f;
	m_resourceSpots = 10;
	m_length = 5.0f;
}

S_Mothership::S_Mothership(D3DXVECTOR3 pos, Quaternion orientation, int pNum) :
	Entity(genId(), MOTHERSHIP, pos, orientation),
	Mothership(pNum),
	ServerEntity(mass, calculateRotationalInertia(mass), 5.0f, 1.0f)
{	
	m_immovable = true;
	m_radius = 5.0f;
	m_resourceSpots = 10;
	m_length = 5.0f;
}



	
D3DXVECTOR3 S_Mothership::calculateRotationalInertia(float mass){
	float radius_squared = 25;
	float height_squared = 25;
	return D3DXVECTOR3( (1.0f / 12.0f) * mass * (3 * radius_squared + height_squared),
						(0.5f) * mass * radius_squared,
						(1.0f / 12.0f) * mass * (3 * radius_squared + height_squared));
};

bool S_Mothership::interact(S_Ship * ship) {
	if(ship->m_playerNum == m_playerNum) 
		return giveResource(ship);
	else 
		return takeResource(ship);

}

//Ship gives resource
bool S_Mothership::giveResource(S_Ship * ship){
	if(ship->m_resource != NULL) {
		S_Resource * res = ship->m_resource;
		m_resourceList.push_back(res);
		res->m_carrier = this;
		ship->m_resource = NULL;
		res->m_spot = this->m_resources >= m_resourceSpots? m_resourceSpots:this->m_resources;
		this->m_resources++;
		res->m_travelFrames = 0;
		cout<<"Given"<<endl;
		return true;
	}
	return false;
}

// Ship takes resource
bool S_Mothership::takeResource(S_Ship * ship){
	if(ship->m_resource == NULL && m_resourceList.size() != 0) {
		S_Resource * res = m_resourceList.back();
		ship->m_resource = res;
		m_resourceList.pop_back();
		res->m_carrier = ship;
		this->m_resources--;
		res->m_spot = 0;
		res->m_travelFrames = 0;
		cout<<"Taken"<<endl;
		return true;
	}
	return false;
}


void S_Mothership::update(float delta_time){
	
	ServerEntity::update(delta_time);

}