/*
 * Collision.cpp
 */

// Project includes
#include <server/game/Collision.h>
#include <server/game/SRCollision.h>
#include <server/game/MSCollision.h>
#include <server/game/SSCollision.h>
#include <server/game/SACollision.h>
#include <server/game/RMCollision.h>
#include <server/game/TCollision.h>
#include <server/game/RRCollision.h>
#include <server/game/PPCollision.h>
#include <server/game/SPCollision.h>
#include <server/game/SECollision.h>
#include <shared/util/SharedUtils.h>
#include <shared/CollisionGEvent.h>


Collision::Collision() {
	m_type = C,
	m_a = m_b = NULL;
	m_closeA = m_closeB = m_poi = m_collision_normal = D3DXVECTOR3(0.0, 0.0, 0.0);
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
{
	// calculate point of impact
	D3DXVECTOR3 delta_pos = closeA - closeB;
	D3DXVec3Normalize(&delta_pos, &delta_pos);

	D3DXVECTOR3 poi = (delta_pos * b->m_radius + closeB) + (-delta_pos * a->m_radius + closeA);
	poi /= 2;

	m_collision_normal = delta_pos;
	m_poi = poi;
}

Collision * Collision::generateCollision(ServerEntity *a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB)
{
	ServerEntity * one, * two;
	Collision * c;
	D3DXVECTOR3 temp;

	if (DEBUG) {
		if ((a->m_type == SHIP && b->m_type == MOTHERSHIP) || (b->m_type == MOTHERSHIP && a->m_type == SHIP)) {
			a->print();
			b->print();
			cout << "Closest to A: " << closeA << " , Closest to B: " << closeB << endl;
		}
	}

	// Ships & Resources
	if(((one = a)->m_type == RESOURCE && (two = b)->m_type == SHIP) || ((one = b)->m_type == RESOURCE && (two = a)->m_type == SHIP && ((temp = closeA) || 1) && ((closeA = closeB) || 1) && ((closeB = temp) || 1))){
		c = new SRCollision(two, one, closeB, closeA, 0.8, 0.6);

		return c;		
	}

	// Give/Take Resource to Mothership
	if(((one = a)->m_type == SHIP && (two = b)->m_type == MOTHERSHIP) || ((one = b)->m_type == SHIP && (two = a)->m_type == MOTHERSHIP && ((temp = closeA) || 1) && ((closeA = closeB) || 1) && ((closeB = temp) || 1))){
		c = new MSCollision(two, one, closeB, closeA, 0.8, 0.6);
		return c; 
	}

	// Ship to Ship
	if(((one = a)->m_type == SHIP && (two = b)->m_type == SHIP) ){
		c = new SSCollision(one, two, closeA, closeB, 0.8, 0.6);
		return c;
	}

	// Asteroid hits Ship
	if(((one = a)->m_type == SHIP && (two = b)->m_type == ASTEROID) || ((one = b)->m_type == SHIP && (two = a)->m_type == ASTEROID && ((temp = closeA) || 1) && ((closeA = closeB) || 1) && ((closeB = temp) || 1))){
		c = new SACollision(one, two, closeA, closeB, 0.8, 0.6);
		return c;
	}

	//Resource and Mothership
	if(((one = a)->m_type == RESOURCE && (two = b)->m_type == MOTHERSHIP) || ((one = b)->m_type == RESOURCE && (two = a)->m_type == MOTHERSHIP  && ((temp = closeA) || 1) && ((closeA = closeB) || 1) && ((closeB = temp) || 1))){
		c = new RMCollision(one, two, closeA, closeB, 0.8, 0.6);
		return c;
	}

	// TractorBeam
	if(((one = a)->m_type == TRACTORBEAM && (two = b)->m_type) || ((one = b)->m_type == TRACTORBEAM && (two = a)->m_type  && ((temp = closeA) || 1) && ((closeA = closeB) || 1) && ((closeB = temp) || 1))){
		c = new TCollision(one, two, closeA, closeB, 0.8, 0.6);
		return c;
	}

	// Resource
	if(((one = a)->m_type == RESOURCE) && ((one = b)->m_type == RESOURCE)){
		c = new RRCollision(one, two, closeA, closeB, 0.8, 0.6);
		return c;
	}

	
	// Resource Temp
	if(((one = a)->m_type == RESOURCE)|| ((one = b)->m_type == RESOURCE)){
		//rtn = false; // temporarily disabling all collisions with resources because of the infinite movement
		return NULL;
	}
	
	
	// Powerup Temp - until is given some implementation
	if(((one = a)->m_type == POWERUP && (two = b))|| ((one = b)->m_type == POWERUP && (two = a))){
		//c = new PPCollision(one, two, closeA, closeB, 0.8, 0.6);
		return NULL;
	}
	

	if(((one = a)->m_type == POWERUP && (two = b)->m_type == SHIP)|| ((one = b)->m_type == POWERUP && (two = a)->m_type == SHIP && ((temp = closeA) || 1) && ((closeA = closeB) || 1) && ((closeB = temp) || 1))){
		c = new SPCollision(two, one, closeB, closeA, 0.8, 0.6);
		return c;
	}

	//Extractor and Ship
	if(((one = a)->m_type == EXTRACTOR && (two = b)->m_type == SHIP)|| ((one = b)->m_type == EXTRACTOR && (two = a)->m_type == SHIP && ((temp = closeA) || 1) && ((closeA = closeB) || 1) && ((closeB = temp) || 1))){
		c = new SECollision(two, one, closeB, closeA, 0.8, 0.6);
		return c;
	}

	if(((one = a)->m_type == ASTEROID && (two = b)->m_type == MOTHERSHIP) || ((one = b)->m_type == ASTEROID && (two = a)->m_type == MOTHERSHIP )){
		c = new Collision(AM, a, b, closeA, closeB, 0.9, 0.6);
		return c;
	}

	if(((one = a)->m_type == ASTEROID && (two = b)->m_type == ASTEROID)){
		c = new Collision(AA, a, b, closeA, closeB, 0.9, 0.6);
		return c;
	}
	
	if(((one = a)->m_type == ASTEROID && (two = b)->m_type == EXTRACTOR) || ((one = b)->m_type == ASTEROID && (two = a)->m_type == EXTRACTOR) ){
		c = new Collision(AE, a, b, closeA, closeB, 0.9, 0.6);
		return c;
	}
	
	c = new Collision(C, a, b, closeA, closeB, 0.9, 0.6);

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
	if(!(m_a->m_type == SHIP && ((S_Ship *)m_a)->checkShield())) {
		if (m_b->m_type == SHIP && ((S_Ship *)m_b)->checkShield()) {
			m_a->applyLinearImpulse(jN * 2);
		} else { 
			m_a->applyLinearImpulse(jN);
		}


		D3DXVECTOR3 angular_impulse, vector_to_point = m_poi - m_a->m_pos;
		D3DXVec3Cross(&angular_impulse, &vector_to_point, &(jN / 3)); // Cross product finds torque
		m_a->applyAngularImpulse(angular_impulse);
	}

	
	if(!(m_b->m_type == SHIP && ((S_Ship *)m_b)->checkShield())) {
		if (m_a->m_type == SHIP && ((S_Ship *)m_a)->checkShield()) {
			m_b->applyLinearImpulse(-jN * 2);
		} else { 
			m_b->applyLinearImpulse(-jN);
		}

		D3DXVECTOR3 angular_impulse, vector_to_point = m_poi - m_b->m_pos;
		D3DXVec3Cross(&angular_impulse, &vector_to_point, &(-jN / 3)); // Cross product finds torque
		m_b->applyAngularImpulse(angular_impulse);
	}

	return new CollisionGEvent(m_a->m_id, m_b->m_id, m_poi, impulse, m_type);
}
