/*
 * S_Ship.h
 */

#include "D3DX10math.h"
#include "Ship.h"
#include "ServerEntity.h"


class S_Ship : public Ship, public ServerEntity {


public:
	float thrusterForce;
	S_Ship();
	S_Ship(D3DXVECTOR3, D3DXVECTOR3, int, bool);
	void init();
	void rotate(D3DXVECTOR3);
	void calculate(float);

};