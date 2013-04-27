#pragma once
// Global includes
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <time.h>

// Project includes
#include <server/network/NetworkServer.h>
#include <shared/InputState.h>
#include <server/game/S_Ship.h>
#include <server/game/S_Asteroid.h>
#include <server/game/S_Resource.h>
#include <server/game/S_TractorBeam.h>
#include <server/game/S_Mothership.h>
#include <server/game/PhysicsWorld.h>
#include <shared/ConfigSettings.h>

#define SERVER_ERROR_FILE "server_error.log"


typedef map<unsigned int, S_Ship*> PlayerMap_t;


class Server
{
public:
	Server(unsigned int port);
	
	//starts server loop. Must be called before any other functions.
	void start();

	//restart server
	void restart();

	//reload server variables from config file
	//variable updates need to be implemented still.
	//Please add any variables you feel would be good to change on the fly.
	void reload();

	virtual ~Server(void);


private:
	//functions
	void loop();
	
	//load/reload variables set by the config file
	void reloadConfig();

	//initialize the gameState
	void initializeGameState();

	//add any new clients that have conntected
	void addNewClients();
	
	//spawn a new ship for client_id
	void spawnShip(unsigned int client_id);

	//move clients that are currently known
	void moveClients();

	//used in startTick and endOftick
	long long milliseconds_now();

	//mark the start of the tick
	inline void startTick();

	//sleep for the remaining of the tick
	inline void endOfTick();


	//variables
	NetworkServer m_server;
	PhysicsWorld m_world;
	GameState<Entity> m_gameState;
	EventBuff_t m_clientInput;
	PlayerMap_t m_playerMap;
	long long m_startTick;
	ofstream m_f_error;
	bool m_start;
	bool m_reload;

	//threadStuff
	CRITICAL_SECTION m_cs;
	HANDLE m_hThread;

	static unsigned __stdcall StaticLoopEntry(void * pThis) {
		Server * pthX = (Server*)pThis;
		pthX->loop();	
		return 1;
	}

};

