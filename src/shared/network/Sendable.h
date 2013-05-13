#pragma once

#include<vector>
#include<assert.h>



//#define ENABLE_DELTA
#define ENABLE_COMPRESSION


#ifdef ENABLE_DELTA

class BitField {

#define BITFIELD_CONTAINER unsigned char
#define MAX_ENTITY_SIZE (1 << sizeof(BITFIELD_CONTAINER)*8)
#define MAX_ENCODED_ENTITY_SIZE (2*MAX_ENTITY_SIZE + sizeof(BITFIED_CONTAINER))

public :
	bool operator[](unsigned int i);
	bool const operator[](unsigned int i) const;
	void setBitAt(unsigned int i, bool value);
	unsigned int size();
	unsigned int sendSize();
	void clear();
	unsigned int encode(char * buff);
	unsigned int decode(char *buff);

private:
	std::vector<char> m_field;
};

#endif

class Sendable
{

public :

	virtual const unsigned int size() const = 0;
	virtual unsigned int encode(char *) = 0;
	virtual unsigned int decode(char *) = 0;

#ifdef ENABLE_DELTA
	static bool isNewObject(char* buff);
	virtual unsigned int encodeDelta(char* buff, char *new_data, unsigned int size);
	virtual unsigned int decodeDelta(char* buff);
	static unsigned int skipDeltaInfo(char * buff);
	char *m_oldState;

protected :
	Sendable();

	//pointer to old non-delta encoded state


	//bit field that represents what bytes have changed (first bit represents new object)
	BitField m_deltaField;

#endif
};

