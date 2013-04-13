/*
 * entity.cpp
 */

#include "entity.hpp"
#include <stdio.h>

int Entity::id_gen = 0;

Entity::Entity() {
	init();
}

Entity::Entity(D3DXVECTOR3 pos, D3DXVECTOR3 dir) {
	init();
	this->pos = pos;
	this->dir = dir;
}

void Entity::init() {
	id = id_gen++;
	type = ENTITY;
	pos = D3DXVECTOR3(0.0, 0.0, 0.0);
	dir = D3DXVECTOR3(0,0,1);
	force = 100;
	mass = 100;
	velocity = 0;
	maxVelocity = 100;
	isThrustOn = false;
}

void Entity::rotate(D3DXVECTOR3 rotation){
	dir = dir + rotation;

	//D3DXVec3Normalize(&dir,&dir); - linker error
	float tmp = abs(sqrt((dir.x * dir.x) + (dir.y * dir.y) + (dir.z * dir.z)));
	dir = D3DXVECTOR3(dir.x/tmp,dir.y/tmp,dir.z/tmp);
}
void Entity::toggleThrust() {
	isThrustOn = !isThrustOn;
}

void Entity::calculate(float dt){
	// Calculate applied force
	float tF =  0;
	if(isThrustOn) tF = force;
	else if(velocity != 0) tF = -force;

	float dv = ((tF / mass ) * dt);
	if(velocity + dv <= maxVelocity)
		velocity = velocity + dv;
	pos = pos + velocity*dir;
}

void main()
{

	//Testing
	Entity a = Entity();
	a.calculate(50);

	printf("%f, %f, %f\n", a.pos.x, a.pos.y, a.pos.z);
	a.toggleThrust();
	a.calculate(5);
	printf("%f, %f, %f\n", a.pos.x, a.pos.y,a.pos.z);

	for(int i = 0; i < 20; i++){
		a.calculate(5);
		printf("%f, %f, %f\n", a.pos.x, a.pos.y,a.pos.z);
	}

	a.toggleThrust();

	for(int i = 0; i < 20; i++){
		a.calculate(5);
		printf("%f, %f, %f\n", a.pos.x, a.pos.y,a.pos.z);
	}

	a.rotate(D3DXVECTOR3(0,0,-2));
	a.toggleThrust();
	for(int i = 0; i < 40; i++){
		a.calculate(5);
		printf("%f, %f, %f\n", a.pos.x, a.pos.y,a.pos.z);
	}

	a.rotate(D3DXVECTOR3(0,1,0));
	for(int i = 0; i < 20; i++){
		a.calculate(5);
		printf("%f, %f, %f\n", a.pos.x, a.pos.y,a.pos.z);
	}

	while(1){
		
	}
	return;
}