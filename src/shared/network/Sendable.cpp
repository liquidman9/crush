#include"Sendable.h"

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

unsigned int BitField::decode(char *buff) {
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

Sendable::Sendable(): m_oldState(NULL)
{}

unsigned int Sendable::encodeDelta(char* buff, char *new_data, unsigned int size) {
		assert(new_data != NULL);
		if(m_oldState == NULL) {
			//object is new
			//set new object
			m_deltaField.setBitAt(0, true);
			//set old state
			m_oldState = new char[size];
			memcpy(m_oldState, new_data, size);
			auto rtn = m_deltaField.encode(buff);
			memcpy(buff + rtn, m_oldState, size);
			return size + rtn;
		} else {
			m_deltaField.setBitAt(0, false);
		}
		char * np = (char *) new_data;
		char * op = (char *) m_oldState;
		//populate the delta bitfield
		for(unsigned int i = 0; i < size; ++i) {
			if(np[i] == op[i]) {
				m_deltaField.setBitAt(i+1, false);
			} else {
				m_deltaField.setBitAt(i+1, true);
			}
		}
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
		return rtn;
	}

bool Sendable::isNewObject(char *buff) {
	buff += sizeof(BITFIELD_CONTAINER);
	return (*buff & 1) == 1;
}

unsigned int Sendable::decodeDelta(char* buff) {
	m_deltaField.clear();
	auto rtn = m_deltaField.decode(buff);
	auto orig_buff = buff;
	buff += rtn;
	if(m_oldState == NULL || m_deltaField[0]) {
		if(m_oldState) delete []m_oldState;
		assert(m_deltaField[0]);
		m_oldState = new char[this->size()];
		memcpy(m_oldState, buff, this->size());
		return rtn + this->size();
	} else {
		unsigned int deltaBytesDecoded = 0;
		for(unsigned int i = 0; i+1 < m_deltaField.size(); i++) {
			if(m_deltaField[i+1]) {
				m_oldState[i] = *buff++;
				deltaBytesDecoded++;
			}
		}
		return rtn + deltaBytesDecoded;
	}
}

unsigned int Sendable::skipDeltaInfo(char *buff) {
	return  *(BITFIELD_CONTAINER *) buff + sizeof(BITFIELD_CONTAINER); 
}


#endif