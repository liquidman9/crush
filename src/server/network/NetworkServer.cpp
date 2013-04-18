/*
 * NetworkServer.cpp
 */

// Project includes
#include <server/network/NetworkServer.h>


NetworkServer::NetworkServer(void) : Network(), m_eventsAvailable(false) {
	WSAStartup(MAKEWORD(2,2),&wsa);
	InitializeCriticalSection(&m_cs);
	if( (m_sock = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET )  {
		throw runtime_error("Could not create socket : " + to_string((long long) WSAGetLastError()));
		return;
	}
	bindSocket();

	unsigned int threadID;
	m_hThread = (HANDLE)_beginthreadex( NULL, // security
		0,             // stack size
		NetworkServer::ThreadStaticEntryPoint,// entry-point-function
		this,           // arg list holding the "this" pointer
		0,		
		&threadID );
}

NetworkServer::NetworkServer(string ip, unsigned short port) : Network(ip, port), m_eventsAvailable(false) {
	WSAStartup(MAKEWORD(2,2),&wsa);
	InitializeCriticalSection(&m_cs);
	if( (m_sock = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET )  {
		throw runtime_error("Could not create socket : " + to_string((long long) WSAGetLastError()));
		return;
	}
	bindSocket();

	unsigned int threadID;
	m_hThread = (HANDLE)_beginthreadex( NULL, // security
		0,             // stack size
		NetworkServer::ThreadStaticEntryPoint,// entry-point-function
		this,           // arg list holding the "this" pointer
		0,		
		&threadID );
}

NetworkServer::NetworkServer(unsigned short port) : Network(port), m_eventsAvailable(false) {
	WSAStartup(MAKEWORD(2,2),&wsa);
	InitializeCriticalSection(&m_cs);
	if( (m_sock = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET )  {
		throw runtime_error("Could not create socket : " + to_string((long long) WSAGetLastError()));

	}
	bindSocket();

	unsigned int threadID;
	m_hThread = (HANDLE)_beginthreadex( NULL, // security
		0,             // stack size
		NetworkServer::ThreadStaticEntryPoint,// entry-point-function
		this,           // arg list holding the "this" pointer
		0,		
		&threadID );
}

void NetworkServer::broadcastGameState(const GameState &state) {
	map<Network,Network>::iterator start = m_connectedClients.begin();
	map<Network,Network>::iterator end = m_connectedClients.end();
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
	for(map<Network,Network>::iterator it = start; it != end; it++) {
		sendToClient(local_buff, total_size, it->second);
	}
}

void NetworkServer::sendToClient(char * const buff, int size, Network &client) {
	if(sendto(m_sock, buff, size, 0, (sockaddr *) &(client.getSockAddr()), sizeof(sockaddr_in)) == SOCKET_ERROR) {
		throw runtime_error("sendto() failed with error code : " + to_string((long long) WSAGetLastError()));
	}
}

EventBuff_t NetworkServer::getEvents() {

	EnterCriticalSection(&m_cs);

	EventBuff_t rtn = m_eventsBuffer;
	m_eventsAvailable = false;
	m_eventsBuffer.clear();

	LeaveCriticalSection(&m_cs);

	return rtn;
}

void inline NetworkServer::bindSocket() {
	if(bind(m_sock ,(struct sockaddr *)&(this->m_sockaddr),
		sizeof(m_sockaddr)) == SOCKET_ERROR) {
			throw runtime_error("Failed to bind socket : " + to_string((long long) WSAGetLastError()));
	}
}


void NetworkServer::updateEventsBuffer() {
	char local_buf[MAX_PACKET_SIZE];
	struct sockaddr_in recv_addr;
	int recv_size = sizeof(recv_addr);	
	while(1) {
		bool error = false;
		memset(local_buf,'\0', MAX_PACKET_SIZE);
		int recv_len;
		//try {
			if ((recv_len = recvfrom(m_sock, local_buf, MAX_PACKET_SIZE, 0, (sockaddr *) &recv_addr, &recv_size)) == SOCKET_ERROR) {
				cerr << ("recvfrom() failed with error code : " + to_string((long long) WSAGetLastError())) << endl;
				error = true;
				//throw e;
			} 
		//} catch (exception &e){
		//	v e.what() << endl;
		//}
		if(!error) {
			Network lookUpAddr(recv_addr);
			NetworkDecoder nd(local_buf, recv_len);

			EnterCriticalSection(&m_cs);

			nd.decodeEvents(m_eventsBuffer);
			m_connectedClients[Network(recv_addr)] = lookUpAddr;
			m_eventsAvailable = true;

			LeaveCriticalSection(&m_cs);
		}
	}
}

NetworkServer::~NetworkServer(void) {
	CloseHandle(m_hThread);
	closesocket(m_sock);
	WSACleanup();
}
