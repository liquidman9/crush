#pragma once

#include <vector>
#include <map>

#include <shared/network/Network.h>
#include <shared/network/NetworkDecoder.h>
#include <shared/game/Entity.h>

typedef Sendable Event;
//typedef string Entity;
typedef vector<Event *> EventBuff_t;
typedef GameState State_t;

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

	//returns true if there are new events available
	bool eventsAvailable() { return m_eventsAvailable; };

	//returns a vector of events received
	EventBuff_t getEvents();

	void broadcastGameState(const State_t &);

	


	virtual ~NetworkServer(void);
private:

	void sendToClient(char * const buff, int size, Network &client);
	WSADATA wsa;
	SOCKET m_sock;
	map <Network, Network> m_connectedClients;
	EventBuff_t m_eventsBuffer;
	void bindSocket();
	bool m_eventsAvailable;
	char m_packetData[MAX_PACKET_SIZE];
	
	//thread stuff
	CRITICAL_SECTION m_cs;
	HANDLE m_hThread;
	void updateEventsBuffer();

	static unsigned __stdcall ThreadStaticEntryPoint(void * pThis) {
		NetworkServer * pthX = (NetworkServer*)pThis;
		pthX->updateEventsBuffer();	
		return 1;
	}

};

