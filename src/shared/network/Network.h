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
#define ENABLE_DELTA
#define ENABLE_COMPRESSION

#ifdef ENABLE_COMPRESSION
#include<shared/network/minilzo.h>
#endif


using namespace std;


#ifdef ENABLE_DELTA
class BitField {
#define BITFIELD_CONTAINER unsigned int
//#define MAX_ENTITY_SIZE (1 << sizeof(BITFIELD_CONTAINER)*8)
//#define MAX_ENCODED_ENTITY_SIZE (2*MAX_ENTITY_SIZE + sizeof(BITFIED_CONTAINER))
public :
	bool operator[](unsigned int i);
	bool const operator[](unsigned int i) const;
	void setBitAt(unsigned int i, bool value);
	unsigned int size();
	unsigned int sendSize();
	void clear();
	unsigned int encode(char * buff);
	unsigned int decode(const char *buff);

private:
	std::vector<char> m_field;
};

#endif


class Network
{
public:
	Network(void);
	Network(string ip, unsigned short port);
	Network(unsigned short port);
	virtual ~Network(void);

#ifdef ENABLE_DELTA
	BitField m_deltaField;
	char * m_oldState;
	unsigned int m_oldSize;
	char * encodeDelta(const char* new_data, unsigned int &size);
	char * decodeDelta(const char *, unsigned int &size);
	void clearDelta();
#endif

#ifdef ENABLE_COMPRESSION
	//compresses input char array of passed size, returns a new buffer and updates size
	char * compress(const char *, unsigned int &size);
	//decompresses input char array of passed size and returns a new pointer with the new value of size
	char * decompress(const char *, unsigned int &size);
	void setHeader(char * buff, unsigned int c_size, unsigned int d_size);
	void loadHeader(const char* buff, unsigned int &c_len, unsigned int &d_size);
	unsigned int sizeofHeader();
#endif
	//encodes buffer of in_size and returns new buffer with out_size
	const char * encodeSendBuff(const char *haed, unsigned int in_size, unsigned int &out_size);
	//decodes buffer of out_size and returns new buffer with out_size
	const char * decodeSendBuff(const char *head, unsigned int in_size, unsigned int &out_size);
	unsigned int getSize(const char *);




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
