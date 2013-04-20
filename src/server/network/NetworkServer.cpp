/*
* NetworkServer.cpp
*/

// Project includes
#include <server/network/NetworkServer.h>


NetworkServer::NetworkServer(void) : 
Network(),
	m_clientCount(0)
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);
	InitializeCriticalSection(&m_cs);
	initializeSocket();
	bindSocket();
	startListening();

	unsigned int threadID;
	m_hThread = (HANDLE)_beginthreadex( NULL, // security
		0,             // stack size
		NetworkServer::ThreadStaticEntryPoint,// entry-point-function
		this,           // arg list holding the "this" pointer
		0,		
		&threadID );
}

NetworkServer::NetworkServer(string ip, unsigned short port) : 
Network(ip, port),
	m_clientCount(0) 
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);
	InitializeCriticalSection(&m_cs);
	initializeSocket();
	bindSocket();
	startListening();

	unsigned int threadID;
	m_hThread = (HANDLE)_beginthreadex( NULL, // security
		0,             // stack size
		NetworkServer::ThreadStaticEntryPoint,// entry-point-function
		this,           // arg list holding the "this" pointer
		0,		
		&threadID );
}

NetworkServer::NetworkServer(unsigned short port) : 
Network(port),
	m_clientCount(0)
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);
	InitializeCriticalSection(&m_cs);	
	initializeSocket();
	bindSocket();
	startListening();

	unsigned int threadID;
	m_hThread = (HANDLE)_beginthreadex( NULL, // security
		0,             // stack size
		NetworkServer::ThreadStaticEntryPoint,// entry-point-function
		this,           // arg list holding the "this" pointer
		0,		
		&threadID );
}

void NetworkServer::startListening() {
	if(listen(m_incomingSock, SOMAXCONN) == SOCKET_ERROR){
		throw runtime_error("Could not start listening socket : " + to_string((long long) WSAGetLastError()));
	}
}

void NetworkServer::initializeSocket() {
	if( (m_incomingSock = socket(AF_INET , SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET )  {
		throw runtime_error("Could not create socket : " + to_string((long long) WSAGetLastError()));
	}
}

void NetworkServer::broadcastGameState(const GameState &state) {
	//accumulate all data into send buffer
	const char* send_buff = state.getSendBuff();

	vector<map<unsigned int, SOCKET>::iterator> removeList;

	//send to every client currently connected
	EnterCriticalSection(&m_cs);
	for(auto it = m_connectedClients.begin();
		it != m_connectedClients.end(); it++) {
			if(!sendToClient(send_buff, state.sendSize(), it->first, it->second)) {
				//client can't be reached
				closesocket(it->second);
				removeList.push_back(it);
			}
	}
	//remove clients who cannot be reached
	for(auto it = removeList.begin(); it != removeList.end(); it++) {
		m_connectedClients.erase(*it);
	}
	LeaveCriticalSection(&m_cs);
	delete []send_buff;
}

bool NetworkServer::sendToClient(const char * const buff, const int size, const unsigned int client,  SOCKET &s) {
	if(send(s, buff, size, 0) == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
		cerr << "failed to send to client " + to_string((long long)client)
			+ ". Error code : " + to_string((long long) WSAGetLastError()) << endl;
		return false;
	}
	return true;
}

EventBuff_t NetworkServer::getEvents() {
	char local_buf[MAX_PACKET_SIZE];
	EventBuff_t rtn;

	bool error = false;
	memset(local_buf,'\0', MAX_PACKET_SIZE);
	int recv_len;

	vector<map<unsigned int,SOCKET>::iterator> removeList;
	EnterCriticalSection(&m_cs);
	for(auto it = m_connectedClients.begin();
		it != m_connectedClients.end(); it++) {

			if ((recv_len = recv(it->second, local_buf, MAX_PACKET_SIZE, 0)) == SOCKET_ERROR
				&& WSAGetLastError() != WSAEWOULDBLOCK) {
					cerr << "recv() from client " << it->first << " failed with error code : " + to_string((long long) WSAGetLastError()) << endl;
					error = true;		
			}
			if(error || recv_len == 0) {
				//cannot reach client, close the connection
				cout << "Connection to client " << it->first << " closed" << endl;
				closesocket(it->second);
				removeList.push_back(it);
			} else if (recv_len > 0) {
				NetworkDecoder nd(local_buf, recv_len);
				nd.decodeEvents(rtn, it->first);
			}
	}
	//remove clients that aren't reachable
	for(auto it = removeList.begin(); it != removeList.end(); it++) {
		m_connectedClients.erase(*it);
	}
	LeaveCriticalSection(&m_cs);

	return rtn;
}

void inline NetworkServer::bindSocket() {
	if(bind(m_incomingSock ,(struct sockaddr *)&(this->m_sockaddr),
		sizeof(m_sockaddr)) == SOCKET_ERROR) {
			throw runtime_error("Failed to bind socket : " + to_string((long long) WSAGetLastError()));
	}
}

void NetworkServer::acceptNewClient()
{
	while(1) {
		bool error = false;
		// if client waiting, accept the connection and save the socket
		SOCKET ClientSocket = accept(m_incomingSock,NULL,NULL);

		if (ClientSocket != INVALID_SOCKET) {			

			if(send(ClientSocket, (const char *) &(m_clientCount), sizeof(m_clientCount), 0) == SOCKET_ERROR){
				cerr << "Error sending client id : " + to_string((long long) WSAGetLastError()) << endl;
				error = true;
			}

			if(!error) {
				//disable nagle on the client's socket
				u_long iMode = 1;
				if(ioctlsocket(ClientSocket, FIONBIO, &iMode) == SOCKET_ERROR){
					throw runtime_error("connect() failed with error code : " + to_string((long long) WSAGetLastError()));
				}

				char value = 1;
				setsockopt( ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof( value ) );

				// insert new client into session id table
				EnterCriticalSection(&m_cs);
				m_connectedClients.insert( pair<unsigned int, SOCKET>(m_clientCount++, ClientSocket) );
				LeaveCriticalSection(&m_cs);
			}
		}
	}
}

vector<unsigned int> NetworkServer::getConnectedClientIDs() {
	vector<unsigned int> rtn;
	EnterCriticalSection(&m_cs);
	for(auto it = m_connectedClients.begin(); it != m_connectedClients.end(); it++){
		rtn.push_back(it->first);
	}
	LeaveCriticalSection(&m_cs);
	return rtn;
}


NetworkServer::~NetworkServer(void) {
	CloseHandle(m_hThread);
	closesocket(m_incomingSock);
	WSACleanup();
}
