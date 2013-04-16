/*
 * Ship.cpp
 */

#include "Ship.h"

Ship::Ship() : 
	m_playerNum(0),
	m_tractorBeamOn(false)
{
}

Ship::Ship(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int pNum, bool tBeamOn) :
	Entity(pos, dir),
	m_playerNum(pNum),
	m_tractorBeamOn(tBeamOn)
{
}