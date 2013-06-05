//=================================================================================================
// SoundManager.cpp This class maintains and manages all sound within the game.
//=================================================================================================

#include "SoundManager.h"

#include <iostream>
#include "SoundLoader.h"
#include <client/graphics/entities/C_Ship.h>
#include <client/GameResources.h>

const float AUDSCALE = 10.0;
SoundManager::SoundManager() {

	isValid = true;

	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	HRESULT hr;
	if ( FAILED(hr = XAudio2Create( &pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR ) ) ) {
		CoUninitialize();
		isValid = false;
	} else {
	
		if ( FAILED(hr = pXAudio2->CreateMasteringVoice( &pMasterVoice ) ) ) {
			pXAudio2->Release();
			CoUninitialize();
			isValid = false;
		} else {
			//Load all sound effects
			newSound(_TEXT("engine_m.wav"),THRUSTSOUND,XAUDIO2_LOOP_INFINITE);
			newSound(_TEXT("tractorbeam_m.wav"),TBEAMSOUND,XAUDIO2_LOOP_INFINITE);
			newSound(_TEXT("enginestart_m.wav"),ENGINESTARTSOUND,0);
			newSound(_TEXT("pulse_m.wav"),PULSESOUND,0);
			newSound(_TEXT("impact1.wav"),COLLISIONSOUND,0);

			//Load ambience
			newSound(_TEXT("ambience.wav"),AMBIENCESOUND,XAUDIO2_LOOP_INFINITE);
			IXAudio2SourceVoice* temp;
			if( FAILED(hr = pXAudio2->CreateSourceVoice( &temp, (WAVEFORMATEX*)(&formats[AMBIENCESOUND]) ) ) ) 
				isValid = false;
			if( FAILED(hr = (temp->SubmitSourceBuffer( &sounds[AMBIENCESOUND] ) ) ) )
				isValid = false;
			temp->Start(0);

			//Load music
			newSound(_TEXT("music.wav"),MUSICSOUND,XAUDIO2_LOOP_INFINITE);
			IXAudio2SourceVoice* temp2;
			if( FAILED(hr = pXAudio2->CreateSourceVoice( &temp2, (WAVEFORMATEX*)(&formats[MUSICSOUND]) ) ) ) 
				isValid = false;
			if( FAILED(hr = (temp2->SubmitSourceBuffer( &sounds[MUSICSOUND] ) ) ) )
				isValid = false;
			temp2->Start(0);
			temp2->SetVolume(0.1f);
			
			//Set up 3D Sound
			pXAudio2->GetDeviceDetails(0,&deviceDetails);
			X3DAudioInitialize( deviceDetails.OutputFormat.dwChannelMask, X3DAUDIO_SPEED_OF_SOUND/100.0f, X3DInstance );
		}
	}
}

SoundManager::~SoundManager() {
	pXAudio2->Release();
	CoUninitialize();
}

void SoundManager::playTractorBeam(C_TractorBeam beam) {
	if (tractorBeams.find(beam.m_playerNum) == tractorBeams.end()) {

		newVoice(tractorBeams,beam.m_playerNum,TBEAMSOUND);
		new3dEmitter(tractorBeams3d,beam.m_playerNum);

		HRESULT hr;
		if( FAILED(hr = (tractorBeams[beam.m_playerNum])->SubmitSourceBuffer( &sounds[TBEAMSOUND] ) ) )
			isValid = false;
	}

	if (beam.m_isOn && beam.m_playerNum != GameResources::playerNum) { //no 3d for player sounds
		X3DAUDIO_VECTOR aud;
		aud.x=0; aud.y=0; aud.z=1;
		tractorBeams3d[beam.m_playerNum]->OrientFront = aud;
		aud.x=0; aud.y=1; aud.z=0;
		tractorBeams3d[beam.m_playerNum]->OrientTop = aud;
		tractorBeams3d[beam.m_playerNum]->Position = beam.m_start;
		tractorBeams3d[beam.m_playerNum]->Velocity = beam.m_velocity;

		X3DAUDIO_DSP_SETTINGS DSPSettings = {0};
		FLOAT32 * matrix = new FLOAT32[deviceDetails.OutputFormat.Format.nChannels];
		DSPSettings.SrcChannelCount = 1;
		DSPSettings.DstChannelCount = deviceDetails.OutputFormat.Format.nChannels;
		DSPSettings.pMatrixCoefficients = matrix;

		X3DAudioCalculate(X3DInstance, &Listener, tractorBeams3d[beam.m_playerNum],
	X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB,
	&DSPSettings );

		tractorBeams[beam.m_playerNum]->SetOutputMatrix( pMasterVoice, 1, deviceDetails.OutputFormat.Format.nChannels, DSPSettings.pMatrixCoefficients ) ;
		tractorBeams[beam.m_playerNum]->SetFrequencyRatio(DSPSettings.DopplerFactor);
		//tractorBeams[beam.m_playerNum]->SetOutputMatrix(pMasterVoice, 1, 1, &DSPSettings.ReverbLevel);

		//XAUDIO2_FILTER_PARAMETERS FilterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * DSPSettings.LPFDirectCoefficient), 1.0f };
		//tractorBeams[beam.m_playerNum]->SetFilterParameters(&FilterParameters);
	}
	if(beam.m_isOn) {
		tractorBeams[beam.m_playerNum]->Start(0);
	} else {
		tractorBeams[beam.m_playerNum]->Stop();
	}
}

void SoundManager::playEngine(C_Ship ship) {
	if (engines.find(ship.m_playerNum) == engines.end()) {

		newVoice(engines,ship.m_playerNum,THRUSTSOUND);
		newVoice(powerups,ship.m_playerNum,PULSESOUND);

		new3dEmitter(engines3d,ship.m_playerNum);
		new3dEmitter(powerups3d,ship.m_playerNum);

	}

	if (ship.m_playerNum == GameResources::playerNum) {
		X3DAUDIO_VECTOR aud;
		aud.x=0; aud.y=0; aud.z=1;
		Listener.OrientFront = aud;
		aud.x=0; aud.y=1; aud.z=0;
		Listener.OrientTop = aud;
		Listener.Position = ship.m_pos;
		Listener.Velocity = ship.m_velocity;
	} else if (ship.m_thruster != 0) { //dont do 3d if self
		X3DAUDIO_VECTOR aud;
		aud.x=0; aud.y=0; aud.z=1;
		engines3d[ship.m_playerNum]->OrientFront = aud;
		aud.x=0; aud.y=1; aud.z=0;
		engines3d[ship.m_playerNum]->OrientTop = aud;
		engines3d[ship.m_playerNum]->Position = ship.m_pos;
		engines3d[ship.m_playerNum]->Velocity = ship.m_velocity;

		aud.x=0; aud.y=0; aud.z=1;
		powerups3d[ship.m_playerNum]->OrientFront = aud;
		aud.x=0; aud.y=1; aud.z=0;
		powerups3d[ship.m_playerNum]->OrientTop = aud;
		powerups3d[ship.m_playerNum]->Position = ship.m_pos;
		powerups3d[ship.m_playerNum]->Velocity = ship.m_velocity;

		X3DAUDIO_DSP_SETTINGS DSPSettings = {0};
		FLOAT32 * matrix = new FLOAT32[deviceDetails.OutputFormat.Format.nChannels];
		DSPSettings.SrcChannelCount = 1;
		DSPSettings.DstChannelCount = deviceDetails.OutputFormat.Format.nChannels;
		DSPSettings.pMatrixCoefficients = matrix;

		X3DAudioCalculate(X3DInstance, &Listener, engines3d[ship.m_playerNum],
	X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB,
	&DSPSettings );

		engines[ship.m_playerNum]->SetOutputMatrix( pMasterVoice, 1, deviceDetails.OutputFormat.Format.nChannels, DSPSettings.pMatrixCoefficients ) ;
		engines[ship.m_playerNum]->SetFrequencyRatio(DSPSettings.DopplerFactor);

		X3DAudioCalculate(X3DInstance, &Listener, powerups3d[ship.m_playerNum],
	X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB,
	&DSPSettings );

		powerups[ship.m_playerNum]->SetOutputMatrix( pMasterVoice, 1, deviceDetails.OutputFormat.Format.nChannels, DSPSettings.pMatrixCoefficients ) ;
		powerups[ship.m_playerNum]->SetFrequencyRatio(DSPSettings.DopplerFactor);
		//engines[ship.m_playerNum]->SetOutputMatrix(pMasterVoice, 1, 1, &DSPSettings.ReverbLevel);

		/*XAUDIO2_FILTER_PARAMETERS FilterParameters;
		FilterParameters.Type = LowPassFilter;
		FilterParameters.OneOverQ= 2.0f * sinf(X3DAUDIO_PI/6.0f * DSPSettings.LPFDirectCoefficient);
		FilterParameters.Frequency = 1.0f;*/
		//engines[ship.m_playerNum]->SetFilterParameters(NULL);
	}

	if (ship.m_thruster == 0) {
		engines[ship.m_playerNum]->Stop();
		engines[ship.m_playerNum]->FlushSourceBuffers();
	}else {
		XAUDIO2_VOICE_STATE vs;
		(engines[ship.m_playerNum])->GetState(&vs);
		if (vs.BuffersQueued == 0) {
		HRESULT hr;
		if( FAILED(hr = (engines[ship.m_playerNum])->SubmitSourceBuffer( &sounds[ENGINESTARTSOUND] ) ) )
			isValid = false;
		} else if (vs.BuffersQueued == 1) {
			HRESULT hr;
			if( FAILED(hr = (engines[ship.m_playerNum])->SubmitSourceBuffer( &sounds[THRUSTSOUND] ) ) )
				isValid = false;
		}

		engines[ship.m_playerNum]->SetVolume((float)ship.m_thruster);
		engines[ship.m_playerNum]->Start(0);
	}
	if (ship.m_hasPowerup && ship.m_powerupType == SPEEDUP && ship.m_powerupStateType == CONSUMED) {
		engines[ship.m_playerNum]->SetVolume(2.0f);
	}
	if (ship.m_hasPowerup && ship.m_powerupType == PULSE && ship.m_powerupStateType == CONSUMED) {
		XAUDIO2_VOICE_STATE vs;
		(powerups[ship.m_playerNum])->GetState(&vs);
		if (vs.BuffersQueued == 0) {
			HRESULT hr;
			if( FAILED(hr = (powerups[ship.m_playerNum])->SubmitSourceBuffer( &sounds[PULSESOUND] ) ) )
				isValid = false;
			powerups[ship.m_playerNum]->Start(0);
		}
	}
}

void SoundManager::playEvent(shared_ptr<GEvent> e) {

	if (e->m_type == COLLISIONEVENT) {
	
		
	CollisionGEvent * c = dynamic_cast<CollisionGEvent*>(&*e);


	if(c->m_ctype == AM || c->m_ctype == AE || c->m_ctype == AA || c->m_ctype == MS || c->m_ctype == SE || c->m_ctype == SS || c->m_ctype == SA) {
		HRESULT hr;
		IXAudio2SourceVoice* temp;
		if( FAILED(hr = pXAudio2->CreateSourceVoice( &(temp), (WAVEFORMATEX*)(&formats[COLLISIONSOUND]) ) ) ) 
			isValid = false;

		if( FAILED(hr = (temp)->SubmitSourceBuffer( &sounds[COLLISIONSOUND] ) ) )
			isValid = false;
		/*
		X3DAUDIO_EMITTER * Emitter = new X3DAUDIO_EMITTER();
		tractorBeams3d.insert(pair<int,X3DAUDIO_EMITTER*>(beam.m_playerNum, Emitter));
		tractorBeams3d[beam.m_playerNum]->ChannelCount = 1;
		tractorBeams3d[beam.m_playerNum]->CurveDistanceScaler = AUDSCALE;
		tractorBeams3d[beam.m_playerNum]->DopplerScaler = 20.0;
		*/
		float impulse = c->m_impulse;
		temp->SetVolume(c->m_impulse/200000.0);
		temp->SetFrequencyRatio(1/(c->m_impulse/200000.0));
		temp->Start(0);
	
	}

	// Picked up a resource // maybe sound?
	if(c->m_ctype == SR){
		// m_idA it is the entity id not the player id
	}

	// Picked up a Powerup
	if(c->m_ctype == SP) {
		// m_idA it is the entity id not the player id
	}

	/*
	switch(c->m_ctype) {
	case 
	}*/
	}
}

void SoundManager::newSound(TCHAR * path, Sound type, int loop) {
	WAVEFORMATEXTENSIBLE wfx_tmp = {0};
	XAUDIO2_BUFFER buffer_tmp  = {0};
	buffer_tmp.LoopCount = loop;
	loadSound(path,wfx_tmp , buffer_tmp );
	sounds[type] = buffer_tmp;
	formats[type] = wfx_tmp;
}

void SoundManager::newVoice(map<int,IXAudio2SourceVoice*> & map, int idx, Sound type) {
	HRESULT hr;
	IXAudio2SourceVoice* temp2;
	map.insert(pair<int,IXAudio2SourceVoice*>(idx,temp2));
	if( FAILED(hr = pXAudio2->CreateSourceVoice( &(map[idx]), (WAVEFORMATEX*)(&formats[type]) ) ) ) 
		isValid = false;
}

void SoundManager::new3dEmitter(map<int,X3DAUDIO_EMITTER*> & map, int idx) {
	X3DAUDIO_EMITTER * Emitter = new X3DAUDIO_EMITTER();
	map.insert(pair<int,X3DAUDIO_EMITTER*>(idx, Emitter));
	map[idx]->ChannelCount = 1;
	map[idx]->CurveDistanceScaler = AUDSCALE;
	map[idx]->DopplerScaler = 20.0;
}


