#pragma once
#include<WinSock2.h>
#include<string>
#include<iostream>
#include<process.h>

#pragma comment(lib,"ws2_32.lib")

#define WIN32_LEAN_AND_MEAN
#define DEFAULT_PORT 5555
#define MAX_PACKET_SIZE 65507
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

