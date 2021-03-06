#pragma once

#include <exception>
#include <vector>
#include <assert.h>
#include <shared/network/Network.h>
#include <client/GameResources.h>
#include <shared/ConfigSettings.h>
//#include <shared/game/GameState.h>


//typedef GameState State_t;

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
	void bindToServer(string ip, unsigned short port);

	//takes server ip and port and string. This function must be called before 
	//using NetworkClient. Client name will be the name of this client's ship.
	void bindToServer(string ip, unsigned short port, const string &client_name);

	//get the game state as currently known by the client
	const GameState<Entity> getGameState();

	//returns true if there is a new gamestate
	bool newStateAvailable() { return m_stateAvailable; };

	//sentds event to server
	void sendToServer(Event*);

	//returns client ID
	unsigned int getClientID() { return m_clientID; };
	

	virtual ~NetworkClient(void);


private:
	unsigned int m_clientID;
	void initializeSocket();
	int recvFromServer(char * local_buf, unsigned int size, unsigned int remaining_data);
	SOCKET m_sock;
	WSADATA wsa;
	Network m_server;
	bool m_stateAvailable;
	GameState<Entity> m_gameState;
	void bindToServer(Network const &, const string &);
	int m_timeOut;
	char * m_recvBuff;


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

