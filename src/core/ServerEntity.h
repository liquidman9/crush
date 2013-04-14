/*
 * ServerEntity.hpp
 */

#include "D3DX10math.h"

class ServerEntity {
public:

	// Physics
	float velocity;
	float maxVelocity;
	float force;
	float mass;

	virtual void rotate(D3DXVECTOR3) = 0;
	virtual void calculate(float) = 0;

};