/*
 * S_Extractor.cpp
 */
 
// Project includes
#include <server/game/S_Extractor.h>

S_Extractor::S_Extractor(D3DXVECTOR3 pos, Quaternion orientation) :
	Entity(genId(), EXTRACTOR, pos, orientation),
	Extractor(),
	ServerEntity(20000000.0f, 5.0f, calculateRotationalInertia(2000000000.0f)),
	m_ore(NULL),
	m_timer(0)
{	
}

void S_Extractor::setStart(long long time) {
	m_timer = time;
}
bool S_Extractor::checkRespawn(long long time) 
{
	if(m_ore != NULL && D3DXVec3Length(&(m_ore->m_pos - m_pos)) > 25) {
		m_ore = NULL;
		m_timer = time;
		return false;
	}
	else if(m_ore != NULL) { 
		m_timer = time;
		return false;
	}
	else if((time - m_timer) >= s_msReset){
		m_timer = time;
		return true;
	}
	return false;
}

S_Resource * S_Extractor::respawn() 
{
	cout<< "Respawned Resource"<<endl;
	m_ore = new S_Resource(D3DXVECTOR3(m_pos.x, m_pos.y + 15.0f, m_pos.z), Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
	return m_ore;
}


S_Resource * S_Extractor::transfer()
{
	cout<< "Transfered Resource"<<endl;
	S_Resource * rtn = m_ore;
	m_ore = NULL;
	return rtn;
}


D3DXMATRIX S_Extractor::calculateRotationalInertia(float mass){
	float radius_squared = 25.0f;
	float height_squared = 100.0f;
	return *D3DXMatrixScaling(&D3DXMATRIX(), (1.0f / 12.0f) * mass * (3 * radius_squared + height_squared),
											 (1.0f / 12.0f) * mass * (3 * radius_squared + height_squared),
											 (0.5f) * mass * radius_squared);
};


void S_Extractor::update(float delta_time){
	// Should not be movable
	//ServerEntity::update(delta_time);
	
}