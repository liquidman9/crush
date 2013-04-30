/*
 * S_Mothership.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Mothership.h>


S_Mothership::S_Mothership(int pNum) :
	Entity(MOTHERSHIP),
	Mothership(pNum),
	ServerEntity(D3DXVECTOR3(0,0,0), 0, 1000)
{
	m_immovable = true;
	m_radius = 15.0;
}

S_Mothership::S_Mothership(D3DXVECTOR3 pos, Quaternion orientation, int pNum) :
	Entity(genId(), MOTHERSHIP, pos, orientation),
	Mothership(pNum),
	ServerEntity(D3DXVECTOR3(0,0,0), 0, 1000)
{	
	m_immovable = true;
	m_radius = 10.0;
}


	
D3DXVECTOR3 S_Mothership::calculateRotationalInertia(float mass){
	float radius_squared = 50;
	float height_squared = 50;
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

bool S_Mothership::giveResource(S_Ship * ship){
	if(ship->m_resource != NULL) {
		m_resourceList.push_back(ship->m_resource);
		ship->m_resource->m_carrier = this;
		ship->m_resource = NULL;
		this->m_resources++;
		cout<<"Given"<<endl;
		return true;
	}
	return false;
}

bool S_Mothership::takeResource(S_Ship * ship){
	if(ship->m_resource == NULL && m_resourceList.size() != 0) {
		ship->m_resource = m_resourceList.back();
		m_resourceList.pop_back();
		ship->m_resource->m_carrier = ship;
		this->m_resources--;
		cout<<"Taken"<<endl;
		return true;
	}
	return false;
}