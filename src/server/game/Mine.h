/*
 * Mine.hpp
 */

#ifndef MINE_H_INCLUDED
#define MINE_H_INCLUDED

// Global includes
#include <d3dx9.h>

// Project includes
#include <server/game/S_Resource.h> 

class Mine {
public:
	static const int s_msReset = 30000; 

	// Fields
	long m_timer;
	D3DXVECTOR3 m_pos;
	S_Resource *m_ore;

	// Constructors
	Mine();

	// Methods
	void setStart(long);
	bool checkRespawn(float);
	S_Resource * respawn();
	S_Resource * transfer();
};



#endif MINE_H_INCLUDED