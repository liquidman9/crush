//=================================================================================================
// InputState.h Sendable state of player input class that contains abstracted possible game inputs.
//=================================================================================================

#pragma once
#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include <shared/network/Sendable.h>

//Declare the types of each input
typedef unsigned char TBType, ThrustType;
typedef short TurnType, PitchType;
typedef bool BrakeType, ReType, PushType, PowerupType, MashType;

class InputState : public Sendable {
	public:
	TBType tractBeam;
	ThrustType thrust;
	TurnType turn;
	PitchType pitch;
	BrakeType brake;
	ReType reorient;
	PushType push;
	PowerupType powerup;
	MashType mash;

	//Returns the current thrust value, normalized from 0 to 1
	double getThrust() {
		return thrust/255.0;
	}

	//Returns the current turning value, normalized from 0 to 1
	double getTurn() {
		return turn/32768.0;
	}

	//Returns the current pitch value, normalized from 0 to 1
	double getPitch() {
		return pitch/32768.0;
	}

	//Returns the current tractor beam value, normalized from 0 to 1
	double getTractorBeam() {
		return tractBeam/255.0;
	}

	//Returns the whether or not the brake is on
	bool getBrake() {
		return brake;
	}

	//Returns true if the player is pushing the reorient button
	bool getReorient() {
		return reorient;
	}

	//Returns true if the player is using the burst reverse tractor beam
	bool getPushBurst() {
		return push;
	}

	// Returns true if the player is pressing the use powerup button
	bool getPowerup() {
		return powerup;
	}

	// Return true if the player presses mash button
	bool getMash() {
		return mash;
	}
	//The size of the server aware input state data
	static const unsigned int m_size = sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+
		sizeof(BrakeType)+sizeof(ReType)+sizeof(PushType) +sizeof(PowerupType)+sizeof(MashType);

	virtual unsigned int encode(char *tmp) const {
		*(TBType *) tmp = tractBeam;
		*(ThrustType *) (tmp+sizeof(TBType)) = thrust;
		*(TurnType *) (tmp+sizeof(TBType)+sizeof(ThrustType)) = turn;
		*(PitchType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)) = pitch;
		*(BrakeType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)) = brake;
		*(ReType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+sizeof(BrakeType)) = reorient;
		*(PushType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+sizeof(BrakeType)+sizeof(ReType)) = push;
		*(PowerupType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+sizeof(BrakeType)+sizeof(ReType)+sizeof(PushType)) = powerup;
		*(MashType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+sizeof(BrakeType)+sizeof(ReType)+sizeof(PushType)+sizeof(MashType)) = mash;
		return sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+sizeof(BrakeType)+sizeof(ReType)+sizeof(PushType)+sizeof(PowerupType)+sizeof(MashType);
	};

	virtual unsigned int decode(const char * tmp) {
		tractBeam = *(TBType *)tmp;
		thrust = *(ThrustType *)(tmp+sizeof(TBType));
		turn = *(TurnType *) (tmp+sizeof(TBType)+sizeof(ThrustType));
		pitch = *(PitchType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType));
		brake = *(BrakeType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType));
		reorient = *(ReType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+sizeof(BrakeType));
		push = *(PushType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+sizeof(BrakeType)+sizeof(ReType));
		powerup = *(PowerupType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+sizeof(BrakeType)+sizeof(ReType)+sizeof(PowerupType));
		mash = *(MashType *) (tmp+sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+sizeof(BrakeType)+sizeof(ReType)+sizeof(PowerupType)+sizeof(MashType));
		return sizeof(TBType)+sizeof(ThrustType)+sizeof(TurnType)+sizeof(PitchType)+sizeof(BrakeType)+sizeof(ReType)+sizeof(PushType)+sizeof(PowerupType)+sizeof(MashType);
	};

	//Returns the size of all server aware input state data when encoded
	virtual const unsigned int size() const { return m_size; };
};

#endif