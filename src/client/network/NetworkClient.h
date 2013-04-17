#pragma once

#include <exception>
#include <vector>
#include <assert.h>
#include <shared/network/Network.h>
#include <shared/game/Entity.h>
#include <shared/network/NetworkDecoder.h>
#include <shared/game/GameState.h>

typedef Entity Event;
typedef GameState State_t;

class NetworkClient :
	public Network
{
public:

	//creates a NetworkClient with default port and any ip address
	//Throws runtime_error exception if socket cannot be opened
	NetworkClient(void);

	//creates NetworkClient with ip and port number passed
	//note: ip should be in the form "127.0.0.1"
	//Throws runtime_error exception if socket cannot be opened
	NetworkClient(string ip, unsigned short port);

	//creates NetworkClient with any ip address and passed port
	//Throws runtime_error exception if socket cannot be opened
	NetworkClient(unsigned short port);

	//takes server ip and port. This function must be called before 
	//using NetworkClient
	int bindToServer(string ip, unsigned short port);

	//get the game state as currently known by the client
	const State_t& getGameState();

	//returns true if there is a new gamestate
	bool newStateAvailable() { return m_stateAvailable; };

	//sentds event to server
	void sendToServer(Event*);


	virtual ~NetworkClient(void);


private:
	SOCKET m_sock;
	WSADATA wsa;
	Network m_server;
	bool m_stateAvailable;
	State_t m_gameState;


	//thread stuff
	void updateGameState();
	CRITICAL_SECTION m_cs;
	HANDLE m_hThread;

	//needed to jump into updateGameState()
	 static unsigned __stdcall ThreadStaticEntryPoint(void * pThis) {
		NetworkClient * pthX = (NetworkClient*)pThis;
		pthX->updateGameState();	
		return 1;
	}
};

