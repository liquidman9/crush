/*
 * PhysicsWorld.cpp
 */

// Project includes
#include <server/game/PhysicsWorld.h>
#include <shared/util/SharedUtils.h>

void PhysicsWorld::collision(float delta_time) {

	for(unsigned i = 0; i < entities.size(); i++)
	{	
		// Collsions between objects
		for(unsigned j = i+1; j < entities.size(); j++)
		{
			Collision * c;
			if ((c = checkCollision(*entities[i], *entities[j])) != NULL){
				// Object Specific Logic
				
				if(typeResponse(entities[i], entities[j])){
					c->resolve();
				}
			}
			delete(c);
		}

		checkInBounds(entities[i]);

	}
}

// delta_time is in fractions of a second.
void PhysicsWorld::update(float delta_time) {
	// Pre-collision step

	// Perform collisions

	// Post-collision step

	// Updating objects
	for(unsigned i = 0; i < entities.size(); i++)
	{
		entities[i]->update(delta_time);
	}
}

float dotPoints(D3DXVECTOR3 m, D3DXVECTOR3 n, D3DXVECTOR3 o, D3DXVECTOR3 p) {
	return D3DXVec3Dot(&(m-n), &(o-p));
}

float squareDist(D3DXVECTOR3 p1, D3DXVECTOR3 q1, D3DXVECTOR3 p2, D3DXVECTOR3 q2, float & s, float & t, D3DXVECTOR3 & c1, D3DXVECTOR3 & c2)
{
	D3DXVECTOR3 d1 = q1 - p1;
	D3DXVECTOR3 d2 = q2 - p2;
	D3DXVECTOR3 dB = p1 - p2;
	D3DXVECTOR3 dist;

	// a is squared length of line of a, e is squared length of line of b
	float a = D3DXVec3Dot(&d1, &d1);
	float e = D3DXVec3Dot(&d2, &d2);
	float f = D3DXVec3Dot(&d2, &dB);

	// Check if both a and b are spheres/lines have length 0
	if(a <= shared::utils::FP_ZERO && e <= shared::utils::FP_ZERO) 
	{
		s = t = 0.0f;
		c1 = p1;
		c2 = p2;
		dist = c1 - c2;
		return D3DXVec3Dot(&dist, &dist);
	}

	// Check if a is a sphere
	if(a <= shared::utils::FP_ZERO) 
	{
		s = 0.0f;
		t = f / e;
		t = shared::utils::clamp(t, 0.0f, 1.0f);
	}

	// a is not a sphere
	else 
	{
		
		float c = D3DXVec3Dot(&d1, &dB);

		// Check if b is a sphere
		if(e <= shared::utils::FP_ZERO) 
		{
			t = 0.0f;
			s = shared::utils::clamp(-c / a, 0.0f, 1.0f);
		}

		// b is not a sphere
		else 
		{
			float b = D3DXVec3Dot(&d1, &d2);
			float denom = (a * e) - (b * b);

			// if the lines aren't parallel, compute the closest point on a to b and clamp. otherwise s = 0
			if(denom <= shared::utils::FP_ZERO)
				s = shared::utils::clamp(((b * f) - (c * e) / denom), 0.0f, 1.0f);

			else
				s = 0.0f;

			// compute the point on b to s of a
			t = (b * s + f) / e;

			// check if t is in [0, 1]. if not clamp t and recompute s for the new value of t
			if(t < 0.0f) 
			{
				t = 0.0f;
				s = shared::utils::clamp(-c / a, 0.0f, 1.0f);
			}

			else if (t > 1.0f) 
			{
				t = 1.0f;
				s = shared::utils::clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}

	c1 = p1 + d1 * s;
	c2 = p2 + d2 * t;
	dist = c1 - c2;

	return D3DXVec3Dot(&dist, &dist);
}

Collision * PhysicsWorld::checkCollision(ServerEntity & a, ServerEntity & b)
{
	float s, t;
	D3DXVECTOR3 c1, c2;

	float distanceSquare = squareDist(a.m_pFront, a.m_pBack, b.m_pFront, b.m_pBack, s, t, c1, c2);
	float radiiSquare = a.m_radius + b.m_radius;
	radiiSquare *= radiiSquare; 

	if(distanceSquare <= radiiSquare)
	{
		/*
		if ((a.m_type == SHIP || b.m_type == SHIP) && (a.m_type != TRACTORBEAM && b.m_type != TRACTORBEAM))
			printf("%f < %f?\n", D3DXVec3Length(&dP), a.m_radius + b.m_radius);
		*/	

		return Collision::generateCollision(&a, &b, c1, c2);
	}

	return NULL;
}


/*
 * Evaulates whether a physical response is neccessary for the collison and 
 * takes care of gameplay logic for the specific collisions
 * returns true if a response is neccessary
 */
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
		if(ship1->interact(ship2) && ship2->interact(ship1)) rtn = true;
		else rtn = false; 
	}

	// Asteroid hits Ship
	if(((one = a)->m_type == SHIP && (two = b)->m_type == ASTEROID) || ((one = b)->m_type == SHIP && (two = a)->m_type == ASTEROID)){
		S_Asteroid * asteroid = (S_Asteroid *)two;
		S_Ship * ship = (S_Ship *) one;
		return ship->interact(asteroid);
		//rtn = true; 
	}

	//Resource and Mothership
	if(((one = a)->m_type == RESOURCE && (two = b)->m_type == MOTHERSHIP) || ((one = b)->m_type == RESOURCE && (two = a)->m_type == MOTHERSHIP)){
		rtn = false;  // change if possible to push a resource into the mothership with the tractorbeam
	}

	// TractorBeam
	if(((one = a)->m_type == TRACTORBEAM && (two = b)->m_type) || ((one = b)->m_type == TRACTORBEAM && (two = a)->m_type)){
		S_TractorBeam * beam = (S_TractorBeam *)one;
		ServerEntity * entity = two;

		beam->interact(entity); //lock on check 
		rtn = false; // could give them the immovable tag 
	}

	// Resource
	if(((one = a)->m_type == RESOURCE) && ((one = b)->m_type == RESOURCE)){
		S_Resource * res1 = (S_Resource *)one;
		S_Resource * res2 = (S_Resource *)two;

		if(res1->m_carrier != NULL && res2->m_carrier != NULL) rtn = true; 
		else rtn = false; // resources can be placed on top of it each other when on the mothership
		// or could give them the immovable tag (relative to their carrier) when on the mothership and while being held
	}

	// Resource Temp
	if(((one = a)->m_type == RESOURCE)|| ((one = b)->m_type == RESOURCE)){
		rtn = false; // temporarily disabling all collisions with resources because of the infinite movement
	}

	// Powerup Temp - until is given some implementation
	if(((one = a)->m_type == POWERUP)|| ((one = b)->m_type == POWERUP)){
		rtn = false;
	}

	if(((one = a)->m_type == POWERUP && (two = b)->m_type == SHIP)|| ((one = b)->m_type == POWERUP && (two = a)->m_type == SHIP)){
		S_Powerup * power = (S_Powerup *)one;
		S_Ship * ship = (S_Ship *)two;
		ship->interact(power);
		rtn = false;
	}

	//Extractor and Ship
	if(((one = a)->m_type == EXTRACTOR && (two = b)->m_type == SHIP)|| ((one = b)->m_type == EXTRACTOR && (two = a)->m_type == SHIP)){
		rtn = false; // temporarily disabling reaction between ship and extractor
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


/*
 * Checks whether an entity is within the sphere boundary and adjusts it's 
 * location and properties if out of bounds
 */
void PhysicsWorld::checkInBounds(ServerEntity * a) {
	float asteroidWorldRadius = m_worldRadius + 100;
	if(abs(D3DXVec3Length(&a->m_pos)) +a->m_radius > m_worldRadius && a->m_type == SHIP) {
		// Out of Bounds
		D3DXVECTOR3 norm;
		D3DXVec3Normalize(&norm, &-a->m_pos);
		a->applyLinearImpulse(norm*10000);

	}
	else if(abs(D3DXVec3Length(&a->m_pos)) > asteroidWorldRadius && a->m_type == ASTEROID) {
		((S_Asteroid *)a)->reCreateAsteroid(asteroidWorldRadius);
		for(unsigned i = 0; i < entities.size(); i++)
		{	
			if(entities[i]->m_type == TRACTORBEAM){
				S_TractorBeam * b = (S_TractorBeam *)entities[i];
				if(b->m_object == a) {
					
					b->m_object = NULL;
					b->m_totalPulling = D3DXVECTOR3(0.0,0.0,0.0);
					b->m_isHolding = false; //tmppp
					b->m_isColliding = false;
					// need method
				}
			}
		}
	}
}