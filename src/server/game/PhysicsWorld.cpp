/*
 * PhysicsWorld.cpp
 */

// Project includes
#include <server/game/PhysicsWorld.h>

// defines
#define NEAR_ZERO 0.0001

ostream& operator<<(ostream& os, const D3DXVECTOR3 &v) {
	os << "<" << v.x << ", " << v.y << ", " << v.z << ">";
	return os;
}

ostream& operator<<(ostream& os, const D3DXQUATERNION &v) {
	os << "<" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ">";
	return os;
}

void PhysicsWorld::collision(float delta_time) {

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
	if(a <= NEAR_ZERO && e <= NEAR_ZERO) 
	{
		s = t = 0.0f;
		c1 = p1;
		c2 = p2;
		dist = c1 - c2;
		return D3DXVec3Dot(&dist, &dist);
	}

	// Check if a is a sphere
	if(a <= NEAR_ZERO) 
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
		if(e <= NEAR_ZERO) 
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
			if(denom <= NEAR_ZERO)
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
Collision * PhysicsWorld::checkCollision(ServerEntity & a, ServerEntity & b) {
	D3DXVECTOR3 p1 = a.m_pFront, p2 = a.m_pBack, p3 = b.m_pFront, p4 = b.m_pBack;
	
	// Distance along A to closest point to B
	float mu_a = shared::utils::clamp((dotPoints(p1, p3, p4, p3) * dotPoints(p4, p3, p2, p1) - dotPoints(p1, p3, p2, p1) * dotPoints(p4, p3, p4, p3)) / 
									  (dotPoints(p2, p1, p2, p1) * dotPoints(p4, p3, p4, p3) - dotPoints(p4, p3, p2, p1) * dotPoints(p4, p3, p2, p1)),
									  0.0f, 1.0f);

	// Distance along B to closest point to A
	float mu_b = shared::utils::clamp((dotPoints(p1, p3, p4, p3) + mu_a * dotPoints(p4, p3, p2, p1)) / dotPoints(p4, p3, p4, p3),
									  0.0f, 1.0f);
	
	// Closest points on each line
	D3DXVECTOR3 p_a = p1 + mu_a * (p2 - p1);
	D3DXVECTOR3 p_b = p3 + mu_b * (p4 - p3);

	// Vector between closest points
	D3DXVECTOR3 delta_ab = p_a - p_b;

	Collision * ret = NULL;

	if(D3DXVec3Length(&delta_ab) < a.m_radius + b.m_radius)
	{
		if ((a.m_type == SHIP || b.m_type == SHIP) && (a.m_type != TRACTORBEAM && b.m_type != TRACTORBEAM)) {
			cout << "Type: " << (int)a.m_type << " and " << (int)b.m_type << endl;
			cout << "Mu_a: " << mu_a << " and Mu_b: " << mu_b << endl;
			printf("%f < %f?\n", D3DXVec3Length(&delta_ab), a.m_radius + b.m_radius);
		}

		ret =  Collision::generateCollision(&a, &b, p_a, p_b);
	}

	return ret;
}

/*
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

	sc = (abs(sN) < NEAR_ZERO ? 0.0f : sN / sD);
	tc = (abs(tN) < NEAR_ZERO ? 0.0f : tN / sD);

	D3DXVECTOR3 dP = lengthBs + (sc * lengthA) - (tc * lengthB);	// a(sc) - b(tc) 

	if(D3DXVec3Length(&dP) < a.m_radius + b.m_radius)
	{
		if ((a.m_type == SHIP || b.m_type == SHIP) && (a.m_type != TRACTORBEAM && b.m_type != TRACTORBEAM))
			printf("%f < %f?\n", D3DXVec3Length(&dP), a.m_radius + b.m_radius);
		return Collision::generateCollision(&a, &b, (a.m_pBack + sc * lengthA), (b.m_pBack + tc * lengthB));
	}
	return NULL;
}
*/


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
	}
}