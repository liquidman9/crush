/*
 * Ship.cpp
 */

// Project includes
#include <shared/game/Ship.h>

Ship::Ship() : 	
	Entity(SHIP),
  	m_playerNum(0),
	m_tractorBeamOn(false),
	m_thrustersOn(false) 
{ }

Ship::Ship(Vector3 pos, Vector3 dir, int pNum) :
	Entity(SHIP, pos, dir),
	m_playerNum(pNum),
	m_tractorBeamOn(false),
	m_thrustersOn(false) 
{ }

void Ship::toggleThrust() {
	m_thrustersOn = !m_thrustersOn;
}