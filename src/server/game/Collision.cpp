/*
 * Collision.cpp
 */

// Project includes
#include <server/game/Collision.h>
#include <server/game/SRCollision.h>
#include <shared/util/SharedUtils.h>
#include <shared/CollisionGEvent.h>


Collision::Collision() {
	m_type = C,
	m_a = m_b = NULL;
	m_closeA = m_closeB = D3DXVECTOR3(0.0, 0.0, 0.0);
	m_elasticity = m_friction = 0.0;
}

Collision::Collision(CType type, ServerEntity * a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB, float elasticity, float friction) :
	m_type(type),
	m_a(a),
	m_b(b),
	m_closeA(closeA),
	m_closeB(closeB),
	m_elasticity(elasticity),
	m_friction(friction)
{}

Collision * Collision::generateCollision(ServerEntity *a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB)
{
	ServerEntity * one, * two;
	Collision * c;
	D3DXVECTOR3 temp;

	// calculate point of impact
	D3DXVECTOR3 delta_pos;
	delta_pos = closeA - closeB;
	D3DXVec3Normalize(&delta_pos, &delta_pos);

	D3DXVECTOR3 poi = (delta_pos * b->m_radius + closeB) + (-delta_pos * a->m_radius + closeA);
	poi /= 2;

	if (DEBUG) {
		if ((a->m_type == SHIP && b->m_type == MOTHERSHIP) || (b->m_type == MOTHERSHIP && a->m_type == SHIP)) {
			a->print();
			b->print();
			cout << "Closest to A: " << closeA << " , Closest to B: " << closeB << endl;
		}
	}
	// Ships & Resources
	if(((one = a)->m_type == RESOURCE && (two = b)->m_type == SHIP) || ((one = b)->m_type == RESOURCE && (two = a)->m_type == SHIP && (temp = closeA) && (closeA = closeB) && (closeB = temp) && (delta_pos = -delta_pos))){
		c = new SRCollision(two, one, closeB, closeA, 0.8, 0.6);
		c->m_collision_normal = delta_pos;
		c->m_poi = poi;

		return c;		
	}

	/*
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
		if(ship1->interact(ship2)) rtn = true;
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
		//rtn = false; // temporarily disabling reaction between ship and extractor
	}
	*/
	
	c = new Collision(C, a, b, closeA, closeB, 0.9, 0.6);
	c->m_collision_normal = delta_pos;
	c->m_poi = poi;

	return c;
}

CollisionGEvent * Collision::resolve()
{
	//float e_coll = 0.8f;
	//float u_fr = 0.6f;
	D3DXVECTOR4 temp;
	
	/*
	// calculate length travelled while colliding/penetration depth
	D3DXVECTOR3 penetA = poi - m_closeA;
	penetA.x = abs(penetA.x); 
	penetA.y = abs(penetA.y); 
	penetA.z = abs(penetA.z); 

	D3DXVECTOR3 penetB = poi - m_closeB;
	penetB.x = abs(penetB.x); 
	penetB.y = abs(penetB.y); 
	penetB.z = abs(penetB.z);
	*/

	// calculate the distance between point of contact and centers of mass
	D3DXVECTOR3 r_a = m_poi - m_a->m_pos;
	D3DXVECTOR3 r_b = m_poi - m_b->m_pos;

	D3DXVECTOR3 relative_linear_velocity = m_a->m_velocity - m_b->m_velocity;

	// Calculate rotation velocity at point of collision
	D3DXVECTOR3 a_collision_rotation_velocity, b_collision_rotation_velocity;
	D3DXVec3Cross(&a_collision_rotation_velocity, &m_a->m_angular_velocity, &r_a);
	D3DXVec3Cross(&b_collision_rotation_velocity, &m_b->m_angular_velocity, &r_b);

	D3DXVECTOR3 relative_angular_velocity = a_collision_rotation_velocity - b_collision_rotation_velocity;

	D3DXVECTOR3 relative_total_velocity = relative_linear_velocity + relative_angular_velocity;

	float vN = min(0.0f, D3DXVec3Dot(&relative_total_velocity, &m_collision_normal));
	float nN = D3DXVec3Dot(&m_collision_normal, &m_collision_normal);

	// calculate angular portion of impulse
	D3DXVECTOR3 inertA, inertB;  

	// I^-1(r x n)
	D3DXVec3Transform(&temp, D3DXVec3Cross(&inertA, &r_a, &m_collision_normal), &m_a->m_rot_inertia_inverse);
	inertA = D3DXVECTOR3(temp.x, temp.y, temp.z);

	D3DXVec3Transform(&temp, D3DXVec3Cross(&inertB, &r_b, &m_collision_normal), &m_b->m_rot_inertia_inverse);
	inertB = D3DXVECTOR3(temp.x, temp.y, temp.z);

	D3DXVECTOR3 inertBoth = *D3DXVec3Cross(&inertA, &inertA, &r_a) + *D3DXVec3Cross(&inertB, &inertB, &r_b);

	// calculate the impulse
	float impulse = (-(1 + m_elasticity) * vN) / (m_a->m_mass_inverse + m_b->m_mass_inverse + D3DXVec3Dot(&inertBoth, &m_collision_normal));
	
	D3DXVECTOR3 jN = impulse * m_collision_normal;

	// Friction force!
	float friction;

	D3DXVECTOR3 tangentVelocity = relative_total_velocity - (vN * m_collision_normal);

	if (D3DXVec3Dot(&tangentVelocity, &tangentVelocity)) {
		D3DXVECTOR3 tangent;
		D3DXVec3Normalize(&tangent, &tangentVelocity);
		float vt = D3DXVec3Dot(&relative_total_velocity, &tangent);
		// calculate angular portion of impulse
		D3DXVECTOR3 tI_a, tI_b;  

		// I^-1(r x n)
		D3DXVec3Transform(&temp, D3DXVec3Cross(&tI_a, &r_a, &tangent), &m_a->m_rot_inertia_inverse);
		tI_a = D3DXVECTOR3(temp.x, temp.y, temp.z);

		D3DXVec3Transform(&temp, D3DXVec3Cross(&tI_b, &r_b, &tangent), &m_b->m_rot_inertia_inverse);
		tI_b = D3DXVECTOR3(temp.x, temp.y, temp.z);

		D3DXVECTOR3 inertBoth = *D3DXVec3Cross(&tI_a, &tI_a, &r_a) + *D3DXVec3Cross(&tI_b, &tI_b, &r_b);

		float kt = m_a->m_mass_inverse + m_b->m_mass_inverse + D3DXVec3Dot(&inertBoth, &tangent);

		friction = max(-m_friction * impulse, min(m_friction * impulse, -vt / kt));

		jN += friction * tangent;
	}

	if (false ){//&& (m_a->m_type == SHIP && m_b->m_type == MOTHERSHIP) || (m_a->m_type == ASTEROID && m_b->m_type == SHIP)) {
		cout << endl << "Collision Info:" << endl;
		cout << "A type: " << (int)m_a->m_type << endl;
		cout << "B type: " << (int)m_b->m_type << endl;
		cout << "Relative vel: " << relative_total_velocity << endl;
		cout << "Impulse force: " << impulse << endl;
		cout << "Friction force: " << friction << endl;
		cout << "Impulse vector: " << jN << endl;
		cout << "DistA: " << r_a << endl;
		cout << "DistB: " << r_b << endl;
		cout << "A_Inert: " << m_a->m_rot_inertia << endl;
		cout << "B_Inert: " << m_b->m_rot_inertia << endl;
		cout << "POI: " << m_poi << endl << endl;
	}

	// Add the impulse to both entities
	if(!(m_a->m_type == SHIP && ((S_Ship *)m_a)->checkShield()))
		m_a->applyImpulse(jN, m_poi);

	
	if(!(m_b->m_type == SHIP && ((S_Ship *)m_b)->checkShield()))
		m_b->applyImpulse(-jN, m_poi);

	return new CollisionGEvent(m_a->m_id, m_b->m_id, m_poi, impulse);
}
