/*
 * ServerEntity.hpp
 */

#include <d3dx9.h>

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