/*
 * S_Ship.cpp
 */

// Project includes
#include <server/game/Collision.h>
#include <shared/util/SharedUtils.h>

Collision::Collision() {
	m_a = m_b = NULL;
	m_closeA = m_closeB = D3DXVECTOR3(0.0, 0.0, 0.0);
}

Collision::Collision(ServerEntity * a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB) :
	m_a(a),
	m_b(b),
	m_closeA(closeA),
	m_closeB(closeB)
{}

Collision * Collision::generateCollision(ServerEntity *a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB)
{
	if (DEBUG) {
		if ((a->m_type == SHIP && b->m_type == MOTHERSHIP) || (b->m_type == MOTHERSHIP && a->m_type == SHIP)) {
			a->print();
			b->print();
			cout << "Closest to A: "; shared::utils::printVec(closeA); cout << " , Closest to B: "; shared::utils::printVec(closeB); cout << endl;
		}
	}
	switch (a->m_type) {
	default:
		return new Collision(a, b, closeA, closeB);
	}

}

void Collision::resolve()
{
	float e_coll = 0.9;
	float u_fr = 0.5;
	D3DXVECTOR4 temp;
	// calculate point of impact
	D3DXVECTOR3 collision_normal, poi, delta_pos;

	delta_pos = m_closeA - m_closeB;

	D3DXVec3Normalize(&collision_normal, &delta_pos);
	poi = (collision_normal * m_b->m_radius + m_closeB) + (-collision_normal * m_a->m_radius + m_closeA);
	poi /= 2;
	
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
	D3DXVECTOR3 r_a = poi - m_a->m_pos;
	D3DXVECTOR3 r_b = poi - m_b->m_pos;

	D3DXVECTOR3 relative_linear_velocity = m_a->m_velocity - m_b->m_velocity;

	// Calculate rotation velocity at point of collision
	D3DXVECTOR3 a_collision_rotation_velocity, b_collision_rotation_velocity;
	D3DXVec3Cross(&a_collision_rotation_velocity, &m_a->m_angular_velocity, &r_a);
	D3DXVec3Cross(&b_collision_rotation_velocity, &m_b->m_angular_velocity, &r_b);

	D3DXVECTOR3 relative_angular_velocity = a_collision_rotation_velocity - b_collision_rotation_velocity;

	D3DXVECTOR3 relative_total_velocity = relative_linear_velocity + relative_angular_velocity;

	float vN = min(0.0f, D3DXVec3Dot(&relative_total_velocity, &collision_normal));
	float nN = D3DXVec3Dot(&collision_normal, &collision_normal);

	// calculate angular portion of impulse
	D3DXVECTOR3 inertA, inertB;  

	// I^-1(r x n)
	D3DXVec3Transform(&temp, D3DXVec3Cross(&inertA, &r_a, &collision_normal), &m_a->m_rot_inertia_inverse);
	inertA = D3DXVECTOR3(temp.x, temp.y, temp.z);

	D3DXVec3Transform(&temp, D3DXVec3Cross(&inertB, &r_b, &collision_normal), &m_b->m_rot_inertia_inverse);
	inertB = D3DXVECTOR3(temp.x, temp.y, temp.z);

	D3DXVECTOR3 inertBoth = *D3DXVec3Cross(&inertA, &inertA, &r_a) + *D3DXVec3Cross(&inertB, &inertB, &r_b);

	// calculate the impulse
	float impulse = (-(1 + e_coll) * vN) / (m_a->m_mass_inverse + m_b->m_mass_inverse + D3DXVec3Dot(&inertBoth, &collision_normal));

	//cout << D3DXVec3Dot(&inertBoth, &n) << endl;
	//cout << impulse << endl;
	
	D3DXVECTOR3 jN = impulse * collision_normal;

	// Friction force!

	D3DXVECTOR3 tangentVelocity = relative_total_velocity - (vN * collision_normal);

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

		float friction = max(-u_fr * impulse, min(u_fr * impulse, -vt / kt));

		jN += friction * tangent;
		cout << "Impulse force: " << impulse << endl;
		cout << "Friction force: " << friction << endl;
	}

	if (true) {
		cout << "Impulse vector: "; shared::utils::printVec(jN); cout << endl;
		cout << "DistA: "; shared::utils::printVec(r_a); cout << endl;
		cout << "DistB: "; shared::utils::printVec(r_b); cout << endl;
		cout << "POI: "; shared::utils::printVec(poi); cout << endl;
	}

	// Add the impulse to both entities
	m_a->applyImpulse(jN, poi);
	m_b->applyImpulse(-jN, poi);

	

}
