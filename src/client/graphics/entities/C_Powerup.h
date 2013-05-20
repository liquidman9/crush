/*
 * C_Powerup.h
 */

#ifndef C_POWERUP_H_INCLUDED
#define C_POWERUP_H_INCLUDED

// Project includes
#include <shared/game/Powerup.h>
#include <client/graphics/entities/C_Entity.h>

#pragma warning( push )
#pragma warning( disable : 4250 )

class C_Powerup : public Powerup, public C_Entity {
public:
	C_Powerup();
	C_Powerup(Entity *);
	//void draw();
};

#pragma warning( pop )


#endif