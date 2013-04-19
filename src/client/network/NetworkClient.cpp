#include <client/network/NetworkClient.h>

NetworkClient::NetworkClient(void):Network(), m_stateAvailable(false) {

	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
		throw runtime_error("WSAStartup failed : " + to_string((long long) WSAGetLastError()));
	}
	InitializeCriticalSection(&m_cs);
	initializeSocket();
}

NetworkClient::NetworkClient(string ip, unsigned short port): Network(ip, port), m_stateAvailable(false) {

	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
		throw runtime_error("WSAStartup failed : " + to_string((long long) WSAGetLastError()));
	}
	InitializeCriticalSection(&m_cs);
	initializeSocket();
}

NetworkClient::NetworkClient(unsigned short port): Network(port), m_stateAvailable(false) {
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
		throw runtime_error("WSAStartup failed : " + to_string((long long) WSAGetLastError()));
	}
	InitializeCriticalSection(&m_cs);
	initializeSocket();
}


void NetworkClient::initializeSocket() {
	if( (m_sock = socket(AF_INET , SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET )  {
		throw runtime_error("Could not create socket : " + to_string((long long) WSAGetLastError()));
	}
}

void NetworkClient::bindToServer(string ip, unsigned short port) {

	m_server = Network(ip, port);

	//bind server port to socket
	if(bind(m_sock ,(struct sockaddr *)&(this->m_sockaddr),
		sizeof(m_sockaddr)) == SOCKET_ERROR) {
			throw runtime_error("bind() failed with error code : " + to_string((long long) WSAGetLastError()));
	}

	
	//connect to server
	if(connect( m_sock, (struct sockaddr *) &(m_server.getSockAddr()), sizeof(m_server.getSockAddr())) == SOCKET_ERROR ){
		cerr << "connect() failed with error code : " + to_string((long long) WSAGetLastError()) << endl;
		
		runtime_error e("connect() failed with error code : " + to_string((long long) WSAGetLastError()));
		throw e;
	}

	if (recv (m_sock, (char *) &m_clientID, sizeof(m_clientID), 0) == SOCKET_ERROR) {
		cerr << "connect() failed with error code : " + to_string((long long) WSAGetLastError()) << endl;
		
		runtime_error e("Failed to get client ID : " + to_string((long long) WSAGetLastError()));
		throw e;
	}


	//set non_blocking
	/*u_long iMode = 1;
	if(ioctlsocket(m_sock, FIONBIO, &iMode) == SOCKET_ERROR){
		throw runtime_error("connect() failed with error code : " + to_string((long long) WSAGetLastError()));
	}*/

	//don't collect data and send in a big packet
	char value = 1;
    setsockopt( m_sock, IPPROTO_TCP, TCP_NODELAY, &value, sizeof( value ) );


	//start thread to recv data from server
	unsigned int threadID;
	m_hThread = (HANDLE)_beginthreadex( NULL, // security
		0,             // stack size
		NetworkClient::ThreadStaticEntryPoint,// entry-point-function
		this,           // arg list holding the "this" pointer
		0,		
		&threadID );
}

void NetworkClient::sendToServer(Event* e) {
	const char * encoded = e->encode();
	int s = e->size();
	if(send(m_sock, encoded,s, 0) == SOCKET_ERROR) {
		throw runtime_error("sendto() failed with error code : " + to_string((long long) WSAGetLastError()));
	}
	delete []encoded;
}

void NetworkClient::updateGameState() {
	char local_buf[MAX_PACKET_SIZE];
	while(1) {
		bool error = false;
		memset(local_buf,'\0', MAX_PACKET_SIZE);
		int recv_len;
		if ((recv_len = recv(m_sock, local_buf, MAX_PACKET_SIZE, 0)) == SOCKET_ERROR) {
				cerr << "recvfrom() failed with error code : " + to_string((long long) WSAGetLastError()) << endl;
				error = true;
		}
		
		if(!error) {
			NetworkDecoder nd(local_buf, recv_len);
			EnterCriticalSection(&m_cs);
			nd.decodeGameState(m_gameState);
			m_stateAvailable = true;
			LeaveCriticalSection(&m_cs);
		}
	}
}


const GameState NetworkClient::getGameState() {
	EnterCriticalSection(&m_cs);
	GameState rtn= GameState(m_gameState);
	m_gameState.clear();
	m_stateAvailable = false;
	LeaveCriticalSection(&m_cs);
	return rtn;
}

NetworkClient::~NetworkClient(void) {
	CloseHandle(m_hThread);
	if(shutdown(m_sock, SD_SEND) == SOCKET_ERROR){
		cerr << "shutdown() failed with error code : " + to_string((long long) WSAGetLastError()) << endl;
	}
	closesocket(m_sock);	
	WSACleanup();
}
