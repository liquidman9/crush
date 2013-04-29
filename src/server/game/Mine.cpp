/*
 * Mine.cpp
 */
 
// Project includes
#include <server/game/Mine.h>

Mine::Mine() :
	m_pos(D3DXVECTOR3(0.0, 0.0, 0.0)),
	m_ore(NULL),
	m_timer(0)
{	
}

void Mine::setStart(long time) {
	m_timer = time;
}
bool Mine::checkRespawn(float time) 
{
	if(m_ore != NULL && m_ore->m_pos != D3DXVECTOR3(0,10,0)) m_ore = NULL; //tmp

	if(m_ore != NULL) {
		m_timer = time;
		return false;
	}
	else if((time - m_timer) >= s_msReset){
		m_timer = time;
		return true;
	}
	return false;
}

S_Resource * Mine::respawn() 
{
	cout<< "Respawned Resource"<<endl;
	m_ore = new S_Resource(D3DXVECTOR3(m_pos.x, m_pos.y + 10, m_pos.z),Quaternion(0.0,0.0,0.0,1.0));
	return m_ore;
}


S_Resource * Mine::transfer()
{
	S_Resource * rtn = m_ore;
	m_ore = NULL;
	return rtn;
}