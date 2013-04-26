
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
#include <server/game/PhysicsWorld.h>
#include <shared/ConfigSettings.h>



using namespace std;

typedef map<unsigned int, shared_ptr<S_Ship>> playerMap_t; 


void initializeGameState(GameState<Entity> &gameState, PhysicsWorld &world, playerMap_t &playerShips) {
	D3DXVECTOR3 m_pos(-10,2,2);
	Quaternion m_dir(0, 0, 0, 1);
	S_Ship *test = new S_Ship(m_pos, m_dir, 0);
	playerShips.insert(pair<unsigned int, shared_ptr<S_Ship>>(0,test));
	gameState.push_back(test);
	world.entities.push_back(test);

	D3DXVECTOR3 m_pos1(10,1,1);
	Quaternion m_dir1(0.25, 0.25, 0.25, 0.25);
	S_Ship *test2 = new S_Ship(m_pos1, m_dir1, 1);
	playerShips.insert(pair<unsigned int, shared_ptr<S_Ship>>(1,test2));
	gameState.push_back(test2);
	world.entities.push_back(test2);


	D3DXVECTOR3 m_pos2(0,0,0);
	Quaternion m_dir2(0, 0, 0, 0);
	S_Asteroid *test3 = new S_Asteroid(m_pos2, m_dir2);
	gameState.push_back(test3);
	world.entities.push_back(test3);


	D3DXVECTOR3 m_pos3(0,0,15);
	Quaternion m_dir3(0, 0, 0, 0);
	S_Asteroid *test4 = new S_Asteroid(m_pos3, m_dir3);
	gameState.push_back(test4);
	world.entities.push_back(test4);


	D3DXVECTOR3 m_pos4(0,0,-15);
	S_Asteroid *test5 = new S_Asteroid(m_pos4, m_dir3);
	gameState.push_back(test5);
	world.entities.push_back(test5);

	D3DXVECTOR3 m_pos5(0,0,-35);
	S_Asteroid *test6 = new S_Asteroid(m_pos5, m_dir3);
	gameState.push_back(test6);
	world.entities.push_back(test6);
}

void SpawnShip(int client_id, GameState<Entity> &gameState, PhysicsWorld &world, playerMap_t &playerShips) {
	D3DXVECTOR3 m_pos(-10,2,2);
	Quaternion m_dir(0, 0, 0, 1);
	S_Ship *tmp = new S_Ship(m_pos, m_dir, client_id);
	playerShips.insert(pair<unsigned int, shared_ptr<S_Ship>>(client_id,tmp));
	gameState.push_back(tmp);
	world.entities.push_back(tmp);
}

void addNewClients(const vector<unsigned int> &cc, GameState<Entity> &gameState, PhysicsWorld &world, playerMap_t &playerMap) {
	for(auto it= cc.begin(); it != cc.end(); it++) {
		auto player = playerMap.find(*it);
		if(player == playerMap.end()) {
			SpawnShip(*it, gameState, world, playerMap);
		}
	}
}

void moveClients(const EventBuff_t &clientInput, const playerMap_t &playerMap){
	for(auto it = clientInput.begin(); it != clientInput.end(); it++) {
		auto player = playerMap.find(it->first);
		if(player != playerMap.end()) {
			player->second->addPlayerInput(*(it->second));
		}
	}
}

long long milliseconds_now() {
    static LARGE_INTEGER s_frequency;
    static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
    if (s_use_qpc) {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return (1000LL * now.QuadPart) / s_frequency.QuadPart;
    } else {
        return GetTickCount();
    }
}


//sleep for time remaining tick
inline void endOfTick(ofstream &f_error, long long start) {
	double sleeptime = (double)1000/60 - (milliseconds_now() - start);
	if(sleeptime <= 0) {
		f_error << "Clock tick violated! " << sleeptime << endl;
	} else {
		Sleep((DWORD)sleeptime);
	}
}


int main(){
	ofstream f_error("server_error.log");

	//initialize Server
	int port = DEFAULT_PORT;
	ConfigSettings::config->getValue("network_serverPort", port);	
	NetworkServer server((unsigned short) port);

	//intialize game State
	PhysicsWorld world;
	GameState<Entity> gameState;
	EventBuff_t clientInput;
	playerMap_t playerMap;  //used to keep track of playerid's to player ships

	initializeGameState(gameState, world, playerMap);

	long long start;

	for(;;) {
		start = milliseconds_now();
		clientInput = server.getEvents();
		addNewClients(server.getNewClientIDs(), gameState, world, playerMap);
		if(!clientInput.empty()) {
			moveClients(clientInput, playerMap);
		}
		world.update(1.0f/60.0f);
		server.broadcastGameState(gameState);
	
		endOfTick(f_error, start);
	}

	return 0;
}
