//#include"Sendable.h"
//
//#ifdef ENABLE_DELTA
//
//
//
//Sendable::Sendable(): m_oldState(NULL)
//{}
//
//unsigned int Sendable::encodeDelta(char* buff, char *new_data, unsigned int size) {
//	assert(new_data != NULL);
//	if(m_oldState == NULL) {
//		//object is new
//		//set new object
//		m_deltaField.setBitAt(0, true);
//		//set old state
//		m_oldState = new char[size];
//		memcpy(m_oldState, new_data, size);
//		auto rtn = m_deltaField.encode(buff);
//		memcpy(buff + rtn, m_oldState, size);
//		return size + rtn;
//	} else {
//		m_deltaField.setBitAt(0, false);
//	}
//	char * np = (char *) new_data;
//	char * op = (char *) m_oldState;
//	//populate the delta bitfield
//	for(unsigned int i = 0; i < size; ++i) {
//		if(np[i] == op[i]) {
//			m_deltaField.setBitAt(i+1, false);
//		} else {
//			m_deltaField.setBitAt(i+1, true);
//		}
//	}
//	//encode the delta bitfield
//	auto rtn = m_deltaField.encode(buff);
//	buff += rtn;
//
//	//encode the rest of the data	
//	for(unsigned int i = 0; i+1 < m_deltaField.size(); i++) {
//		if(m_deltaField[i+1]) {
//			*buff = new_data[i];
//			buff++;
//			rtn++;
//		}
//	}
//
//	//update oldState
//	memcpy(m_oldState, new_data, size);
//	return rtn;
//}
//
//bool Sendable::isNewObject(char *buff) {
//	buff += sizeof(BITFIELD_CONTAINER);
//	return (*buff & 1) == 1;
//}
//
//unsigned int Sendable::decodeDelta(char* buff) {
//	m_deltaField.clear();
//	auto rtn = m_deltaField.decode(buff);
//	auto orig_buff = buff;
//	buff += rtn;
//	if(m_oldState == NULL || m_deltaField[0]) {
//		if(m_oldState) delete []m_oldState;
//		assert(m_deltaField[0]);
//		m_oldState = new char[this->size()];
//		memcpy(m_oldState, buff, this->size());
//		return rtn + this->size();
//	} else {
//		unsigned int deltaBytesDecoded = 0;
//		for(unsigned int i = 0; i+1 < m_deltaField.size(); i++) {
//			if(m_deltaField[i+1]) {
//				m_oldState[i] = *buff++;
//				deltaBytesDecoded++;
//			}
//		}
//		return rtn + deltaBytesDecoded;
//	}
//}
//
//unsigned int Sendable::skipDeltaInfo(char *buff) {
//	return  (unsigned int)(ceil(((float)*(BITFIELD_CONTAINER *) buff)/8)) + sizeof(BITFIELD_CONTAINER); 
//}
//
//
//#endif