#pragma once
#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include <shared/network/Sendable.h>

class InputState : public Sendable {
	public:
	unsigned char tractBeam;
	unsigned char thrust;
	short turn;
	short pitch;
	bool brake;
	short roll;

	double getThrust() {
		return thrust/255.0;
	}

	double getTurn() {
		return turn/32768.0;
	}

	double getPitch() {
		return pitch/32768.0;
	}

	double getTractorBeam() {
		return tractBeam/255.0;
	}

	bool getBrake() {
		return brake;
	}

	double getRoll() {
		return roll/32768.0;
	}

	static const unsigned int m_size = sizeof(unsigned char)+sizeof(unsigned char)+sizeof(short)*3+sizeof(bool);

	virtual const char* encode() const {
		char * tmp = new char[m_size];
		*(unsigned char *) tmp = tractBeam;
		*(unsigned char *) (tmp+sizeof(unsigned char)) = thrust;
		*(short *) (tmp+sizeof(unsigned char)+sizeof(char)) = turn;
		*(short *) (tmp+sizeof(unsigned char)+sizeof(char)+sizeof(short)) = pitch;
		*(short *) (tmp+sizeof(unsigned char)+sizeof(char)+sizeof(short)+sizeof(short)) = roll;
		*(bool *) (tmp+sizeof(unsigned char)+sizeof(char)+sizeof(short)+sizeof(short)+sizeof(short)) = brake;
		return tmp;
	};

	virtual void decode(const char * tmp) {
		tractBeam = *(unsigned char *)tmp;
		thrust = *(unsigned char *)(tmp+sizeof(unsigned char));
		turn = *(short *) (tmp+sizeof(unsigned char)+sizeof(unsigned char));
		pitch = *(short *) (tmp+sizeof(unsigned char)+sizeof(unsigned char)+sizeof(short));
		roll = *(short *) (tmp+sizeof(unsigned char)+sizeof(unsigned char)+sizeof(short)+sizeof(short));
		brake = *(bool *) (tmp+sizeof(unsigned char)+sizeof(unsigned char)+sizeof(short)+sizeof(short)+sizeof(short));
	};
	virtual const unsigned int size() const { return m_size; };
};

#endif