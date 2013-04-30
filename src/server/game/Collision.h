/*
 * Collison.h
 */

// Project includes
#include <server/game/ServerEntity.h>

class Collision {
public:
	ServerEntity * m_a, * m_b;
	D3DXVECTOR3 m_closeA, m_closeB;

	static Collision * generateCollision(ServerEntity * a, ServerEntity * b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB);
	void resolve();

	Collision();

private:
	Collision(ServerEntity * a, ServerEntity *b, D3DXVECTOR3 closeA, D3DXVECTOR3 closeB);
};