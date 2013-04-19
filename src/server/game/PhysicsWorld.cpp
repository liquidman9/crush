/*
 * PhysicsWorld.cpp
 */

// Project includes
#include <server/game/PhysicsWorld.h>

void PhysicsWorld::update() {
	//int iterCount = 10;	// Attempt to solve collisions 10 times, to be implemented


	for(unsigned i = 0; i < entities.size(); i++)
	{	
		for(unsigned j = i+1; j < entities.size(); j++)
		{
			//if(i != j)//entities[i].m_id != entities[j].m_id)
			//{
				if(checkCollision(*entities[i], *entities[j])){
					printf("Collide %i with %i\n", i, j);
					respond(entities[i], entities[j]);
				}
				else printf("No Collision\n");
			//}
		}

		for(unsigned k = 0; k < boundaries.size(); k++) {
			if(checkCollision(*entities[i], boundaries[k])){
					printf("Collide %i with Boundary %i\n", i, k);
					respond(entities[i], boundaries[k]);
			}
		}
	}


		
	for(unsigned i = 0; i < entities.size(); i++)
	{
		entities[i]->calculate(.005f);
		printf("Number %i: %f,%f,%f \n", i, entities[i]->m_pos.x,entities[i]->m_pos.y,entities[i]->m_pos.z);
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

	float optimizedP = (2.0f * (a1 - a2)) / (a->mass + b->mass);

	D3DXVECTOR3 nv1 = v1 - optimizedP * b->mass * n;
	D3DXVECTOR3 nv2 = v2 + optimizedP * a->mass * n;

	a->velocity = nv1;
	a->m_pos+= a->velocity;
	b->velocity = nv2;
	b->m_pos+=b->velocity;
}

bool PhysicsWorld::checkCollision(ServerEntity a, Boundary b){

    D3DXVECTOR3 v(a.m_pos - b.m_point);
    float dis(D3DXVec3Dot(&v, &b.m_normal));

    if(dis > a.radius)
    {
        return false;
    }

    return true;
}

void PhysicsWorld::respond(ServerEntity * a, Boundary b) {
	D3DXVECTOR3 n = b.m_normal;

	D3DXVECTOR3 v1 = a->velocity;
	float a1 = D3DXVec3Dot(&v1,&n);

	D3DXVECTOR3 nv1 = v1 - 2*n*a1;
	a->velocity = nv1;
	a->m_pos+= a->velocity;
}