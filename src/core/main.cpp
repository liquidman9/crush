#include "S_Ship.h"
#include <stdio.h>

void main()
{

	//Testing
	S_Ship a = S_Ship();
	a.calculate(50);

	printf("%f, %f, %f\n", a.m_pos.x, a.m_pos.y, a.m_pos.z);
	a.toggleThrust();
	a.calculate(5);
	printf("%f, %f, %f\n", a.m_pos.x, a.m_pos.y,a.m_pos.z);

	for(int i = 0; i < 20; i++){
		a.calculate(5);
		printf("%f, %f, %f\n", a.m_pos.x, a.m_pos.y,a.m_pos.z);
	}

	a.toggleThrust();

	for(int i = 0; i < 20; i++){
		a.calculate(5);
		printf("%f, %f, %f\n", a.m_pos.x, a.m_pos.y,a.m_pos.z);
	}

	a.rotate(D3DXVECTOR3(0,0,-2));
	a.toggleThrust();
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
}