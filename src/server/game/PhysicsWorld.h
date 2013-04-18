/*
 * PhysicsWorld.h
 */
#include <shared/game/Entity.h>
#include <vector>
#include <d3dx9.h>
#include <server/game/ServerEntity.h>
#include <server/game/Boundary.h>



using namespace std;

class PhysicsWorld {
public:
	vector<ServerEntity *> entities;
	vector<Boundary> boundaries;
	vector<D3DXVECTOR3> forceVectors;
	vector<float> forceMags;

	void update();
	bool checkCollision(ServerEntity,ServerEntity);
	bool checkCollision(ServerEntity, Boundary);
	void respond(ServerEntity *, ServerEntity *);
	void respond(ServerEntity *, Boundary);

};