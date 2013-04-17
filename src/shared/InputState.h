#pragma once
#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include <shared/network/Sendable.h>

class InputState : public Sendable {
	public:
	bool tractBeam;
	unsigned char thrust;
	short turn;
	short pitch;

	static const unsigned int m_size = sizeof(bool)+sizeof(char)+sizeof(short)*2;

	virtual const char* encode() const {
		char * tmp = new char[m_size];
		*(bool *) tmp = tractBeam;
		*(char *) (tmp+sizeof(bool)) = thrust;
		*(short *) (tmp+sizeof(bool)+sizeof(char)) = turn;
		*(short *) (tmp+sizeof(bool)+sizeof(char)+sizeof(short)) = pitch;
		return tmp;
	};

	virtual void decode(const char * tmp) {
		tractBeam = *(bool *)tmp;
		thrust = *(char *)(tmp+sizeof(bool));
		turn = *(short *) (tmp+sizeof(bool)+sizeof(char));
		pitch = *(short *) (tmp+sizeof(bool)+sizeof(char)+sizeof(short));
	};
	virtual const unsigned int size() const { return m_size; };
};

#endif