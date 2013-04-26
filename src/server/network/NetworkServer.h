#pragma once

// Global includes
#include <vector>
#include <map>

// Project includes
#include <shared/network/Network.h>
#include <shared/network/NetworkDecoder.h>
#include <shared/game/Entity.h>
#include <shared/ConfigSettings.h>

#define NETWORKSERVER_MAX_CLIENTS 4
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
	void broadcastGameState(const GameState<Entity> &);

	//returns a vector of connected Client IDs
	vector<unsigned int> getConnectedClientIDs();

	//returns a vector of that have connected since the last call to
	//getNewClientIDs;
	vector<unsigned int> getNewClientIDs();

	
	virtual ~NetworkServer(void);

private:
	unsigned int m_clientCount;
	void initializeSocket();
	void startListening();
	void bindSocket();
	void removeClients(const vector<map<unsigned int, SOCKET>::iterator> &removeList);
	bool sendToClient(const char * const, const int, const unsigned int,  SOCKET &);
	SOCKET m_incomingSock;
	map <unsigned int, SOCKET> m_connectedClients;
	map <unsigned int, unsigned int> m_newClients;
	EventBuff_t m_eventsBuffer;

	
	//thread stuff
	CRITICAL_SECTION m_cs;
	HANDLE m_hThread;
	void acceptNewClient();
	void updateEventsBuffer();
	


	static unsigned __stdcall ThreadStaticEntryPoint(void * pThis) {
		NetworkServer * pthX = (NetworkServer*)pThis;
		pthX->acceptNewClient();	
		return 1;
	}

};

