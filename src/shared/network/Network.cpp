
// Global includes
#include <WinSock2.h>

// Project includes
#include <shared/network/Network.h>


Network::Network(void) {
	memset(&m_sockaddr, 0, sizeof(m_sockaddr));
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.s_addr = INADDR_ANY;
	m_sockaddr.sin_port = htons(DEFAULT_PORT);
}

Network::Network(string ip,unsigned short port) {
	memset(&m_sockaddr, 0, sizeof(m_sockaddr));
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	m_sockaddr.sin_port = htons(port);
}

Network::Network(unsigned short port) {
	memset(&m_sockaddr, 0, sizeof(m_sockaddr));
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.s_addr = INADDR_ANY;
	m_sockaddr.sin_port = htons(port);
}

Network::Network(struct sockaddr_in si){
	memset(&m_sockaddr, 0, sizeof(m_sockaddr));
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr = si.sin_addr;
	m_sockaddr.sin_port = si.sin_port;
}



Network::~Network(void) {

}



#ifdef ENABLE_DELTA
char * Network::encodeDelta(const char* new_data, unsigned int &size) {
	assert(new_data != NULL);
	//if(m_oldState == NULL) {
	//	//allocate return buffer
	//	unsigned int buff_size = (unsigned int) ceil((float)size/8) 
	//		+ sizeof(BITFIELD_CONTAINER) + size;
	//	char * buff = new char[buff_size];
	//	m_oldState = new char[size];
	//	memcpy(m_oldState, new_data, size);
	//	auto rtn = m_deltaField.encode(buff);
	//	memcpy(buff + rtn, m_oldState, size);
	//	size += rtn;
	//	return buff;
	//} else {
	//	//m_deltaField.setBitAt(0, false);
	//}

	if(m_oldSize < size) {
		char * tmp = new char[size];
		memcpy(tmp, m_oldState, m_oldSize);
		delete []m_oldState;
		m_oldState = tmp;
	}

	char * np = (char *) new_data;
	char * op = (char *) m_oldState;
	unsigned int diff_size = m_oldSize > size ? size : m_oldSize;
	//populate the delta bitfield
	for(unsigned int i = 0; i < diff_size; ++i) {
		if(np[i] == op[i]) {
			m_deltaField.setBitAt(i, false);
		} else {
			m_deltaField.setBitAt(i, true);
		}
	}
	//resize oldStaet
	if(m_oldSize < size) {
		for(unsigned int i = m_oldSize; i < size; ++i) {
			m_deltaField.setBitAt(i, true);
		}
		m_oldSize = size;
	}
	//allocate return buffer
	unsigned int buff_size = ceil((float)m_deltaField.size()/8) 
		+ sizeof(BITFIELD_CONTAINER) + m_oldSize;
	char *buff = new char[buff_size];

	//encode the delta bitfield
	auto rtn = m_deltaField.encode(buff);
	buff += rtn;

	//encode the rest of the data	
	for(unsigned int i = 0; i+1 < m_deltaField.size(); i++) {
		if(m_deltaField[i+1]) {
			*buff = new_data[i];
			buff++;
			rtn++;
		}
	}

	//update oldState
	memcpy(m_oldState, new_data, size);
	size = rtn;
	return buff;
}

char * Network::decodeDelta(const char *buff, unsigned int &size) {
	m_deltaField.clear();
	auto rtn = m_deltaField.decode(buff);
	auto orig_buff = buff;
	buff += rtn;
	if(m_oldState == NULL) {
		//if(m_oldState) delete []m_oldState;
		//assert(m_deltaField[0]);
		m_oldState = new char[m_deltaField.size()];
		char * rtn_buff = new char[m_deltaField.size()];
		memcpy(rtn_buff, buff, m_deltaField.size());
		memcpy(m_oldState, buff, m_deltaField.size());
		size = m_deltaField.size();
		return rtn_buff;
	} else {
		unsigned int deltaBytesDecoded = 0;
		if(m_deltaField.size() > m_oldSize) {
			char * tmp = new char[m_deltaField.size()];
			memcpy(tmp, m_oldState, m_deltaField.size());
			delete []m_oldState;
			m_oldState = tmp;
			m_oldSize = m_deltaField.size();
		}
		for(unsigned int i = 0; i < m_deltaField.size(); i++) {
			if(m_deltaField[i]) {
				m_oldState[i] = *buff++;
				deltaBytesDecoded++;
			}
		}
		char *rtn_buff = new char[m_deltaField.size()];
		memcpy(rtn_buff, m_oldState, m_deltaField.size());
		size = m_deltaField.size();
		return rtn_buff;
	}
}


void Network::clearDelta() {
	delete []m_oldState;
	m_oldState = NULL;
	m_oldSize = 0;
}

#endif

#ifdef ENABLE_COMPRESSION
char * Network::compress(const char *, unsigned int &size) {

}
char * Network::decompress(const char *, unsigned int &size) { 

}
#endif

unsigned int getSize(const char *);




#ifdef ENABLE_DELTA

bool BitField::operator[](unsigned int i) {
	return m_field[i] > 0;
}

bool const BitField::operator[](unsigned int i) const {
	return m_field[i] > 0;
}

void BitField::setBitAt(unsigned int i, bool user_value) {
	//if this assert fails change BITFIELD_CONTAINER to unsigned short then tell Eric
	char value = user_value ? 1 : 0;
	assert(i < MAX_ENTITY_SIZE);	
	if (i == m_field.size()) {
		m_field.push_back(value);
	} else if (i > m_field.size()) {
		m_field.resize(i, 0);
		m_field[i] = value;
	} else {
		m_field[i] = value;
	}
}

unsigned int BitField::size() {
	return m_field.size();
}

unsigned int BitField::sendSize() {
	return m_field.size() + sizeof(BITFIELD_CONTAINER);
}

void BitField::clear() {
	m_field.clear();
}

unsigned int BitField::encode(char * buff) {
	*(BITFIELD_CONTAINER *) buff = (BITFIELD_CONTAINER) m_field.size();
	buff += sizeof(BITFIELD_CONTAINER);
	unsigned int size = m_field.size()/8 + (m_field.size()%8 ? 1 : 0);
	for(unsigned int i = 0; i < m_field.size(); ++i) {
		for(unsigned int k = 0; k < sizeof(char)*8 && (i*8+k) < m_field.size(); ++k) {
			unsigned int tmp = (i*8+k);
			if (m_field[(i*8+k)]) {
				buff[i] |= (1 << k);
			} else {
				buff[i] &= ~(1 << k);
			}
		}
	}
	return size + sizeof(BITFIELD_CONTAINER);
}

unsigned int BitField::decode(const char *buff) {
	unsigned int rtn =  *(BITFIELD_CONTAINER *) buff;
	buff += sizeof(BITFIELD_CONTAINER);
	unsigned int size = rtn/8 + (rtn%8 ? 1 : 0); 
	for(unsigned int i = 0; i < size; i++) {
		for(unsigned int k = 0; k < sizeof(char)*8; ++k) {
			if(i == size -1 && k == rtn%8) {
				//don't decode bits that aren't part of the
				//delta filed
				break;
			}
			auto tmp =(buff[i] & (1 << k));
			m_field.push_back(((buff[i] & (1 << k)) > 0) ? 1 : 0);
		}
	}
	return size + sizeof(BITFIELD_CONTAINER);
}

#endif