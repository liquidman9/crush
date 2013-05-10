/*
 * S_Extractor.cpp
 */
 
// Project includes
#include <server/game/S_Extractor.h>

S_Extractor::S_Extractor(D3DXVECTOR3 pos, Quaternion orientation) :
	Entity(genId(), EXTRACTOR, pos, orientation),
	Extractor(),
	ServerEntity(20000000.0f, calculateRotationalInertia(20000000.0f), 5.0f, 1.0f),
	m_ore(NULL),
	m_timer(0)
{	
}

void S_Extractor::setStart(long time) {
	m_timer = time;
}
bool S_Extractor::checkRespawn(long time) 
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
	m_ore = new S_Resource(D3DXVECTOR3(m_pos.x, m_pos.y + 15.0, m_pos.z),Quaternion(0.0,0.0,0.0,1.0));
	return m_ore;
}


S_Resource * S_Extractor::transfer()
{
	cout<< "Transfered Resource"<<endl;
	S_Resource * rtn = m_ore;
	m_ore = NULL;
	return rtn;
}


D3DXVECTOR3 S_Extractor::calculateRotationalInertia(float mass){
	float radius_squared = 5.0f;
	return D3DXVECTOR3( (2.0f / 5.0f) * mass * radius_squared,
						(2.0f / 5.0f) * mass * radius_squared,
						(2.0f / 5.0f) * mass * radius_squared);
};


void S_Extractor::update(float delta_time){
	ServerEntity::update(delta_time);
	
}