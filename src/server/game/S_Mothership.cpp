/*
 * S_Mothership.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_Mothership.h>


S_Mothership::S_Mothership(int pNum) :
	Entity(MOTHERSHIP),
	Mothership(pNum),
	ServerEntity(D3DXVECTOR3(0,0,0), 0, 1000)
{

}

S_Mothership::S_Mothership(D3DXVECTOR3 pos, Quaternion orientation, int pNum) :
	Entity(genId(), MOTHERSHIP, pos, orientation),
	Mothership(pNum),
	ServerEntity(D3DXVECTOR3(0,0,0), 0, 1000)
{	

}

