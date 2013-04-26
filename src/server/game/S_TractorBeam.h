/*
 * S_TractorBeam.h
 */

#ifndef S_TRACTORBEAM_H_INCLUDED
#define S_TRACTORBEAM_H_INCLUDED

// External includes
#define WIN32_LEAN_AND_MEAN
#include <d3dx9.h>

// Project includes
#include <shared/game/TractorBeam.h>
#include <server/game/ServerEntity.h>
#include <shared/InputState.h>


#pragma warning( push )
#pragma warning( disable : 4250 )

class S_TractorBeam : public TractorBeam, public ServerEntity{ //switch back to capsule

public:
	// Fields

	// Constructors
	S_TractorBeam(int);
};

#pragma warning( pop )

#endif S_TRACTORBEAM_H_INCLUDED