#include "Server.h"
#include <shared/util/SharedUtils.h>

using namespace shared::utils;
using namespace server;

Server::Server(unsigned int port):m_server(port)
{
	m_timeLimit = game::timelimit;
	m_pause = false;
	m_start = false;
	m_reload = false;
	m_endClock = 0;
	m_hThread = NULL;
	string error_file = SERVER_ERROR_FILE;
	ConfigSettings::config->getValue("server_errorLogFile", error_file);
	m_f_error = ofstream(error_file);
	if(!m_f_error) {
		cerr << "error opening log file: " << error_file << endl;
	}
}


void Server::start() {
	m_pause = false;
	m_start = true;
	if(m_hThread == NULL) {
		unsigned int threadID;
		m_hThread = (HANDLE)_beginthreadex( NULL, // security
			0,             // stack size
			Server::StaticLoopEntry,// entry-point-function
			this,           // arg list holding the "this" pointer
			0,		
			&threadID );
		SetThreadPriority(m_hThread, THREAD_PRIORITY_HIGHEST);
	}
}

void Server::restart() {
	m_pause = false;
	m_startGame = true;
	m_start = true;
}

void Server::reload() {
	m_reload = true;
}

void Server::pause() {
	m_pause = !m_pause;
}

void Server::startGame() {
	m_startGame = true;
}

void Server::reloadConfig() {
	ConfigSettings::config->reloadSettingsFile();

	// Load config
	server::initFromConfig();

	// Update values as necessary
	m_timeLimit = server::game::timelimit;
}

void Server::setUpExtractor() {
	D3DXVECTOR3 m_pos1(0,0,0);
	Quaternion m_dir1(0.0, 0.0, 0.0, 1.0);

	m_extractor = new S_Extractor( m_pos1,m_dir1);
	m_extractor->respawn();


	m_gameState.push_back(m_extractor);
	m_world.entities.push_back(m_extractor);
	S_Resource * res = m_extractor->getResource();

	m_gameState.push_back(res);
	m_world.entities.push_back(res);

	//m_extractor->setStart(milliseconds_now());
}

void Server::setUpAsteroids() {
	cout << "Begin setting up asteroids..." <<endl;
	srand((unsigned int)milliseconds_now());

	Quaternion defaultAsteroidDir(0, 0, 0, 1);
	using namespace server::world;
	for(int i = 0; i < asteroids_num; i++) {
		S_Asteroid * newAsteroid;
		bool fits = false;
		float scale = 1;
		D3DXVECTOR3 asteroidPos(0,0,0);

		while(!fits) {
			scale =  (rand_float() * asteroids_scale_range) + asteroids_scale_start;
			asteroidPos = Vec3RandRange(asteroids_pos_start, asteroids_pos_range);
			cout << i;
			newAsteroid = new S_Asteroid(asteroidPos, defaultAsteroidDir, scale);

			fits = true;

			// Check with Current Entities
			for(unsigned int j = 0; j < m_world.entities.size(); j++) {
				ServerEntity *cmp = m_world.entities[j];
				if(m_world.checkCollision(*cmp,*newAsteroid) != NULL){
					fits = false;
					break;
				}
			}

			// check with spawn spots of players 1 -4

		}

		D3DXVECTOR3 initial_vel = Vec3RandRange(-asteroids_vel_range/2, asteroids_vel_range) * newAsteroid->m_mass,
					initial_rot_vel = Vec3RandRange(-asteroids_rot_vel_range/2, asteroids_rot_vel_range);
		D3DXVECTOR4 temp;
		D3DXVec3Transform(&temp, &initial_rot_vel, &newAsteroid->m_rot_inertia);
		initial_rot_vel = D3DXVECTOR3(temp.x, temp.y, temp.z);

		newAsteroid->applyLinearImpulse(initial_vel);
		newAsteroid->applyAngularImpulse(initial_rot_vel);

		m_gameState.push_back(newAsteroid);
		m_world.entities.push_back(newAsteroid);
	}

	cout << "Finished setting up asteroids..." <<endl;
}

void Server::setUpBoundaries() {
	m_world.m_worldRadius = world::size;

}

void Server::setUpPowerups() {

	vector<D3DXVECTOR3> points;
	points.push_back(D3DXVECTOR3(0,100,0));
	points.push_back(D3DXVECTOR3(0,-100,0));

	m_powerupSource = new PowerupSource(points, milliseconds_now());

	for(unsigned int i = 0; i < m_powerupSource->m_powerups.size(); i++) {
		m_gameState.push_back(m_powerupSource->m_powerups[i]);
		m_world.entities.push_back(m_powerupSource->m_powerups[i]);
	}
}

void Server::initializeGameState() {
	m_gameState.clear();
	m_server.broadcastGameState(m_gameState);
	m_playerMap.clear();
	m_mothershipMap.clear();
	m_world.entities.clear();
	addNewClients(m_server.getConnectedClientIDs());
	

	setUpBoundaries();


}


void Server::removeDisconClients() {
	vector<unsigned int> disconClients = m_server.getDisconClients();
	for (auto it = disconClients.begin(); it != disconClients.end(); it++){
		auto player = m_playerMap.find(*it);
		if(player != m_playerMap.end()) {
			player->second->m_playerName = "";
			//would normally remove here, but no good way of removing yet
			//may not even implement this
		}
	}
}

void Server::initializeGameClock() {
	m_endClock = milliseconds_now() + m_timeLimit;
}

void Server::updateGameClock() {
	long long time_remaining = m_endClock - milliseconds_now();
	if(time_remaining > 0) {
		m_gameState.setTime(time_remaining);
	} else {
		m_gameState.setTime(0);
	}
}

bool Server::gameOver() {
	return m_gameState.getRemainingTimeNum() == 0;
}

void Server::declareWinner() {
	unsigned int max = 0;
	unsigned int client = 0;
	for(auto it = m_mothershipMap.begin(); it != m_mothershipMap.end(); it++) {
		if(it->second->m_resourceList.size() > max) {
			max = it->second->m_resourceList.size();
			client = it->first;
		}
	}
	m_gameState.setWinner((int) client);
}

void Server::updateScore() {
	scoreList_t list;
	for(auto it = m_mothershipMap.begin(); it != m_mothershipMap.end(); it++) {
		list.push_back(pair<unsigned int, unsigned char>(it->first, (unsigned char)it->second->m_resources));
	}
	m_gameState.setScore(list);
}


void Server::loop() {
	float loopCycle = (float) 1.0/60.0f;
	long long prev_tick = milliseconds_now();
	bool print_once = false;
	for(;;) {
		startTick();
		if(m_start) {
			initializeGameState();
			reloadConfig();
			setUpExtractor();
			setUpAsteroids();
			setUpPowerups();
		
			cout << "CRUSH Server has started" << endl;
			m_start = false;
		} else if (m_reload) {
			reloadConfig();
			cout << "Config file reloaded" << endl;
			m_reload = false;
		}
		
		if(m_pause) {
			m_server.broadcastGameState(m_gameState);
			endOfTick();
			continue;
		}
		if (m_startGame) {
			print_once = true;
			initializeGameClock();
			cout << "Game Started" << endl;
			m_startGame = false;
		}

		updateGameClock();
		updateScore();
		if(gameOver()) {
			declareWinner();
			if(print_once) {
				cout << "Game ended \nClient " << m_gameState.getWinner() 
					<< " wins" << endl;
				print_once = false;
			}
		}

		//(optional) currently just removes the name from a player's
		//ship who has disconnected
		removeDisconClients();

		addNewClients(m_server.getNewClientIDs());


		m_clientInput = m_server.getEvents();
		if(!m_clientInput.empty()) {
			moveClients();
		}		

		long long cur = milliseconds_now();
		float physics_delta = (float)(milliseconds_now() - prev_tick) / 1000.0f;
		prev_tick = cur;

		
		m_powerupSource->update(milliseconds_now());

		m_world.collision(physics_delta);

		for(auto i = m_playerMap.begin(); i != m_playerMap.end(); i++) {
			i->second->calcTractorBeam();
		}
		m_world.update(physics_delta);

		// Add new resource (if spawned)
		S_Resource * res = m_extractor->getResource();
		if(res != NULL) {
			cout << "Adding resource" << endl;
			m_gameState.push_back(res);
			m_world.entities.push_back(res);
		}

		m_server.broadcastGameState(m_gameState);

		endOfTick();
	}

}

void Server::addNewClients(vector<pair<unsigned int, string>> const &cc) {
	//vector<pair<unsigned int, string>> cc = m_server.getNewClientIDs();
	//get new client ids and if they aren't in the playerMap add them
	for(auto it= cc.begin(); it != cc.end(); it++) {
		auto player = m_playerMap.find(it->first);
		if(player == m_playerMap.end()) {
			spawnShip(it->first);
			spawnMothership(it->first);
		}
	}
}

void Server::spawnShip(unsigned int client_id) {
	// Temp
	S_Ship *ship = new S_Ship(genSpawnPos(client_id, world::ship_spawn_distance_from_center), genShipSpawnDir(client_id), client_id);
	m_playerMap.insert(pair<unsigned int, S_Ship*>(client_id,ship));
	m_gameState.push_back(ship);
	m_world.entities.push_back(ship);

	S_TractorBeam *beam = new S_TractorBeam(ship);
	m_gameState.push_back(beam);
	m_world.entities.push_back(beam);

	ship->m_tractorBeam = beam;

}

void Server::spawnMothership(unsigned int client_id) {
	S_Mothership *tmp = new S_Mothership(genSpawnPos(client_id, world::mothership_distance_from_center), genMotherShipSpawnDir(client_id), client_id);
	m_mothershipMap.insert(pair<unsigned int, S_Mothership*>(client_id,tmp));
	m_gameState.push_back(tmp);
	m_world.entities.push_back(tmp);
}

D3DXVECTOR3 Server::genSpawnPos(unsigned int client_id, float distance) {
	D3DXVECTOR3 rtn;
	if (client_id % 2 == 0){
		rtn = D3DXVECTOR3((float)(-1.0+client_id)*distance, 0, 0);
	} else {
		rtn = D3DXVECTOR3(0, 0, (float)(-2.0+client_id)*distance);
	}
	return rtn;
}

Quaternion Server::genShipSpawnDir(unsigned int client_id) {
	Quaternion rtn;
	D3DXQuaternionRotationYawPitchRoll(&rtn, D3DXToRadian((float) (1.0-client_id)*90), 0 , 0);
	return rtn;
}

Quaternion Server::genMotherShipSpawnDir(unsigned int client_id) {
	Quaternion rtn;
	if(client_id % 2 == 0) {
		D3DXQuaternionRotationYawPitchRoll(&rtn, D3DXToRadian((client_id)*90), 0 , 0);
	} else {
		D3DXQuaternionRotationYawPitchRoll(&rtn, D3DXToRadian((float)-(2.0-client_id)*90), 0 , 0);
	}
	return rtn;
}



void Server::moveClients() {
	//add client inputs to every player that we have input for
	for(auto it = m_clientInput.begin(); it != m_clientInput.end(); it++) {
		auto player = m_playerMap.find(it->first);
		if(player != m_playerMap.end()) {
			player->second->addPlayerInput(*(it->second));
		}
	}
}



long long Server::milliseconds_now() {
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

inline void Server::startTick() {
	m_startTick = milliseconds_now();
}

inline void Server::endOfTick() {
	double sleeptime = (double)1000/60 - (milliseconds_now() - m_startTick);
	if(sleeptime <= 0) {
		m_f_error << "Clock tick violated! " << sleeptime << endl;
	} else {
		Sleep((DWORD)sleeptime);
	}
}

GameState<Entity> Server::getGameState() {
	return m_gameState;
}

Server::~Server(void)
{
	TerminateThread(m_hThread, 0);
	CloseHandle(m_hThread);
}





