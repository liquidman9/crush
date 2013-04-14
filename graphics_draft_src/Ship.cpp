/*
 * Ship.cpp
 */

#include "Ship.h"

Ship::Ship() : 
	m_playerNum(0),
	m_color(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)),
	m_tractorBeamOn(false)
{
}

Ship::Ship(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int pNum, bool tBeamOn) :
	Entity(pos, dir),
	m_playerNum(pNum),
	m_tractorBeamOn(tBeamOn)
{
}