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
typedef bool BrakeType, ReType, PushType, PowerupType, MashType, StartType;

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
	StartType start;
	TurnType strafe;
	BrakeType reverse;


	//Returns the current thrust value, normalized from 0 to 1
	double getThrust() {
		return thrust/255.0;
	}

	//Returns the current turning value, normalized from 0 to 1
	double getTurn() {
		return turn/32768.0;
	}

	//Returns the current strafing value, normalized from -1 to 1
	double getStrafe() { 
		return strafe/32768.0;
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

	//Returns the whether or not the reverse is on
	bool getReverse() {
		return reverse;
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

	// Returns true if player hits start
	bool getStart() {
		return start;
	}

	
private: 
	struct sendStruct {
		TBType tractBeam;
		ThrustType thrust;
		TurnType turn;
		PitchType pitch;
		BrakeType brake;
		ReType reorient;
		PushType push;
		PowerupType powerup;
		MashType mash;
		StartType start;
		TurnType strafe;
		BrakeType reverse;
	};

public:
	//The size of the server aware input state data
	static const unsigned int m_size = sizeof(sendStruct);

	virtual unsigned int encode(char *tmp) const {
		sendStruct s;
		s.tractBeam = tractBeam;
		s.turn = turn;
		s.thrust = thrust;
		s.pitch = pitch;
		s.brake = brake;
		s.reorient = reorient;
		s.push = push;
		s.powerup = powerup;
		s.mash = mash;
		s.start = start;
		s.strafe = strafe;
		s.reverse = reverse;
		memcpy(tmp, (char*)&s, sizeof(s));
		return sizeof(s);
	};

	virtual unsigned int decode(const char * tmp) {
		sendStruct s;
		memcpy((char*)&s,tmp,sizeof(s));

		tractBeam = s.tractBeam;
		turn = s.turn;
		thrust = s.thrust;
		pitch = s.pitch;
		brake = s.brake;
		reorient = s.reorient;
		push = s.push;
		powerup = s.powerup;
		mash = s.mash;
		start = s.start;
		strafe = s.strafe;
		reverse = s.reverse;
		return sizeof(s);
	};

	//Returns the size of all server aware input state data when encoded
	virtual const unsigned int size() const { return m_size; };
};

#endif