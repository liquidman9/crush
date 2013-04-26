#include "Server.h"


Server::Server(unsigned int port):m_server(port)
{
	m_start = false;
	m_reload = false;
	m_hThread = NULL;
	string error_file = SERVER_ERROR_FILE;
	ConfigSettings::config->getValue("server_errorLogFile", error_file);
	m_f_error = ofstream(error_file);
}


void Server::start() {
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
	m_start = true;
}

void Server::reload() {
	m_reload = true;
}

void Server::reloadConfig() {
	ConfigSettings::config->reloadSettingsFile();

	//add variables to update here
}

void Server::initializeGameState() {
	m_gameState.clear();
	m_playerMap.clear();
	m_world.entities.clear();
	D3DXVECTOR3 m_pos(-10,2,2);
	Quaternion m_dir(0, 0, 0, 1);
	S_Ship *test = new S_Ship(m_pos, m_dir, 0);
	m_playerMap.insert(pair<unsigned int, S_Ship*>(0,test));
	m_gameState.push_back(test);
	m_world.entities.push_back(test);

	D3DXVECTOR3 m_pos1(10,1,1);
	Quaternion m_dir1(0.25, 0.25, 0.25, 0.25);
	S_Ship *test2 = new S_Ship(m_pos1, m_dir1, 1);
	m_playerMap.insert(pair<unsigned int, S_Ship*>(1,test2));
	m_gameState.push_back(test2);
	m_world.entities.push_back(test2);


	D3DXVECTOR3 m_pos2(0,0,0);
	Quaternion m_dir2(0, 0, 0, 0);
	S_Asteroid *test3 = new S_Asteroid(m_pos2, m_dir2);
	m_gameState.push_back(test3);
	m_world.entities.push_back(test3);


	D3DXVECTOR3 m_pos3(0,0,15);
	Quaternion m_dir3(0, 0, 0, 0);
	S_Asteroid *test4 = new S_Asteroid(m_pos3, m_dir3);
	m_gameState.push_back(test4);
	m_world.entities.push_back(test4);


	D3DXVECTOR3 m_pos4(0,0,-15);
	S_Asteroid *test5 = new S_Asteroid(m_pos4, m_dir3);
	m_gameState.push_back(test5);
	m_world.entities.push_back(test5);

	D3DXVECTOR3 m_pos5(0,0,-35);
	S_Asteroid *test6 = new S_Asteroid(m_pos5, m_dir3);
	m_gameState.push_back(test6);
	m_world.entities.push_back(test6);
}


void Server::loop() {
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

		addNewClients();

		m_clientInput = m_server.getEvents();
		if(!m_clientInput.empty()) {
			moveClients();
		}

		m_world.update(1.0f/60.0f);
		m_server.broadcastGameState(m_gameState);
	
		endOfTick();
	}
}

void Server::addNewClients() {
	vector<unsigned int> cc = m_server.getNewClientIDs();
	//get new client ids and if they aren't in the playerMap add them
	for(auto it= cc.begin(); it != cc.end(); it++) {
		auto player = m_playerMap.find(*it);
		if(player == m_playerMap.end()) {
			spawnShip(*it);
		}
	}
}

void Server::spawnShip(unsigned int client_id) {
	D3DXVECTOR3 m_pos((FLOAT)-10*client_id,2,2);
	Quaternion m_dir(0, 0, 0, 1);
	S_Ship *tmp = new S_Ship(m_pos, m_dir, client_id);
	m_playerMap.insert(pair<unsigned int, S_Ship*>(client_id,tmp));
	m_gameState.push_back(tmp);
	m_world.entities.push_back(tmp);
}

void Server::moveClients() {
	//add client inputs to every player that we have
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



