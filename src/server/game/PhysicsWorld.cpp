/*
 * PhysicsWorld.cpp
 */

// Project includes
#include <server/game/PhysicsWorld.h>

ostream& operator<<(ostream& os, const D3DXVECTOR3 &v) {
	os << "<" << v.x << ", " << v.y << ", " << v.z << ">";
	return os;
}

ostream& operator<<(ostream& os, const D3DXQUATERNION &v) {
	os << "<" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ">";
	return os;
}

void PhysicsWorld::update(float delta_time) {
	//int iterCount = 10;	// Attempt to solve collisions 10 times, to be implemented


	for(unsigned i = 0; i < entities.size(); i++)
	{	
		for(unsigned j = i+1; j < entities.size(); j++)
		{
			//if(i != j)//entities[i].m_id != entities[j].m_id)
			//{
				if(checkCollision(*entities[i], *entities[j])){
					respond(entities[i], entities[j]);
				}
			//}
		}

		for(unsigned k = 0; k < boundaries.size(); k++) {
			if(checkCollision(*entities[i], boundaries[k])){
					respond(entities[i], boundaries[k]);
			}
		}
	}


	for(unsigned i = 0; i < entities.size(); i++)
	{
		//entities[i]->calculate(.005f);
		entities[i]->update(delta_time);
			// Derived Variables
		/*
		ServerEntity * e = entities[i];
		cout << "Entity: " << i << endl;
		cout << "Velocity: " << e->m_velocity << endl;
		cout << "Angular Velocity: " << e->m_angular_velocity << endl;
		cout << "Orientation Delta: " << e->m_orientation_delta << endl;
		cout << "Orientation: " << e->m_orientation << endl;
		*/
	}
}

bool PhysicsWorld::checkCollision(ServerEntity& a, ServerEntity& b){
	double dx = a.m_pos.x - b.m_pos.x; 
	dx *= dx;
	double dy = a.m_pos.y - a.m_pos.y; 
	dy *= dy; 

	double sum = a.m_radius + a.m_radius; 
	sum *= sum;


	if(dx + dy <= sum){
		return true;
	}
	return false;
}



void PhysicsWorld::respond(ServerEntity * a, ServerEntity * b) {
	D3DXVECTOR3 n = a->m_pos - b->m_pos;
	D3DXVec3Normalize(&n,&n);

	D3DXVECTOR3 v1 = a->m_velocity;
	D3DXVECTOR3 v2 = b->m_velocity;
	float a1 = D3DXVec3Dot(&v1,&n);
	float a2 = D3DXVec3Dot(&v2,&n);

	float optimizedP = (2.0f * (a1 - a2)) / (a->m_mass + b->m_mass);

	D3DXVECTOR3 nv1 = v1 - optimizedP * b->m_mass * n;
	D3DXVECTOR3 nv2 = v2 + optimizedP * a->m_mass * n;

	a->m_velocity = nv1;
	a->m_pos+= a->m_velocity;
	b->m_velocity = nv2;
	b->m_pos+=b->m_velocity;
}

bool PhysicsWorld::checkCollision(ServerEntity& a, Boundary& b){

    D3DXVECTOR3 v(a.m_pos - b.m_point);
    float dis(D3DXVec3Dot(&v, &b.m_normal));

    if(dis > a.m_radius)
    {
        return false;
    }

    return true;
}

void PhysicsWorld::respond(ServerEntity * a, Boundary b) {
	D3DXVECTOR3 n = b.m_normal;

	D3DXVECTOR3 v1 = a->m_velocity;
	float a1 = D3DXVec3Dot(&v1,&n);

	D3DXVECTOR3 nv1 = v1 - 2*n*a1;
	a->m_velocity = nv1;
	a->m_pos+= a->m_velocity;
}