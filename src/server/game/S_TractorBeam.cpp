/*
 * S_TractorBeam.cpp
 */

// External includes
#include <stdio.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/S_TractorBeam.h>


S_TractorBeam::S_TractorBeam(int pNum) :
	Entity(TRACTORBEAM),
	TractorBeam(pNum),
	ServerEntity()
{

}

