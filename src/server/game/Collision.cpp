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
	default:
		return new Collision(a, b, closeA, closeB);
	}

}

void Collision::resolve()
{
	// calculate point of impact
	D3DXVECTOR3 poi;
	D3DXVec3Normalize(&poi, &m_closeB);
	poi *= -(m_a->m_radius);
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

	// If the velocities are moving apart, don't do anything
	if(-vN < -0.01f || nN < 0.01)
		return;

	
	cout << "a: \tpos: (" << m_a->m_pos.x << ", " << m_a->m_pos.y << ", " << m_a->m_pos.z << ") closest point: (" << m_closeA.x << ", " << m_closeA.y << ", " << m_closeA.z << ") " << endl; 
	cout << "\tfront: (" << m_a->m_pFront.x << ", " << m_a->m_pFront.y << ", " << m_a->m_pFront.z << ") back: (" << m_a->m_pBack.x << ", " << m_a->m_pBack.y << ", " << m_a->m_pBack.z << ") " << endl; 
	cout << "\tb: pos: (" << m_b->m_pos.x << ", " << m_b->m_pos.y << ", " << m_b->m_pos.z << ") closest point: (" << m_closeB.x << ", " << m_closeB.y << ", " << m_closeB.z << ") " << endl; 
	cout << "\tfront: (" << m_b->m_pFront.x << ", " << m_b->m_pFront.y << ", " << m_b->m_pFront.z << ") back: (" << m_b->m_pBack.x << ", " << m_b->m_pBack.y << ", " << m_b->m_pBack.z << ") " << endl; 
	cout << "poi: (" << poi.x << ", " << poi.y << ", " << poi.z << ")" << endl;
	

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
	//cout << inertBoth.x << ", " << inertBoth.y << ", " << inertBoth.z << endl;

	// calculate the impulse
	//float impulse = (-(1 + m_a->m_elastic) * vN) / ((nN * (m_a->m_mass_inverse + m_b->m_mass_inverse)));
	float impulse = (-(1 + m_a->m_elastic) * vN) / ((nN * (m_a->m_mass_inverse + m_b->m_mass_inverse)) + D3DXVec3Dot(&inertBoth, &n));
	
	cout << D3DXVec3Dot(&inertBoth, &n) << endl;
	cout << impulse << endl;
	
	D3DXVECTOR3 jN = impulse * n;
	D3DXVECTOR3 jNRA;
	D3DXVECTOR3 jNRB;
	D3DXVec3Cross(&jNRA, &distA, &jN);
	D3DXVec3Cross(&jNRB, &distB, &jN);

	// Add the impulse to both entities
	m_a->t_impulse += jN;
	m_b->t_impulse += -jN;
	m_a->t_angular_impulse += (.15 * jNRA);
	m_b->t_angular_impulse -= (.15 * jNRB);
}