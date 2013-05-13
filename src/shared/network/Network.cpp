
// Global includes
#include <WinSock2.h>

// Project includes
#include <shared/network/Network.h>


Network::Network(void): m_oldState(NULL), m_oldSize(0) {
	memset(&m_sockaddr, 0, sizeof(m_sockaddr));
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.s_addr = INADDR_ANY;
	m_sockaddr.sin_port = htons(DEFAULT_PORT);
}

Network::Network(string ip,unsigned short port): m_oldState(NULL), m_oldSize(0) {
	memset(&m_sockaddr, 0, sizeof(m_sockaddr));
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	m_sockaddr.sin_port = htons(port);
}

Network::Network(unsigned short port): m_oldState(NULL), m_oldSize(0) {
	memset(&m_sockaddr, 0, sizeof(m_sockaddr));
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.s_addr = INADDR_ANY;
	m_sockaddr.sin_port = htons(port);
}

Network::Network(struct sockaddr_in si): m_oldState(NULL), m_oldSize(0) {
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
	unsigned int buff_size = (unsigned int) ceil((float)m_deltaField.size()/8) 
		+ sizeof(BITFIELD_CONTAINER) + m_oldSize + sizeof(unsigned int);
	char *rtn_buff = new char[buff_size];
	//add space for overall size
	char *buff = rtn_buff + sizeof(unsigned int);

	//encode the delta bitfield
	auto rtn = m_deltaField.encode(buff);
	buff += rtn;

	//encode the rest of the data	
	for(unsigned int i = 0; i < m_deltaField.size(); i++) {
		if(m_deltaField[i]) {
			*buff = new_data[i];
			buff++;
			rtn++;
		}
	}

	//update oldState
	memcpy(m_oldState, new_data, size);
	size = rtn + sizeof(unsigned int);
	*(unsigned int*) rtn_buff = size;
	return rtn_buff;
}

char * Network::decodeDelta(const char *buff, unsigned int &size) {
	m_deltaField.clear();
	//skip over header size
	buff += sizeof(unsigned int);
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
char * Network::compress(const char *head, unsigned int &size) {
	static bool init = false;
		if(!init){
			lzo_init();
			init = true;
		}
		char *out_tmp = new char[2*size];
		unsigned char scratch [LZO1X_1_MEM_COMPRESS];
		unsigned char* in = (unsigned char *) head;
		unsigned char* out = (unsigned char*) (out_tmp + sizeofHeader());
		lzo_uint in_len = size;
		lzo_uint out_len;
		lzo1x_1_compress(in,in_len,out,&out_len,scratch);
		setHeader(out_tmp,out_len,in_len);
		size = getSize(out_tmp);
		return out_tmp;
}

char * Network::decompress(const char *head, unsigned int &size) { 
	static bool init = false;
	if(!init) {
		lzo_init();
		init = true;
	}
	unsigned int d_size;
	unsigned int c_len;
	unsigned long d_len;
	loadHeader(head, c_len, d_size);
	head += sizeofHeader();
	unsigned char* d_out = new unsigned char[d_size];
	//skip over stored compressed_size
	auto r = lzo1x_decompress((unsigned char*)head,c_len,d_out,&d_len,NULL);
	if(r != LZO_E_OK) {
		cerr << "OH GOD WE GUNNA CRASH (decompress failed)" << endl;
	}
	size = d_len;
	return (char *) d_out;
}

void Network::loadHeader(const char* buff, unsigned int &c_len, unsigned int &d_size) {
	c_len = *(unsigned int*)buff -  (2*sizeof(unsigned int));
	buff += sizeof(unsigned int);
	d_size = *(unsigned int*)buff;
	buff += sizeof(unsigned int);
}

void Network::setHeader(char * buff, unsigned int c_size, unsigned int d_size){
	*(unsigned int*)buff = c_size + 2*sizeof(d_size);
	buff += sizeof(unsigned int);
	*(unsigned int*)buff = d_size;
	buff += sizeof(unsigned int);	
}

unsigned int Network::sizeofHeader() {
	return 2*sizeof(unsigned int);
}
#endif

unsigned int Network::getSize(const char *head) {
	return *(unsigned int*) head;
}

const char * Network::encodeSendBuff(const char *head, unsigned int in_size, unsigned int &out_size) {

#ifdef ENABLE_DELTA
	auto delta_buff = encodeDelta(head, in_size);
	head = delta_buff;
#endif

#ifdef ENABLE_COMPRESSION
	auto compression_buff = compress(head, in_size);
	head = compression_buff;
#endif

#if defined(ENABLE_DELTA) && defined(ENABLE_COMPRESSION )
	delete []delta_buff;
#endif
	
#if !defined(ENABLE_COMPRESSION) && !defined(ENABLE_DELTA)
	char * tmp = new char[in_size];
	memcpy(tmp, head, in_size);
	head = tmp;
#endif
	out_size = in_size;
	return head;
}

const char * Network::decodeSendBuff(const char *head, unsigned int in_size, unsigned int &out_size) {


#ifdef ENABLE_COMPRESSION
	auto compression_buff = decompress(head, in_size);
	head = compression_buff;
#endif

#ifdef ENABLE_DELTA
	auto delta_buff = decodeDelta(head, in_size);
	head = delta_buff;
#endif

#if defined(ENABLE_DELTA) && defined(ENABLE_COMPRESSION )
	delete []compression_buff;
#endif
	
#if !defined(ENABLE_COMPRESSION) && !defined(ENABLE_DELTA)
	char * tmp = new char[in_size];
	memcpy(tmp, head, in_size);
	head = tmp;
#endif

	out_size = in_size;
	return head;
}


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
	//auto tmp  = MAX_ENTITY_SIZE;
	//assert(i < MAX_ENTITY_SIZE);
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
	for(unsigned int i = 0; i < size; ++i) {
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