/*
 * PhysicsWorld.cpp
 */

// Project includes
#include <server/game/PhysicsWorld.h>

// defines
#define NEAR_ZERO 0.00001

ostream& operator<<(ostream& os, const D3DXVECTOR3 &v) {
	os << "<" << v.x << ", " << v.y << ", " << v.z << ">";
	return os;
}

ostream& operator<<(ostream& os, const D3DXQUATERNION &v) {
	os << "<" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ">";
	return os;
}

void PhysicsWorld::collision(float delta_time) {
	//int iterCount = 10;	// Attempt to solve collisions 10 times, to be implemented


	for(unsigned i = 0; i < entities.size(); i++)
	{	
		// Collsions between objects
		for(unsigned j = i+1; j < entities.size(); j++)
		{
			Collision * c;
				if ((c = checkCollision(*entities[i], *entities[j])) != NULL)
					// Object Specific Logic
					if(typeResponse(entities[i], entities[j]))
						c->resolve();

			delete(c);
		}

		// Boundary Collisions
		for(unsigned k = 0; k < boundaries.size(); k++) {
			if(checkCollision(*entities[i], boundaries[k])){
					respond(entities[i], boundaries[k]);
			}
		}
	}
}


void PhysicsWorld::update(float delta_time) {
	// Updating Positions
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


Collision * PhysicsWorld::checkCollision(ServerEntity& a, ServerEntity& b){
	D3DXVECTOR3 lengthA = (a.m_pFront - a.m_pBack); 
	D3DXVECTOR3 lengthB = (b.m_pFront - b.m_pBack); 
	D3DXVECTOR3 lengthBs = (a.m_pBack - b.m_pBack); 

	float dotA = D3DXVec3Dot(&lengthA, &lengthA);
	float dotB = D3DXVec3Dot(&lengthA, &lengthB);
	float dotC = D3DXVec3Dot(&lengthB, &lengthB);
	float dotD = D3DXVec3Dot(&lengthA, &lengthBs);
	float dotE = D3DXVec3Dot(&lengthB, &lengthBs);

	float D = dotA * dotC - dotB * dotB;

	float sc, sN, sD = D;
	float tc, tN, tD = D;

	// Compute line params of two closest points
	if(D < NEAR_ZERO)
	{
		sN = 0.0;	// force using end point on line a
		sD = 1.0;
		tN = dotE;
		tD = dotC;
		//cout << "D is 0" << endl;
	}

	else
	{
		sN = (dotB * dotE - dotC * dotD);
		tN = (dotA * dotE - dotB * dotD);

		if(sN < 0.0)
		{
			sN = 0.0;
			tN = dotE;
			tD = dotC;
		}

		else if(sN > sD)
		{
			sN = sD;
			tN = dotE + dotB;
			tD = dotC;
		}
	}

	if(tN < 0.0)
	{
		tN = 0.0;

		if(-dotD < 0.0)
			sN = 0.0;

		else if(-dotD > dotA)
			sN = sD;

		else
		{
			sN = -dotD;
			sD = dotA;
		}
	}

	else if(tN > tD)
	{
		tN = tD;

		if((-dotD + dotB) < 0.0)
			sN = 0;
		
		else if((-dotD + dotB) > dotA)
			sN = sD;

		else
		{
			sN = (-dotD + dotB);
			sD = dotA;
		}
	}

	sc = (abs(sN) < NEAR_ZERO ? 0.0 : sN / sD);
	tc = (abs(tN) < NEAR_ZERO ? 0.0 : tN / sD);

	D3DXVECTOR3 dP = lengthBs + (sc * lengthA) - (tc * lengthB);	// a(sc) - b(tc) 

	//printf("%f < %f?\n", D3DXVec3Length(&dP), a.m_radius + b.m_radius);

	if(D3DXVec3Length(&dP) < a.m_radius + b.m_radius)
	{
		//cout << "Not null" << endl;
		return Collision::generateCollision(&a, &b, (a.m_pBack + sc * lengthA), (b.m_pBack + tc * lengthB));
	}

	//cout << "null" << endl;
	return (Collision *)NULL;
}

bool PhysicsWorld::typeResponse(ServerEntity * a, ServerEntity * b) {
	ServerEntity * one, * two;
	bool rtn = true; // true if going to calculate a physics response

	// Ships & Resources
	if(((one = a)->m_type == RESOURCE && (two = b)->m_type == SHIP) || ((one = b)->m_type == RESOURCE && (two = a)->m_type == SHIP)){
		S_Ship * ship = (S_Ship *)two;
		S_Resource * res = (S_Resource *) one;
		bool gatheredOrDropped = ship->interact(res);
		if(gatheredOrDropped) rtn = false;
		else rtn = true;
	}

	// Give/Take Resource to Mothership
	if(((one = a)->m_type == SHIP && (two = b)->m_type == MOTHERSHIP) || ((one = b)->m_type == SHIP && (two = a)->m_type == MOTHERSHIP)){
		S_Mothership * mothership = (S_Mothership *)two;
		S_Ship * ship = (S_Ship *) one;
		mothership->interact(ship);
		rtn = true; 
	}

	// Ship to Ship
	if(((one = a)->m_type == SHIP && (two = b)->m_type == SHIP) ){
		S_Ship * ship1 = (S_Ship *) one;
		S_Ship * ship2 = (S_Ship *) two;
		ship1->interact(ship2);
		ship2->interact(ship1);
		rtn = true; 
	}

	// Asteroid hits Ship
	if(((one = a)->m_type == SHIP && (two = b)->m_type == ASTEROID) || ((one = b)->m_type == SHIP && (two = a)->m_type == ASTEROID)){
		S_Asteroid * asteroid = (S_Asteroid *)two;
		S_Ship * ship = (S_Ship *) one;
		ship->interact(asteroid);
		rtn = true; 
	}


	if(((one = a)->m_type == RESOURCE && (two = b)->m_type == MOTHERSHIP) || ((one = b)->m_type == RESOURCE && (two = a)->m_type == MOTHERSHIP)){
		rtn = false;  // change if possible to push a resource into the mothership with the tractorbeam
	}


	if(((one = a)->m_type == TRACTORBEAM && (two = b)->m_type) || ((one = b)->m_type == TRACTORBEAM && (two = a)->m_type)){
		S_TractorBeam * beam = (S_TractorBeam *)one;
		ServerEntity * entity = two;

		if(beam->m_isOn){
		
			if(entity->m_type == SHIP && beam->m_ship == entity || entity->m_type == MOTHERSHIP) rtn = false; // tmp
			// If is already locked check if closer
			else if(beam->isLocked()) {	
				if(beam->getCurrentDistance() > D3DXVec3Length(&beam->getDistanceVectorOf(entity->m_pos))){
					beam->lockOn(entity);
				}
			}
			// nothing locked so lock on
			else {	
				beam->lockOn(entity);
			}
		}
		rtn = false; // could give them the immovable tag 
	}

	if(((one = a)->m_type == RESOURCE) && ((one = b)->m_type == RESOURCE)){
		S_Resource * res1 = (S_Resource *)one;
		S_Resource * res2 = (S_Resource *)two;

		if(res1->m_carrier != NULL && res2->m_carrier != NULL) rtn = true; 
		else rtn = false; // resources can be placed on top of it each other when on the mothership
		// or could give them the immovable tag (relative to their carrier) when on the mothership and while being held
	}

	return rtn;

}
float lower = 1.0;
void PhysicsWorld::respond(ServerEntity * a, ServerEntity * b) {
	//cout<< (int) a->m_type << " and "<<(int) b->m_type << " collide"<<endl;
	if(typeResponse(a, b)) {
		//cout<< (int) a->m_type << " and "<<(int) b->m_type << " collide"<<endl;
		D3DXVECTOR3 n = a->m_pos - b->m_pos;
		D3DXVec3Normalize(&n,&n);

		D3DXVECTOR3 v1 = a->m_velocity;
		D3DXVECTOR3 v2 = b->m_velocity;
		float a1 = D3DXVec3Dot(&v1,&n);
		float a2 = D3DXVec3Dot(&v2,&n);

		float optimizedP = (2.0f * (a1 - a2)) / (a->m_mass + b->m_mass);

		D3DXVECTOR3 nv1 = v1 - optimizedP * b->m_mass * n;
		D3DXVECTOR3 nv2 = v2 + optimizedP * a->m_mass * n;

		if(!a->m_immovable){
			a->m_velocity = nv1/lower;
			a->m_pos+= a->m_velocity;
		}

		if(!b->m_immovable){
			b->m_velocity = nv2/lower;
			b->m_pos+=b->m_velocity;
		}
	}
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
	//a->m_velocity = nv1/lower;
	//a->m_pos+= a->m_velocity;
}