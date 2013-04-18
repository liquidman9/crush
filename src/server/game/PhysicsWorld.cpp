
/*
 * PhysicsWorld.cpp
 */

// Project includes
#include <server/game/PhysicsWorld.h>

void PhysicsWorld::update() {
	//int iterCount = 10;	// Attempt to solve collisions 10 times, to be implemented


	for(int i = 0; i < entities.size(); i++)
	{	
		for(int j = i+1; j < entities.size(); j++)
		{
			//if(i != j)//entities[i].m_id != entities[j].m_id)
			//{
				if(checkCollision(*entities[i], *entities[j])){
					printf("Collide %i with %i\nA: %f,%f,%f \nB: %f,%f,%f\n", i, j, entities[i]->m_pos.x,entities[i]->m_pos.y,entities[i]->m_pos.z,entities[j]->m_pos.x,entities[j]->m_pos.y,entities[j]->m_pos.z);
					respond(entities[i], entities[j]);
				}
				else printf("No Collision\nA: %f,%f,%f \nB: %f,%f,%f\n",entities[i]->m_pos.x,entities[i]->m_pos.y,entities[i]->m_pos.z,entities[j]->m_pos.x,entities[j]->m_pos.y,entities[j]->m_pos.z);
			//}
		}
	}

		
	for(int i = 0; i < entities.size(); i++)
	{
		entities[i]->calculate(.005);
	}
}

bool PhysicsWorld::checkCollision(ServerEntity a, ServerEntity b){
	double dx = a.m_pos.x - b.m_pos.x; 
	dx *= dx;
	double dy = a.m_pos.y - a.m_pos.y; 
	dy *= dy; 

	double sum = a.radius + a.radius; 
	sum *= sum;


	if(dx + dy <= sum){
		return true;
	}
	return false;
}



void PhysicsWorld::respond(ServerEntity * a, ServerEntity * b) {
	D3DXVECTOR3 n = a->m_pos - b->m_pos;
	D3DXVec3Normalize(&n,&n);

	D3DXVECTOR3 v1 = a->velocity;
	D3DXVECTOR3 v2 = b->velocity;
	float a1 = D3DXVec3Dot(&v1,&n);
	float a2 = D3DXVec3Dot(&v2,&n);

	float optimizedP = (2.0 * (a1 - a2)) / (a->mass + b->mass);

	D3DXVECTOR3 nv1 = v1 - optimizedP * b->mass * n;
	D3DXVECTOR3 nv2 = v2 + optimizedP * a->mass * n;

	a->velocity = nv1;
	b->velocity = nv2;
}