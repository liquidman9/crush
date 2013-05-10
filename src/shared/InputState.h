#pragma once
#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include <shared/network/Sendable.h>

typedef unsigned char TBType, ThrustType;
typedef short TurnType, PitchType;
typedef bool BrakeType, ReType, PushType;

class InputState : public Sendable {
	public:
	TBType tractBeam;
	ThrustType thrust;
	TurnType turn;
	PitchType pitch;
	BrakeType brake;
	ReType reorient;
	PushType push;


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

	bool getReorient() {
		return reorient;
	}

	bool getPushBurst() {
		return push;
	}

	static const unsigned int m_size = sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+
		sizeof(BrakeType)+sizeof(ReType)+sizeof(PushType);

	virtual unsigned int encode(char * tmp) const {
		//char * tmp = new char[m_size];
		*(TBType *) tmp = tractBeam;
		*(ThrustType *) (tmp+sizeof(TBType)) = thrust;
		*(TurnType *) (tmp+sizeof(TBType)+sizeof(ThrustType)) = turn;
		*(PitchType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)) = pitch;
		*(BrakeType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)) = brake;
		*(ReType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+sizeof(BrakeType)) = reorient;
		*(PushType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+sizeof(BrakeType)+sizeof(ReType)) = push;
		return size();
	};

	virtual unsigned int decode(const char * tmp) {
		tractBeam = *(TBType *)tmp;
		thrust = *(ThrustType *)(tmp+sizeof(TBType));
		turn = *(TurnType *) (tmp+sizeof(TBType)+sizeof(ThrustType));
		pitch = *(PitchType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType));
		brake = *(BrakeType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType));
		reorient = *(ReType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+sizeof(BrakeType));
		push = *(PushType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+sizeof(BrakeType)+sizeof(ReType));
		return size();
	};
	virtual const unsigned int size() const { return m_size; };
};

#endif