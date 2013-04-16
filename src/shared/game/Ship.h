/*
 * Ship.h
 */

#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED


// Project includes
#include <shared/game/Entity.h>

class Ship : public virtual Entity {
public:
	int m_playerNum;
	bool m_tractorBeamOn;
	bool m_thrustersOn;
	Ship();
	Ship(Vector3 pos, Vector3 dir, int pNum);
	void toggleThrust();
};


#endif