/*
 * main.cpp
 */

// Global includes
#include <stdio.h>

// Project includes
#include <server/game/S_Ship.h>
#include <server/game/PhysicsWorld.h>
/*
void main()
{

	//Testing
	S_Ship a = S_Ship(D3DXVECTOR3(100,0,0),D3DXVECTOR3(0,0,0),1,false);
	S_Ship b = S_Ship(D3DXVECTOR3(0,0,0),D3DXVECTOR3(0,0,0),1,false);
	a.m_pos = D3DXVECTOR3(100,0,0);
	PhysicsWorld world = PhysicsWorld();
	world.entities.push_back(&a);
	world.entities.push_back(&b);
	world.update();



	S_Ship a = S_Ship();
	a.calculate(50);

	printf("%f, %f, %f\n", a.m_pos.x, a.m_pos.y, a.m_pos.z);
	a.m_acceleratorOn = !a.m_acceleratorOn;
	a.calculate(5);
	printf("%f, %f, %f\n", a.m_pos.x, a.m_pos.y,a.m_pos.z);

	for(int i = 0; i < 20; i++){
		a.calculate(5);
		printf("%f, %f, %f\n", a.m_pos.x, a.m_pos.y,a.m_pos.z);
	}

	a.m_acceleratorOn = !a.m_acceleratorOn;

	for(int i = 0; i < 20; i++){
		a.calculate(5);
		printf("%f, %f, %f\n", a.m_pos.x, a.m_pos.y,a.m_pos.z);
	}

	a.rotate(D3DXVECTOR3(0,0,-2));
	a.m_acceleratorOn = !a.m_acceleratorOn;
	for(int i = 0; i < 40; i++){
		a.calculate(5);
		printf("%f, %f, %f\n", a.m_pos.x, a.m_pos.y,a.m_pos.z);
	}

	a.rotate(D3DXVECTOR3(0,1,0));
	for(int i = 0; i < 20; i++){
		a.calculate(5);
		printf("%f, %f, %f\n", a.m_pos.x, a.m_pos.y,a.m_pos.z);
	}
	
	while(1){
		
	}
	return;
}*/