#pragma once

// Global includes
#include <vector>
#include <map>

// Project includes
#include <shared/network/Network.h>
#include <shared/game/Entity.h>
#include <shared/ConfigSettings.h>

//typedef GameState State_t;

class NetworkServer :
	public Network
{
public:

	//creates a NetworkServer with default port and any ip address
	//Throws runtime_error exception if socket cannot be opened
	NetworkServer(void);

	//creates NetworkServer with ip and port number passed
	//note: ip should be in the form "127.0.0.1"
	//Throws runtime_error exception if socket cannot be opened
	NetworkServer(string ip, unsigned short port);

	//creates NetworkServer with any ip address and passed port
	//Throws runtime_error exception if socket cannot be opened
	NetworkServer(unsigned short port);

	//returns true always. Don't use this function.
	bool eventsAvailable() { return true; };

	//returns a vector of events received
	EventBuff_t getEvents();

	//Sends games State to all connected clients
	void broadcastGameState(GameState<Entity> const&);

	//returns a vector of connected Client IDs
	vector<pair<unsigned int,string>> getConnectedClientIDs();

	//returns a vector of that have connected since the last call to
	//getNewClientIDs;
	vector<pair<unsigned int, string>> getNewClientIDs();

	//returns a vector of ids of clients who have disconnected since the last call
	// to getDisconClients();
	vector<unsigned int> getDisconClients();

	
	virtual ~NetworkServer(void);

private:
	unsigned int m_clientCount;
	void initializeSocket();
	void startListening();
	void bindSocket();
	void removeClients(const vector<map<unsigned int, SOCKET>::iterator> &removeList);
	bool sendToClient(const char * const, const int, const unsigned int,  SOCKET &);
	void decodeEvents(const char * head, unsigned int size, map<unsigned int, shared_ptr<Event> > &g, unsigned int client);
	SOCKET m_incomingSock;
	GameState<Entity> m_sendGS;
	map <unsigned int, SOCKET> m_connectedClients;
	map <unsigned int, string> m_clientIDs;
	map <unsigned int, string> m_newClients;
	vector<unsigned int> m_disconClients;

	
	//thread stuff
	CRITICAL_SECTION m_cs;
	CRITICAL_SECTION m_cs1;
	CONDITION_VARIABLE m_workerReady;
	CONDITION_VARIABLE m_broadcastReady;
	bool m_sendAvailable;

	HANDLE m_hThread;
	HANDLE m_hThread1;

	void acceptNewClient();
	void updateEventsBuffer();
	void broadcastGameStateWorker();
	void initializeThreads();
	


	static unsigned __stdcall ThreadStaticEntryPoint(void * pThis) {
		NetworkServer * pthX = (NetworkServer*)pThis;
		pthX->acceptNewClient();	
		return 1;
	}

	static unsigned __stdcall ThreadStaticEntryPoint1(void * pThis) {
		NetworkServer * pthX = (NetworkServer*)pThis;
		pthX->broadcastGameStateWorker();	
		return 1;
	}

};

