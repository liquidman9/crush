#pragma once
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <xaudio2.h>

class SoundManager {
public:
	SoundManager();
	void play();
	~SoundManager();
private:
	IXAudio2MasteringVoice* pMasterVoice;
	IXAudio2SourceVoice* pSourceVoice;
	IXAudio2* pXAudio2;

};

#endif