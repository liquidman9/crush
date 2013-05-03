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


const char* TractorBeam::encode() const {
	// Declare ret
	char *rtn = new char[m_size];

	// Get entity encode
	const char *tmp = Entity::encode();

	// Copy entity encode into this ret
	memcpy(rtn, tmp, Entity::size());

	//always delete the encode buffer
	delete []tmp;

	// Set up temp buffer at the end of entity encoding
	char* tmp_rtn = rtn;
	tmp_rtn += Entity::size();

	// Encode Scale
	*(SHIP_PLAYERNUM_TYPE *) (tmp_rtn) = m_playerNum;
	tmp_rtn += sizeof(SHIP_PLAYERNUM_TYPE);

	*(bool *) (tmp_rtn) = m_isOn;
	tmp_rtn += sizeof(bool);

	*(D3DXVECTOR3 *) (tmp_rtn) = m_start;
	tmp_rtn += sizeof(D3DXVECTOR3);

	*(D3DXVECTOR3 *) (tmp_rtn) = m_end;
	tmp_rtn += sizeof(D3DXVECTOR3);

	*(float *) (tmp_rtn) = m_sentRadius;

	return rtn;
}

ostream& operator<<(ostream& os, const TractorBeam& e) {
	os << e.getID() << " " << e.m_start.x << " " << e.m_start.y << " " << e.m_start.z
		<< " " << e.m_end.x << " " << e.m_end.y << " " << e.m_end.z 
		<< " " << e.m_playerNum <<  " " << e.m_isOn << " " << e.m_sentRadius;
	return os;
}

void TractorBeam::decode(const char *buff) {
	Entity::decode(buff);
	m_type = TRACTORBEAM;
	buff += Entity::size();
	m_playerNum = *(SHIP_PLAYERNUM_TYPE*) buff;
	buff += sizeof(SHIP_PLAYERNUM_TYPE);
	m_isOn = *(bool*) buff;
	buff += sizeof(bool);
	m_start = *(D3DXVECTOR3*) buff;
	buff += sizeof(D3DXVECTOR3);
	m_end = *(D3DXVECTOR3*) buff;
	buff += sizeof(D3DXVECTOR3);
	m_sentRadius= *(float*) buff;
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