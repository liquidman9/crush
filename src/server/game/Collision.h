/*
 * Collison.h
 */

// Project includes
#include <server/game/ServerEntity.h>

class Collision {
public:
	ServerEntity * m_a, * m_b;
	D3DXVECTOR3 m_closeA, m_closeB, m_poi, m_collision_normal;

	static Collision * generateCollision(ServerEntity * a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB);
	void resolve();

	Collision();

private:
	Collision(ServerEntity * a, ServerEntity *b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB);
};