/*
 * S_Ship.cpp
 */

// Project includes
#include <server/game/Collision.h>

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
	switch (a->m_type) {		
	}

	return new Collision(a, b, closeA, closeB);
}

void Collision::resolve()
{
	// calculate point of impact
	D3DXVECTOR3 poi;
	D3DXVec3Normalize(&poi, &m_closeB);
	poi *= m_a->m_radius;
	poi += m_closeA;

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

	D3DXVECTOR3 vAB = m_a->m_velocity - m_b->m_velocity;

	// Calculate normal vector of velocity
	D3DXVECTOR3 n = m_closeA - m_closeB;
	float vN = D3DXVec3Dot(&vAB, &n);
	float nN = D3DXVec3Dot(&n, &n);

	// calculate the distance between point of contact and centers of mass
	D3DXVECTOR3 distA = poi - m_a->m_pos;
	distA = D3DXVECTOR3(abs(distA.x), abs(distA.y), abs(distA.z));
	D3DXVECTOR3 distB = poi - m_b->m_pos;
	distB = D3DXVECTOR3(abs(distB.x), abs(distB.y), abs(distB.z));

	// calculate angular portion of impulse
	D3DXVECTOR3 inertA;  
	D3DXVec3Cross(&inertA, &distA, &n);
	D3DXVECTOR3 inertB;  
	D3DXVec3Cross(&inertB, &distB, &n);

	inertA.x = m_a->m_rot_inertia_inverse.x * inertA.x;
	inertA.y = m_a->m_rot_inertia_inverse.y * inertA.y;
	inertA.z = m_a->m_rot_inertia_inverse.z * inertA.z;
	inertB.x = m_b->m_rot_inertia_inverse.x * inertB.x;
	inertB.y = m_b->m_rot_inertia_inverse.y * inertB.y;
	inertB.z = m_b->m_rot_inertia_inverse.z * inertB.z;

	D3DXVec3Cross(&inertA, &inertA, &distA);
	D3DXVec3Cross(&inertB, &inertB, &distB);

	D3DXVECTOR3 inertBoth = inertA + inertB;

	// calculate the impulse
	float impulse = (-(1 + m_a->m_elastic) * vN) / ((nN * (m_a->m_mass_inverse + m_b->m_mass_inverse)));
	D3DXVECTOR3 jN = impulse * n;
	D3DXVECTOR3 jNRA;
	D3DXVECTOR3 jNRB;
	D3DXVec3Cross(&jNRA, &distA, &jN);
	D3DXVec3Cross(&jNRB, &distB, &jN);



	// Add the impulse to both entities
	m_a->t_impulse += jN;
	m_b->t_impulse += -jN;
	//m_a->t_angular_impulse += jNRA;
	//m_b->t_angular_impulse += -jNRB;

	/*
	// calculate the force vector of each collision
	D3DXVECTOR3 forceOnA = m_b->m_mass * (m_b->m_velocity / timeB);
	D3DXVECTOR3 forceOnB = m_a->m_mass * (m_a->m_velocity / timeA);

	m_a->applyImpulse(forceOnA, poi, timeA);
	m_b->applyImpulse(forceOnB, poi, timeB);
	*/
}