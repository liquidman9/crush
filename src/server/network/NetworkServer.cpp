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
	initializeSocket();
	bindSocket();
	startListening();
	initializeThreads();
}

NetworkServer::NetworkServer(string ip, unsigned short port) : 
Network(ip, port),
	m_clientCount(0) 
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);
	initializeSocket();
	bindSocket();
	startListening();
	initializeThreads();
}

NetworkServer::NetworkServer(unsigned short port) : 
Network(port),
	m_clientCount(0)
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);

	initializeSocket();
	bindSocket();
	startListening();
	initializeThreads();
}

void NetworkServer::initializeThreads() {
	m_sendAvailable = false;
	InitializeCriticalSection(&m_cs);
	InitializeCriticalSection(&m_cs1);
	InitializeConditionVariable(&m_workerReady);
	InitializeConditionVariable(&m_broadcastReady);

	unsigned int threadID = 0;
	m_hThread = (HANDLE)_beginthreadex( NULL, // security
		0,             // stack size
		NetworkServer::ThreadStaticEntryPoint,// entry-point-function
		this,           // arg list holding the "this" pointer
		0,		
		&threadID );

	threadID = 1;
	m_hThread = (HANDLE)_beginthreadex( NULL, // security
		0,             // stack size
		NetworkServer::ThreadStaticEntryPoint1,// entry-point-function
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

void NetworkServer::broadcastGameStateWorker() {
	vector<map<unsigned int, SOCKET>::iterator> removeList;
	unsigned int curr_clients = 0;
	bool send_empty = false;
	GameState<Entity> e;
	for(;;){		
		//send to every client currently connected
		EnterCriticalSection(&m_cs1);		
		while(!m_sendAvailable) {
			SleepConditionVariableCS(&m_broadcastReady, &m_cs1, INFINITE);
		}
		m_sendAvailable = false;
#ifdef ENABLE_DELTA
		if(curr_clients != m_connectedClients.size()) {
			curr_clients = m_connectedClients.size();
			m_sendGS.resetDeltas();
			send_empty = true;
		}
#endif
		//get send buff and size
		auto send_buff = m_sendGS.getSendBuff();
#ifdef ENABLE_COMPRESSION
		unsigned int size = *(unsigned int*) send_buff;
#else
		unsigned int size = m_sendGS.sendSize();
#endif
		LeaveCriticalSection(&m_cs1);
		WakeConditionVariable(&m_workerReady);

		//prep empty gamestate send if necessary
#ifdef ENABLE_DELTA
		char * send_buff_e;
		if(send_buff)
			send_buff_e = e.getSendBuff();
#ifdef ENABLE_COMPRESSION 
		unsigned int size_e = *(unsigned int*) send_buff_e;
#else
		unsigned int size_e = e.sendSize();
#endif
#endif

		EnterCriticalSection(&m_cs);
		for(auto it = m_connectedClients.begin();
			it != m_connectedClients.end(); it++) {
#ifdef ENABLE_DELTA
				if(send_empty) {
					if(!sendToClient(send_buff_e, size_e, it->first, it->second)) {
						//client can't be reached
						removeList.push_back(it);
					}
				}
#endif
				if(!sendToClient(send_buff, size, it->first, it->second)) {
					//client can't be reached
					removeList.push_back(it);
				}
		}
		//remove clients who cannot be reached
		removeClients(removeList);
		LeaveCriticalSection(&m_cs);
		removeList.clear();
		
#ifdef ENABLE_DELTA
		if(send_buff)
			delete []send_buff_e;
		send_empty = false;
#endif

		delete []send_buff;		
	}
}

void NetworkServer::broadcastGameState(GameState<Entity> const &state) {
	if(m_clientCount == 0) {
		return;
	}

	//copy gameState to WorkerGameState producer/consumer style
	EnterCriticalSection(&m_cs1);
	while(m_sendAvailable) {
		SleepConditionVariableCS(&m_workerReady, &m_cs1, INFINITE);
	}
	m_sendGS = state;
	m_sendAvailable = true;
	WakeConditionVariable(&m_broadcastReady);
	LeaveCriticalSection(&m_cs1);
}

bool NetworkServer::sendToClient(const char * const buff, const int size, const unsigned int client,  SOCKET &s) {
	int send_len;
	if((send_len = send(s, buff, size, 0) == SOCKET_ERROR) && WSAGetLastError() != WSAEWOULDBLOCK) {
		cerr << "failed to send to client " + to_string((long long)client)
			+ ". Error code : " + to_string((long long) WSAGetLastError()) << endl;
		return false;
	}
	/*if(send_len != size) {
	cerr << "Error send_len: "<<  send_len << "expected send size: " << size <<  endl;
	}*/
	//assert(send_len == size);
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
				removeList.push_back(it);
			} else if (recv_len > 0) {
				/*NetworkDecoder nd(local_buf, recv_len);
				nd.decodeEvents(rtn, it->first);*/
				decodeEvents(local_buf, recv_len, rtn, it->first);
			}
	}
	//remove clients that aren't reachable
	removeClients(removeList);
	LeaveCriticalSection(&m_cs);

	return rtn;
}

void NetworkServer::removeClients(const vector<map<unsigned int, SOCKET>::iterator> &removeList){
	for(auto it = removeList.begin(); it != removeList.end(); it++) {
		cout << "Connection to client " << (*it)->first << " closed" << endl;
		if(closesocket((*it)->second) == SOCKET_ERROR){
			cerr << "closesocket() for client " << (*it)->first << " failed with error code : " + to_string((long long) WSAGetLastError()) << endl;
		}
		auto nci = m_newClients.find((*it)->first);
		if(nci != m_newClients.end()) {
			m_newClients.erase(nci);
		}
		auto cid = m_clientIDs.find((*it)->first);
		if(cid != m_clientIDs.end()) {
			m_clientIDs.erase(cid);
		}
		m_clientCount--;
		m_connectedClients.erase(*it);
	}
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
		// if client waiting, accept the connection and save the socket
		SOCKET ClientSocket = accept(m_incomingSock,NULL,NULL);

		if (ClientSocket != INVALID_SOCKET) {

			//get max clients from config
			int max_clients_i = 0;
			ConfigSettings::config->getValue("network_maxClients", max_clients_i);
			unsigned int max_clients;
			if(max_clients_i <= 0) {
				max_clients = NETWORKSERVER_MAX_CLIENTS;
			} else {
				max_clients = max_clients_i;
			}

			unsigned int i = max_clients;
			unsigned int tmp_client_count = m_clientCount;
			// find clientID for new client
			if (m_clientCount < max_clients - 1) {
				for (i = 0; i < max_clients; i++) {
					if(m_connectedClients.find(i) == m_connectedClients.end()) {							
						tmp_client_count++;
						break;
					}
				}
			}

			//server full;
			if(i == max_clients) {
				cout << "client connection rejected" << endl;
				closesocket(ClientSocket);
			} else {

				//send clientID
				int clientID = tmp_client_count - 1;
				if(send(ClientSocket, (const char *) &(clientID), sizeof(m_clientCount), 0) == SOCKET_ERROR){
					//LOOK HERE IF THERE ARE PROBLEMS CONNECTING
					cerr << "Error sending client id : " + to_string((long long) WSAGetLastError()) << endl;
					closesocket(ClientSocket);
					continue;
				}

				char client_name[MAX_PLAYERNAME_SIZE];
				if(recv(ClientSocket, client_name, MAX_PLAYERNAME_SIZE,0) == SOCKET_ERROR) {
					cerr << "Error recving player name from client " << clientID << " error :" << WSAGetLastError() << endl;
					cerr << "Closing connection to client " << clientID << endl;
					closesocket(ClientSocket);
					continue;
				}

				//set client to non-blocking
				u_long iMode = 1;
				if(ioctlsocket(ClientSocket, FIONBIO, &iMode) == SOCKET_ERROR) {
					runtime_error e("Error setting client socket to non-blocking : "
						+ to_string((long long) WSAGetLastError()));
					throw e;
				}

				//disable nagle on the client's socket
				char value = 1;
				setsockopt( ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof( value ) );

				//insert new client into connected clients
				EnterCriticalSection(&m_cs);
				EnterCriticalSection(&m_cs1);
				m_clientCount = tmp_client_count;
				m_newClients.insert(pair<unsigned int, string>(i,string(client_name)));
				m_connectedClients.insert(pair<unsigned int, SOCKET> (i, ClientSocket));
				m_clientIDs.insert(pair<unsigned int, string>(i, string(client_name)));
				LeaveCriticalSection(&m_cs1);
				LeaveCriticalSection(&m_cs);
				cout << "New client " << i << " connected." << endl;
			}
		}
	}
}

void NetworkServer::decodeEvents(char * head, unsigned int size, map<unsigned int, shared_ptr<Event> > &g, unsigned int client) {
	Event* ep = NULL;
	char* curr_head = head;
	for(unsigned int cur_size = 0; cur_size < size; cur_size += ep->size() ) {
		ep = new InputState();
		ep->decode(curr_head);
		g[client] = shared_ptr<Event>(ep);
		break;
	}	
}


vector<pair<unsigned int,string>> NetworkServer::getConnectedClientIDs() {
	vector<pair<unsigned int,string>> rtn;
	EnterCriticalSection(&m_cs);
	for(auto it = m_clientIDs.begin(); it != m_clientIDs.end(); it++){
		rtn.push_back(*it);
	}
	LeaveCriticalSection(&m_cs);
	return rtn;
}

vector<pair<unsigned int, string>> NetworkServer::getNewClientIDs(){
	vector<pair<unsigned int, string>> rtn;
	EnterCriticalSection(&m_cs);
	for(auto it = m_newClients.begin(); it != m_newClients.end(); it++){
		rtn.push_back(*it);
	}
	m_newClients.clear();
	LeaveCriticalSection(&m_cs);
	return rtn;
}

vector<unsigned int> NetworkServer::getDisconClients() {
	vector<unsigned int> rtn;
	EnterCriticalSection(&m_cs);
	rtn = m_disconClients;
	m_disconClients.clear();
	LeaveCriticalSection(&m_cs);
	return rtn;
}


NetworkServer::~NetworkServer(void) {
	TerminateThread(m_hThread, 0);
	TerminateThread(m_hThread1, 0);
	CloseHandle(m_hThread);
	CloseHandle(m_hThread1);
	for(auto it = m_connectedClients.begin(); it != m_connectedClients.end(); it++){
		closesocket(it->second);
	}
	closesocket(m_incomingSock);
	WSACleanup();
}
