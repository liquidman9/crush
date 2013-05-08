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
	int timeout = TIMEOUT;
	ConfigSettings::config->getValue("network_timeout", timeout);
	if(setsockopt( m_sock, SOL_SOCKET, SO_RCVTIMEO, (const char *) &timeout, sizeof( timeout )) ) {
		runtime_error e("Could not set timeout : " + to_string((long long) WSAGetLastError()));
		throw e;
	}
}

void NetworkClient::bindToServer(string ip, unsigned short port, const string &client_name) {
	bindToServer(Network(ip, port), client_name);
}

void NetworkClient::bindToServer(string ip, unsigned short port) {
	bindToServer(Network(ip, port), "");
}

void NetworkClient::bindToServer(Network const &n, const string &client_name) {
	m_server = n;

	//bind server port to socket
	/*if(bind(m_sock ,(struct sockaddr *)&(this->m_sockaddr),
	sizeof(m_sockaddr)) == SOCKET_ERROR) {
	throw runtime_error("bind() failed with error code : " + to_string((long long) WSAGetLastError()));
	}*/


	//connect to server
	if(connect( m_sock, (struct sockaddr *) &(m_server.getSockAddr()), sizeof(m_server.getSockAddr())) == SOCKET_ERROR ){
		cerr << "connect() failed with error code : " + to_string((long long) WSAGetLastError()) << endl;

		runtime_error e("connect() failed with error code : " + to_string((long long) WSAGetLastError()));
		throw e;
	}

	//get client ID
	int clientID;
	if (recv (m_sock, (char *) &clientID, sizeof(m_clientID), 0) == SOCKET_ERROR) {
		cerr << "Failed to get client ID : " + to_string((long long) WSAGetLastError()) << endl;

		runtime_error e("Failed to get client ID : " + to_string((long long) WSAGetLastError()));
		throw e;
	}

	if(clientID < 0) {
		cerr << "Connection to server rejected : Server full" << endl;
		runtime_error e("Connection to server rejected: Server full.");
		throw e;
	}
	m_clientID = clientID;

	//send player name
	string send_name = client_name;
	if(send_name == "") {
		send_name = "Player " + to_string((long long) clientID);
	}
	send_name.resize(MAX_PLAYERNAME_SIZE);

	if (send (m_sock, send_name.c_str(), MAX_PLAYERNAME_SIZE, 0) == SOCKET_ERROR) {
		cerr << "Failed to send player name : " + to_string((long long) WSAGetLastError()) << endl;

		runtime_error e("Failed to send player name : " + to_string((long long) WSAGetLastError()));
		throw e;
	}

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
		runtime_error e("sendto() failed with error code : " + to_string((long long) WSAGetLastError()));
		throw e;
	}
	delete []encoded;
}

void NetworkClient::updateGameState() {
	char buff[MAX_PACKET_SIZE];
	char *local_buf;
	int remaining_data = 0;
	GameState<Entity> local_gs;
	while(1) {
		local_buf = buff + remaining_data;
		bool error = false;
		//memset(local_buf,'\0', MAX_PACKET_SIZE);
		int recv_len;
		if ((recv_len = recv(m_sock, local_buf, MAX_PACKET_SIZE, 0)) == SOCKET_ERROR) {
			cerr << "recvfrom() failed with error code : " + to_string((long long) WSAGetLastError()) << endl;
			error = true;
		}
		auto test = m_gameState.getRecvSize(local_buf);

		if(error && WSAGetLastError() == WSAETIMEDOUT) {
			cerr << "conntection to the server timedout" << endl;
		}

		int total_size = recv_len;
		while(!error && recv_len < m_gameState.gsMinSize()) {
			if ((recv_len = recv(m_sock, local_buf+total_size, MAX_PACKET_SIZE, 0)) == SOCKET_ERROR) {
				cerr << "recvfrom() failed with error code : " + to_string((long long) WSAGetLastError()) << endl;
				error = true;
				break;
			}
			total_size += recv_len;
		}
		int expected_size;

		if(!error) {
			expected_size = m_gameState.getExpectedSize(local_buf, total_size);
		}
		
		while(!error && total_size < expected_size) {
			if ((recv_len = recv(m_sock, local_buf+total_size, MAX_PACKET_SIZE, 0)) == SOCKET_ERROR) {
				cerr << "recvfrom() failed with error code : " + to_string((long long) WSAGetLastError()) << endl;
				error = true;
				break;
			}
			total_size += recv_len;
			expected_size = m_gameState.getExpectedSize(local_buf, total_size);
		}

		if(!error) {
			remaining_data = local_gs.decode(local_buf, total_size);
			EnterCriticalSection(&m_cs);
			if(!(m_stateAvailable && m_gameState.size() == 0)) {
				m_gameState = local_gs;
			}			
			m_stateAvailable = true;
			LeaveCriticalSection(&m_cs);
			assert(remaining_data >= 0);
		}
	}
}


const GameState<Entity> NetworkClient::getGameState() {
	EnterCriticalSection(&m_cs);
	GameState<Entity> rtn= GameState<Entity>(m_gameState);
	m_gameState.clear();
	m_stateAvailable = false;
	LeaveCriticalSection(&m_cs);
	return rtn;
}

NetworkClient::~NetworkClient(void) {
	TerminateThread(m_hThread, 0);
	CloseHandle(m_hThread);
	closesocket(m_sock);	
	WSACleanup();
}
