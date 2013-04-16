/*
 * Ship.h
 */

#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

#include "Entity.h"

class Ship : public Entity {
public:
	int m_playerNum;
	bool m_tractorBeamOn;
	Ship();
	Ship(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int pNum, bool tBeamOn);
};


#endif