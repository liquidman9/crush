/*
 * PhysicsWorld.h
 */

// Global includes 

#pragma once
#include <vector>
#include <d3dx9.h>

// Project includes
#include <server/game/S_Ship.h>
#include <server/game/S_Asteroid.h>
#include <server/game/S_Resource.h>
#include <server/game/S_TractorBeam.h>
#include <server/game/S_Mothership.h>
#include <server/game/Boundary.h>
#include <server/game/Collision.h>


#pragma comment(lib,"D3dx9.lib")

using namespace std;

class PhysicsWorld {
public:
	float m_worldRadius;
	vector<ServerEntity *> entities;
	vector<Boundary> boundaries;
	vector<D3DXVECTOR3> forceVectors;
	vector<float> forceMags;

	void collision(float delta_time);
	void update(float delta_time);
	Collision * checkCollision(ServerEntity&,ServerEntity&);	bool checkCollision(ServerEntity&, Boundary&);
	bool typeResponse(ServerEntity *, ServerEntity *);
	void respond(ServerEntity *, ServerEntity *);
	void respond(ServerEntity *, Boundary);
	void checkInBounds(ServerEntity *);

};