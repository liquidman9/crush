/*
 * ServerEntity.hpp
 */

#ifndef SERVERENTITY_H_INCLUDED
#define SERVERENTITY_H_INCLUDED

// Global includes
#include <d3dx9.h>

// Project includes
#include <shared\game\Entity.h>



class ServerEntity : public virtual Entity{
public:

	// Physics
	D3DXVECTOR3 velocity;
	float maxVelocity;
	D3DXVECTOR3 force;
	float mass;
	float radius;

	void rotate(D3DXVECTOR3);
	virtual void calculate(float);

};


#endif SERVERENTITY_H_INCLUDED