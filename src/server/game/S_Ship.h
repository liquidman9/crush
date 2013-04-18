/*
 * S_Ship.h
 */

#ifndef S_SHIP_H_INCLUDED
#define S_SHIP_H_INCLUDED

// External includes
#include <d3dx9.h>

// Project includes
#include <shared/game/Ship.h>
#include <server/game/ServerEntity.h>



class S_Ship : public Ship, public ServerEntity{ //switch back to capsule

public:
	// Fields
	float thrusterForce;
	bool m_acceleratorOn;


	// Constructors
	S_Ship();
	S_Ship(D3DXVECTOR3, D3DXVECTOR3, int, bool);

	// Methods
	void init();
	void rotate(D3DXVECTOR3);
	void calculate(float);

};

#endif S_SHIP_H_INCLUDED