/*
 * S_Extractor.cpp
 */
 
// Project includes
#include <server/game/S_Extractor.h>
#include <server/Globals.h>
#include <vector>
#include <memory>

using namespace server::entities::extractor;
using namespace shared::utils;

S_Extractor::S_Extractor(D3DXVECTOR3 pos, Quaternion orientation) :
	Entity(genId(), EXTRACTOR, pos, orientation),
	Extractor(),
	ServerEntity(2000000000.0f, 5.0f, calculateRotationalInertia(2000000000.0f)),
	m_cur_resource(NULL),
	m_timer(resource_respawn_time)
{ }

/*
void S_Extractor::setStart(float time) {
	m_timer = time;
} 
*/

S_Resource * S_Extractor::getResource() 
{
	if (respawned) {
		respawned = false;
		return m_cur_resource;
	} else {
		return NULL;
	}
}


void S_Extractor::respawn() 
{
	cout << "Respawned Resource" << endl;
	// CONFIG: RESOURCE RESPAWN LOCATIONS
	D3DXVECTOR3 loc = D3DXVECTOR3(rand_float(-0.1f, 0.2f), rand_float(0.9f, 0.1f) , rand_float(-0.1f, 0.2f));
	D3DXVec3Normalize(&loc, &loc);
	m_cur_resource = new S_Resource(m_pos + loc * (m_length * 2 + m_radius * 2), Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
	m_cur_resource->m_carrier = this;
	m_timer = resource_respawn_time;
	respawned = true;
}

/*
S_Resource * S_Extractor::transfer()
{
	cout<< "Transfered Resource"<<endl;
	S_Resource * rtn = m_ore;
	m_ore = NULL;
	return rtn;
}
*/


D3DXMATRIX S_Extractor::calculateRotationalInertia(float mass){
	float radius_squared = m_radius * m_radius;
	float height_squared = m_length * m_length;
	return *D3DXMatrixScaling(&D3DXMATRIX(), (1.0f / 12.0f) * mass * (3 * radius_squared + height_squared),
											 (1.0f / 12.0f) * mass * (3 * radius_squared + height_squared),
											 (0.5f) * mass * radius_squared);
};


void S_Extractor::update(float delta_time){	
	if (m_cur_resource->m_carrier != this) {
		m_timer -= delta_time;
		if (m_timer <= 0) {
			respawn();
		}
	}
	ServerEntity::update(delta_time);
}