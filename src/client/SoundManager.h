//=================================================================================================
// SoundManager.h This class maintains and manages all sound within the game.
//=================================================================================================

#pragma once
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <xaudio2.h>
#include <x3daudio.h> 
#include <shared/game/GameState.h>
#include <map>
#include <list>
#include <client/graphics/entities/C_Ship.h>
#include <client/graphics/entities/C_TractorBeam.h>

#pragma comment(lib,"x3daudio.lib")

#define SENUM_TYPE char
enum Sound : char { THRUSTSOUND, TBEAMSOUND, ENGINESTARTSOUND, PULSESOUND, COLLISIONSOUND, AMBIENCESOUND, MUSICSOUND};

class SoundManager {
public:
	SoundManager();
	~SoundManager();
	void playTractorBeam(C_TractorBeam beam);
	void playEngine(C_Ship ship);
	void playEvent(shared_ptr<GEvent> e);
	void cleanEvents();
	bool isValid;
private:
	IXAudio2MasteringVoice* pMasterVoice;
	IXAudio2SourceVoice* pSourceVoice;
	IXAudio2SourceVoice* pSourceVoicetb;
	IXAudio2* pXAudio2;
	X3DAUDIO_HANDLE X3DInstance;
	X3DAUDIO_LISTENER Listener;
	XAUDIO2_DEVICE_DETAILS deviceDetails;
	map<SENUM_TYPE, XAUDIO2_BUFFER> sounds;
	map<SENUM_TYPE, WAVEFORMATEXTENSIBLE> formats;
	map<int,IXAudio2SourceVoice*> engines;
	map<int,X3DAUDIO_EMITTER*> engines3d;
	map<int,IXAudio2SourceVoice*> powerups;
	map<int,X3DAUDIO_EMITTER*> powerups3d;
	map<int,IXAudio2SourceVoice*> tractorBeams;
	map<int,X3DAUDIO_EMITTER*> tractorBeams3d;
	void newSound(TCHAR * path, Sound type, int loop);
	void newVoice(map<int,IXAudio2SourceVoice*> & map, int idx, Sound type);
	void new3dEmitter(map<int,X3DAUDIO_EMITTER*> & map, int idx);

	list<IXAudio2SourceVoice*> collisions;
};

#endif