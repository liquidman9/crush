#pragma once

// Global includes
#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <string>
#include <iostream>
#include <process.h>
#include <vector>
#include <map>


#define NETWORKSERVER_MAX_CLIENTS 4

// Project includes
#include <shared/game/GameState.h>
#include <shared/game/Entity.h>
#include <shared/GameInput.h>

#pragma comment(lib,"ws2_32.lib")

//typedef vector<shared_ptr<Entity> > GameState;
typedef InputState Event;
typedef map<unsigned int, shared_ptr<Event>> EventBuff_t;

#define DEFAULT_PORT 8889
#define MAX_PACKET_SIZE 65507
#define TIMEOUT 5; //(in seconds)



using namespace std;

class Network
{
public:
	Network(void);
	Network(string ip, unsigned short port);
	Network(unsigned short port);
	virtual ~Network(void);

	const Network& operator=(const Network &rhs) {
		if(this == &rhs) {
			return *this;
		}
		m_sockaddr.sin_family = rhs.m_sockaddr.sin_family;
		m_sockaddr.sin_addr = rhs.m_sockaddr.sin_addr;
		m_sockaddr.sin_port = rhs.m_sockaddr.sin_port;
		return *this;
	}

	bool operator<(const Network &rhs) const {
		if (this->m_sockaddr.sin_addr.s_addr < rhs.m_sockaddr.sin_addr.s_addr){
			return true;
		} else if (this->m_sockaddr.sin_addr.s_addr > rhs.m_sockaddr.sin_addr.s_addr) {
			return false;
		} else {
			return this->m_sockaddr.sin_port < rhs.m_sockaddr.sin_port;
		}
	}

	bool operator==(const Network &rhs) const {
		int this_ip = this->m_sockaddr.sin_addr.s_addr;
		int rhs_ip = rhs.m_sockaddr.sin_addr.s_addr;
		short this_port = this->m_sockaddr.sin_port;
		short rhs_port = rhs.m_sockaddr.sin_port;
		return this_ip == rhs_ip && this_port == rhs_port; 
	}

private:
	Network(struct sockaddr_in);
	struct sockaddr_in& getSockAddr() { return m_sockaddr; };
	struct sockaddr_in m_sockaddr;
	friend class NetworkClient;
	friend class NetworkServer;
};
