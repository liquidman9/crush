/*
 * TractorBeam.cpp
 */

// Project includes
#include <shared/game/TractorBeam.h>

TractorBeam::TractorBeam() :
	Entity(TRACTORBEAM),
	m_playerNum(0),
	m_isOn(false),
	m_start(D3DXVECTOR3()),
	m_end(D3DXVECTOR3()),
	m_sentRadius(1)

{
}

TractorBeam::TractorBeam(int pNum) :
	Entity(TRACTORBEAM),
	m_playerNum(pNum),
	m_isOn(false),
	m_start(D3DXVECTOR3()),
	m_end(D3DXVECTOR3()),
	m_sentRadius(1)
{
}


unsigned int TractorBeam::encode(char *head) const {
	// Get entity encode
	unsigned int rtn = Entity::encode(head);

	// Encode Scale
	*(SHIP_PLAYERNUM_TYPE *) (head+rtn) = m_playerNum;
	rtn += sizeof(SHIP_PLAYERNUM_TYPE);

	*(bool *) (head+rtn) = m_isOn;
	rtn += sizeof(bool);

	*(D3DXVECTOR3 *) (head+rtn) = m_start;
	rtn += sizeof(D3DXVECTOR3);

	*(D3DXVECTOR3 *) (head+rtn) = m_end;
	rtn += sizeof(D3DXVECTOR3);

	*(float *) (head+rtn) = m_sentRadius;
	rtn += sizeof(float);

	return rtn;
}

ostream& operator<<(ostream& os, const TractorBeam& e) {
	os << e.getID() << " " << e.m_start.x << " " << e.m_start.y << " " << e.m_start.z
		<< " " << e.m_end.x << " " << e.m_end.y << " " << e.m_end.z 
		<< " " << e.m_playerNum <<  " " << e.m_isOn << " " << e.m_sentRadius;
	return os;
}

unsigned int TractorBeam::decode(const char *buff) {
	unsigned int rtn = Entity::decode(buff);
	m_type = TRACTORBEAM;
	m_playerNum = *(SHIP_PLAYERNUM_TYPE*) (buff+rtn);
	rtn += sizeof(SHIP_PLAYERNUM_TYPE);
	m_isOn = *(bool*) (buff+rtn);
	rtn += sizeof(bool);
	m_start = *(D3DXVECTOR3*) (buff+rtn);
	rtn += sizeof(D3DXVECTOR3);
	m_end = *(D3DXVECTOR3*) (buff+rtn);
	rtn += sizeof(D3DXVECTOR3);
	m_sentRadius= *(float*) (buff+rtn);
	rtn += sizeof(float);
	return rtn;
}


void TractorBeam::update(shared_ptr<Entity> sp_source) {
	Entity *source = sp_source.get();
	TractorBeam * srcTractorBeam = dynamic_cast<TractorBeam*>(source);
	if (srcTractorBeam == 0) {
		//TODO figure out how to handle this case when not in debug
#ifdef _DEBUG
//TODO put this back in, make sure it doesn't break anyone's build
//		MessageBox( NULL, L"Error converting Entity to Ship", L"CRUSH Game", MB_OK );
#endif
	} else {
		Entity::update(sp_source);
		m_playerNum = srcTractorBeam->m_playerNum;
		m_isOn = srcTractorBeam->m_isOn;
		m_start = srcTractorBeam->m_start;
		m_end = srcTractorBeam->m_end;
		m_sentRadius = srcTractorBeam->m_sentRadius;
	}
}