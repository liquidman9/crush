#pragma once
// Global includes
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <time.h>

// Project includes
#include <server/network/NetworkServer.h>
#include <shared/InputState.h>
#include <server/game/PowerupSource.h>
#include <server/game/S_Ship.h>
#include <server/game/S_Asteroid.h>
#include <server/game/S_Resource.h>
#include <server/game/S_TractorBeam.h>
#include <server/game/S_Mothership.h>
#include <server/game/S_Extractor.h>
#include <server/game/S_Powerup.h>
#include <server/game/PhysicsWorld.h>
#include <shared/ConfigSettings.h>

#define SERVER_ERROR_FILE "server_error.log"

#define MS_DIST_FROM_MINE 300
#define SHIP_DIST_FROM_MINE 200
#define TIME_LIMIT 300000


typedef map<unsigned int, S_Ship*> PlayerMap_t;
typedef map<unsigned int, S_Mothership*> MothershipMap_t;


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

	//pauses/unpauses gamestate from being updated
	//first call pauses server, second call unpauses, etc
	void pause();

	void startGame();

	virtual ~Server(void);


private:
	//functions
	void loop();
	
	//load/reload variables set by the config file
	void reloadConfig();

	//initialize the gameState
	void initializeGameState();

	// add initial resources and the mine
	void setUpExtractor();

	// add initial asteroids to the map
	void setUpAsteroids();

	void setUpBoundaries();

	void setUpPowerups();

	//add any new clients that have conntected
	void addNewClients(vector<pair<unsigned int, string>> const &cc);
	
	//spawn a new ship for client_id
	void spawnShip(unsigned int client_id);

	//spawn a new mothership for client_id
	void spawnMothership(unsigned int client_id);

	// spawn an Asteroid
	void spawnAsteroid();

	//move clients that are currently known
	void moveClients();

	//used in startTick and endOftick
	long long milliseconds_now();

	//mark the start of the tick
	inline void startTick();

	//sleep for the remaining of the tick
	inline void endOfTick();

	void removeDisconClients();

	D3DXVECTOR3 genSpawnPos(unsigned int client_id, unsigned int distance);

	Quaternion genShipSpawnDir(unsigned int client_id);

	Quaternion genMotherShipSpawnDir(unsigned int client_id);

	void initializeGameClock();

	void updateGameClock();

	void declareWinner();

	bool gameOver();

	void updateScore();


	//variables
	NetworkServer m_server;
	PhysicsWorld m_world;
	GameState<Entity> m_gameState;
	EventBuff_t m_clientInput;
	PlayerMap_t m_playerMap;
	MothershipMap_t m_mothershipMap;
	S_Extractor * m_extractor;
	PowerupSource * m_powerupSource;
	long long m_startTick;
	long long m_endClock;
	int m_timeLimit;
	bool m_startGame;
	ofstream m_f_error;
	bool m_start;
	bool m_reload;
	bool m_pause;

	//threadStuff
	CRITICAL_SECTION m_cs;
	HANDLE m_hThread;

	static unsigned __stdcall StaticLoopEntry(void * pThis) {
		Server * pthX = (Server*)pThis;
		pthX->loop();	
		return 1;
	}

};

