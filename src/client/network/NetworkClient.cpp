#include <client/network/NetworkClient.h>

NetworkClient::NetworkClient(void):Network(), m_stateAvailable(false) {

	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
		throw runtime_error("WSAStartup failed : " + to_string((long long) WSAGetLastError()));
	}
	InitializeCriticalSection(&m_cs);
	if( (m_sock = socket(AF_INET , SOCK_DGRAM, 0 )) == INVALID_SOCKET )  {
		throw runtime_error("Could not create socket : " + to_string((long long) WSAGetLastError()));
	}
}

NetworkClient::NetworkClient(string ip, unsigned short port): Network(ip, port), m_stateAvailable(false) {

	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
		throw runtime_error("WSAStartup failed : " + to_string((long long) WSAGetLastError()));
	}
	InitializeCriticalSection(&m_cs);
	if( (m_sock = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP )) == INVALID_SOCKET )  {
		throw runtime_error("Could not create socket : " + to_string((long long) WSAGetLastError()));
	}
}

NetworkClient::NetworkClient(unsigned short port): Network(port), m_stateAvailable(false) {
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
		throw runtime_error("WSAStartup failed : " + to_string((long long) WSAGetLastError()));
	}
	InitializeCriticalSection(&m_cs);
	if( (m_sock = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP)) == INVALID_SOCKET )  {
		throw runtime_error("Could not create socket : " + to_string((long long) WSAGetLastError()));
	}
}

int NetworkClient::bindToServer(string ip, unsigned short port) {

	m_server = Network(ip, port);

	//bind server port to socket
	if(bind(m_sock ,(struct sockaddr *)&(this->m_sockaddr),
		sizeof(m_sockaddr)) == SOCKET_ERROR) {
			return WSAGetLastError();
	}

	//start thread to recv data from server
	unsigned int threadID;
	m_hThread = (HANDLE)_beginthreadex( NULL, // security
		0,             // stack size
		NetworkClient::ThreadStaticEntryPoint,// entry-point-function
		this,           // arg list holding the "this" pointer
		0,		
		&threadID );
	return 0;
}

void NetworkClient::sendToServer(Event* e) {
	//char local_buf[MAX_PACKET_SIZE];
	//strcpy(local_buf, e.c_str());
	//struct sockaddr_in server;
	//server.sin_family = AF_INET;
	//server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//server.sin_port = htons( 8888 );
	//m_server = Network("127.0.0.1", 8888);*/
	const char * encoded = e->encode();
	if(sendto(m_sock, encoded, e->size(), 0, (sockaddr *) &m_server.getSockAddr(), sizeof(sockaddr_in)) == SOCKET_ERROR) {
		throw runtime_error("sendto() failed with error code : " + to_string((long long) WSAGetLastError()));
	}
	delete encoded;
}

void NetworkClient::updateGameState() {
	char local_buf[MAX_PACKET_SIZE];
		while(1) {
		bool error = false;
		memset(local_buf,'\0', MAX_PACKET_SIZE);
		int recv_len;
		try {
			if ((recv_len = recv(m_sock, local_buf, MAX_PACKET_SIZE, 0) == SOCKET_ERROR)) {
				throw runtime_error("recvfrom() failed with error code : " + to_string((long long) WSAGetLastError()));
			}
		} catch (exception e) {
			cerr << e.what() << endl;
			error = true;
		}
		if(!error){
			NetworkDecoder nd(local_buf, recv_len);
			EnterCriticalSection(&m_cs);
			nd.decodeGameState(m_gameState);
			//m_gameState.push_back(n.decode(local_buf));
			m_stateAvailable = true;
			//m_gameState.push_back(string(local_buf);
			LeaveCriticalSection(&m_cs);
		}
	}
}


State_t NetworkClient::getGameState() {
	EnterCriticalSection(&m_cs);
	State_t rtn = m_gameState;
	m_gameState.clear();
	m_stateAvailable = false;
	LeaveCriticalSection(&m_cs);
	return rtn;
}

NetworkClient::~NetworkClient(void) {
	CloseHandle(m_hThread);
	closesocket(m_sock);	
	WSACleanup();
}
