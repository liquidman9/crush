/*
* NetworkServer.cpp
*/

// Project includes
#include <server/network/NetworkServer.h>


NetworkServer::NetworkServer(void) : 
Network(),
	m_eventsAvailable(false),
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
	m_eventsAvailable(false),
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
	m_eventsAvailable(false),
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
	char local_buff[MAX_PACKET_SIZE] = { 0 };
	memset(m_packetData,'\0', MAX_PACKET_SIZE);
	unsigned int total_size = 0;
	//vector<Entity*> state = g.getEntities();
	for(unsigned int i = 0; i < state.size(); i++) {
		const char* tmp = state[i]->encode();
		memcpy(local_buff + total_size, tmp, state[i]->size());
		total_size += state[i]->size();
		delete tmp;
	}
	//memcpy(m_packetData
	//strncpy(this->m_packetData, local_string.c_str(), MAX_PACKET_SIZE);
	EnterCriticalSection(&m_cs);
	for(map<unsigned int, SOCKET>::iterator it = start; it != end; it++) {
		sendToClient(local_buff, total_size, it->second);
	}
	LeaveCriticalSection(&m_cs);
}

void NetworkServer::sendToClient(char * const buff, int size, SOCKET &s) {
	if(send(s, buff, size, 0) == SOCKET_ERROR) {
		throw runtime_error("send() failed with error code : " + to_string((long long) WSAGetLastError()));
	}
}

EventBuff_t NetworkServer::getEvents() {
	char local_buf[MAX_PACKET_SIZE];
	struct sockaddr_in recv_addr;
	int recv_size = sizeof(recv_addr);

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
			nd.decodeEvents(m_eventsBuffer, it->first);
			m_eventsAvailable = true;
		}
	}

	EventBuff_t rtn = m_eventsBuffer;
	m_eventsAvailable = true;
	m_eventsBuffer.clear();

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
