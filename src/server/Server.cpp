#include "Server.h"


Server::Server(unsigned int port):m_server(port)
{
	m_timeLimit = TIME_LIMIT;
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

	ConfigSettings::config->getValue("gp_timeLimit", m_timeLimit); 
	//add variables to update here
}

void Server::setUpResourceMine() {
	D3DXVECTOR3 m_pos1(0,0,0);
	Quaternion m_dir1(0.0, 0.0, 0.0, 1.0);
	m_resourceMine = new Mine();
	S_Resource *res = m_resourceMine->respawn();
	m_gameState.push_back(res);
	m_world.entities.push_back(res);

	m_resourceMine->setStart(milliseconds_now());
}

void Server::setUpAsteroids() {
	
	int count = 20;
	int range = 400;
	int start = -200;
	Quaternion m_dir3(0, 0, 0, 1);
	for(int i = 0; i < count; i++) {

		float scale =  (rand() % S_Asteroid::s_range) + S_Asteroid::s_start;
		D3DXVECTOR3 m_pos4((rand()%range)+start,(rand()%range)+start,(rand()%range)+start);
		S_Asteroid *test5 = new S_Asteroid(m_pos4, m_dir3,scale);
		m_gameState.push_back(test5);
		m_world.entities.push_back(test5);
	}
	
}

void Server::setUpBoundaries() {

	float bound = 500;
	Boundary left = Boundary(D3DXVECTOR3(1.0f,0.0f,0.0f), D3DXVECTOR3(-bound,0.0f,0.0f));
	Boundary right = Boundary(D3DXVECTOR3(-1.0f,0.0f,0.0f), D3DXVECTOR3(bound,0.0f,0.0f));
	Boundary top = Boundary(D3DXVECTOR3(0.0f,-1.0f,0.0f), D3DXVECTOR3(0.0f,bound,0.0f));
	Boundary down = Boundary(D3DXVECTOR3(0.0f,1.0f,0.0f), D3DXVECTOR3(0.0f,-bound,0.0f));
	Boundary front = Boundary(D3DXVECTOR3(0.0f,0.0f,1.0f), D3DXVECTOR3(0.0f,0.0f,-bound));
	Boundary back = Boundary(D3DXVECTOR3(0.0f,0.0f,-1.0f), D3DXVECTOR3(0.0f,0.0f,bound));

	m_world.boundaries.push_back(left);
	m_world.boundaries.push_back(right);
	m_world.boundaries.push_back(top);
	m_world.boundaries.push_back(down);
	m_world.boundaries.push_back(front);
	m_world.boundaries.push_back(back);

}

void Server::initializeGameState() {
	m_gameState.clear();
	m_server.broadcastGameState(m_gameState);
	m_playerMap.clear();
	m_world.entities.clear();
	addNewClients(m_server.getConnectedClientIDs());
	

	setUpResourceMine();

	setUpAsteroids();

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
		list.push_back(pair<unsigned int, unsigned char>(it->first, (unsigned char)it->second->m_resourceList.size()));
	}
	m_gameState.setScore(list);
}


void Server::loop() {
	float loopCycle = (float) 1.0/60.0f;
	bool print_once = false;
	for(;;) {
		if(m_start) {
			initializeGameState();
			reloadConfig();
			cout << "CRUSH Server has started" << endl;
			m_start = false;
		} else if (m_reload) {
			reloadConfig();
			cout << "Config file reloaded" << endl;
			m_reload = false;
		}		
		startTick();
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

		if(m_resourceMine->checkRespawn(milliseconds_now())) {
			S_Resource * res = m_resourceMine->respawn();
			m_gameState.push_back(res);
			m_world.entities.push_back(res);
		}

		m_world.collision(loopCycle);

		for(auto i = m_playerMap.begin(); i != m_playerMap.end(); i++) i->second->calcTractorBeam();
		m_world.update(loopCycle);
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
	S_Ship *ship = new S_Ship(genSpawnPos(client_id, SHIP_DIST_FROM_MINE), genShipSpawnDir(client_id), client_id);
	m_playerMap.insert(pair<unsigned int, S_Ship*>(client_id,ship));
	m_gameState.push_back(ship);
	m_world.entities.push_back(ship);

	S_TractorBeam *beam = new S_TractorBeam(ship);
	m_gameState.push_back(beam);
	m_world.entities.push_back(beam);

	ship->m_tractorBeam = beam;
}

void Server::spawnMothership(unsigned int client_id) {
	S_Mothership *tmp = new S_Mothership(genSpawnPos(client_id, MS_DIST_FROM_MINE), genMotherShipSpawnDir(client_id), client_id);
	m_mothershipMap.insert(pair<unsigned int, S_Mothership*>(client_id,tmp));
	m_gameState.push_back(tmp);
	m_world.entities.push_back(tmp);
}

D3DXVECTOR3 Server::genSpawnPos(unsigned int client_id, unsigned int distance) {
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


Server::~Server(void)
{
	TerminateThread(m_hThread, 0);
	CloseHandle(m_hThread);
}



