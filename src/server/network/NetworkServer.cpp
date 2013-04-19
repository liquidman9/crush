/*
* NetworkServer.cpp
*/

// Project includes
#include <server/network/NetworkServer.h>


NetworkServer::NetworkServer(void) : 
Network(),
	m_clientCount(0)
{
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

void NetworkServer::startListening(){
	if(listen(m_incomingSock, SOMAXCONN) == SOCKET_ERROR){
		throw runtime_error("Could not create socket : " + to_string((long long) WSAGetLastError()));
	}
}

void NetworkServer::initializeSocket() {
	if( (m_incomingSock = socket(AF_INET , SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET )  {
		throw runtime_error("Could not create socket : " + to_string((long long) WSAGetLastError()));
	}
}

void NetworkServer::broadcastGameState(const GameState &state) {
	map<unsigned int, SOCKET>::iterator start = m_connectedClients.begin();
	map<unsigned int,SOCKET>::iterator end = m_connectedClients.end();

	//accumulate all data into send buffer
	const char* send_buff = state.getSendBuff();
	
	//send to every client currently connected
	EnterCriticalSection(&m_cs);
	for(map<unsigned int, SOCKET>::iterator it = start; it != end; it++) {
		sendToClient(send_buff, state.sendSize(), it->first, it->second);
	}
	LeaveCriticalSection(&m_cs);
	delete []send_buff;
}

void NetworkServer::sendToClient(const char * const buff, const int size, const unsigned int client,  SOCKET &s) {
	if(send(s, buff, size, 0) == SOCKET_ERROR) {
		throw runtime_error("failed to send to client " + to_string((long long)client)
			+ ". Error code : " + to_string((long long) WSAGetLastError()));
	}
}

EventBuff_t NetworkServer::getEvents() {
	char local_buf[MAX_PACKET_SIZE];
	EventBuff_t rtn;

	bool error = false;
	memset(local_buf,'\0', MAX_PACKET_SIZE);
	int recv_len;

	map<unsigned int, SOCKET>::iterator start = m_connectedClients.begin();
	map<unsigned int,SOCKET>::iterator end = m_connectedClients.end();

	for(map<unsigned int,SOCKET>::iterator it = start; it != end; it++){
		if ((recv_len = recv(it->second, local_buf, MAX_PACKET_SIZE, 0)) == SOCKET_ERROR) {
			cerr << ("recvfrom() failed with error code : " + to_string((long long) WSAGetLastError())) << endl;
			error = true;		
		}

		if(!error && recv_len > 0) {
			NetworkDecoder nd(local_buf, recv_len);
			nd.decodeEvents(rtn, it->first);
		}
	}

	//m_eventsBuffer.clear();

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
	while(1){
		// if client waiting, accept the connection and save the socket
		SOCKET ClientSocket = accept(m_incomingSock,NULL,NULL);

		if (ClientSocket != INVALID_SOCKET) 
		{
			bool error = false;
			if(send(ClientSocket, (const char *) &(m_clientCount), sizeof(m_clientCount), 0) == SOCKET_ERROR){
				cerr << "Error sending client id : " + to_string((long long) WSAGetLastError()) << endl;
				error = true;
			}

			if(!error) {
				//disable nagle on the client's socket
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


NetworkServer::~NetworkServer(void) {
	CloseHandle(m_hThread);
	closesocket(m_incomingSock);
	WSACleanup();
}
