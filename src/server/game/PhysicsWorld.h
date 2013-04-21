/*
 * PhysicsWorld.h
 */

// Global includes 
#include <vector>
#include <d3dx9.h>

// Project includes
#include <shared/game/Entity.h>
#include <server/game/ServerEntity.h>
#include <server/game/Boundary.h>

#pragma comment(lib,"D3dx9.lib")

using namespace std;

class PhysicsWorld {
public:
	vector<ServerEntity *> entities;
	vector<Boundary> boundaries;
	vector<D3DXVECTOR3> forceVectors;
	vector<float> forceMags;

	void update(float delta_time);
	bool checkCollision(ServerEntity,ServerEntity);
	bool checkCollision(ServerEntity, Boundary);
	void respond(ServerEntity *, ServerEntity *);
	void respond(ServerEntity *, Boundary);

};