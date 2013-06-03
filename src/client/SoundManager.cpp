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

	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	HRESULT hr;
	if ( FAILED(hr = XAudio2Create( &pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR ) ) ) {
		CoUninitialize();
	} else {
	
		if ( FAILED(hr = pXAudio2->CreateMasteringVoice( &pMasterVoice ) ) ) {
			pXAudio2->Release();
			CoUninitialize();
		} else {
	
			WAVEFORMATEXTENSIBLE wfx = {0};
			XAUDIO2_BUFFER buffer = {0};
			buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

			WAVEFORMATEXTENSIBLE wfxtb = {0};
			XAUDIO2_BUFFER buffertb = {0};
			buffertb.LoopCount = XAUDIO2_LOOP_INFINITE;

			loadSound(_TEXT("engine_m.wav"),wfx, buffer);
			loadSound(_TEXT("tractorbeam_m.wav"),wfxtb, buffertb);
			sounds[THRUSTSOUND] = buffer;
			sounds[TBEAMSOUND] = buffertb;
			formats[THRUSTSOUND] = wfx;
			formats[TBEAMSOUND] = wfxtb;

			WAVEFORMATEXTENSIBLE wfx_engs = {0};
			XAUDIO2_BUFFER buffer_engs = {0};
			buffer_engs.LoopCount = 0;

			loadSound(_TEXT("enginestart_m.wav"),wfx_engs, buffer_engs);
			sounds[ENGINESTARTSOUND] = buffer_engs;
			formats[ENGINESTARTSOUND] = wfx_engs;

			WAVEFORMATEXTENSIBLE wfx_pulse = {0};
			XAUDIO2_BUFFER buffer_pulse = {0};
			loadSound(_TEXT("pulse_m.wav"),wfx_pulse, buffer_pulse);
			sounds[PULSESOUND] = buffer_pulse;
			formats[PULSESOUND] = wfx_pulse;

			//Load ambience (music)
			WAVEFORMATEXTENSIBLE wfx_amb = {0};
			XAUDIO2_BUFFER buffer_amb = {0};
			buffer_amb.LoopCount = XAUDIO2_LOOP_INFINITE;
			loadSound(_TEXT("ambience.wav"),wfx_amb, buffer_amb);
			IXAudio2SourceVoice* temp;
			if( FAILED(hr = pXAudio2->CreateSourceVoice( &temp, (WAVEFORMATEX*)(&wfx_amb) ) ) ) 
				std::cout << "failure 2!" << std::endl;
			if( FAILED(hr = (temp->SubmitSourceBuffer( &buffer_amb ) ) ) )
				std::cout << "failure 3!" << std::endl;
			//temp->Start(0);
			
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
		HRESULT hr;
		IXAudio2SourceVoice* temp;
		tractorBeams.insert(pair<int,IXAudio2SourceVoice*>(beam.m_playerNum,temp));
		if( FAILED(hr = pXAudio2->CreateSourceVoice( &(tractorBeams[beam.m_playerNum]), (WAVEFORMATEX*)(&formats[TBEAMSOUND]) ) ) ) 
			std::cout << "failure 2!" << std::endl;

		if( FAILED(hr = (tractorBeams[beam.m_playerNum])->SubmitSourceBuffer( &sounds[TBEAMSOUND] ) ) )
			std::cout << "failure 3!" << std::endl;

		X3DAUDIO_EMITTER * Emitter = new X3DAUDIO_EMITTER();
		tractorBeams3d.insert(pair<int,X3DAUDIO_EMITTER*>(beam.m_playerNum, Emitter));
		tractorBeams3d[beam.m_playerNum]->ChannelCount = 1;
		tractorBeams3d[beam.m_playerNum]->CurveDistanceScaler = AUDSCALE;
		tractorBeams3d[beam.m_playerNum]->DopplerScaler = 20.0;

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
		HRESULT hr;
		IXAudio2SourceVoice* temp2;
		engines.insert(pair<int,IXAudio2SourceVoice*>(ship.m_playerNum,temp2));
		if( FAILED(hr = pXAudio2->CreateSourceVoice( &(engines[ship.m_playerNum]), (WAVEFORMATEX*)(&formats[THRUSTSOUND]) ) ) ) 
			std::cout << "failure 2!" << std::endl;

		IXAudio2SourceVoice* temp3;
		powerups.insert(pair<int,IXAudio2SourceVoice*>(ship.m_playerNum,temp3));
		if( FAILED(hr = pXAudio2->CreateSourceVoice( &(powerups[ship.m_playerNum]), (WAVEFORMATEX*)(&formats[PULSESOUND]) ) ) ) 
			std::cout << "failure 2!" << std::endl;

		X3DAUDIO_EMITTER * Emitter = new X3DAUDIO_EMITTER();
		engines3d.insert(pair<int,X3DAUDIO_EMITTER*>(ship.m_playerNum, Emitter));
		engines3d[ship.m_playerNum]->ChannelCount = 1;
		engines3d[ship.m_playerNum]->CurveDistanceScaler = AUDSCALE;
		engines3d[ship.m_playerNum]->DopplerScaler = 20.0;

		X3DAUDIO_EMITTER * Emitter2 = new X3DAUDIO_EMITTER();
		powerups3d.insert(pair<int,X3DAUDIO_EMITTER*>(ship.m_playerNum, Emitter2));
		powerups3d[ship.m_playerNum]->ChannelCount = 1;
		powerups3d[ship.m_playerNum]->CurveDistanceScaler = AUDSCALE;
		powerups3d[ship.m_playerNum]->DopplerScaler = 20.0;
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
			std::cout << "failure 3!" << std::endl;
		} else if (vs.BuffersQueued == 1) {
			HRESULT hr;
			if( FAILED(hr = (engines[ship.m_playerNum])->SubmitSourceBuffer( &sounds[THRUSTSOUND] ) ) )
				std::cout << "failure 3!" << std::endl;
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
				std::cout << "failure 3!" << std::endl;
			powerups[ship.m_playerNum]->Start(0);
		}
	}
}

void SoundManager::playEvent(shared_ptr<GEvent> e) {
	}
/*
void SoundManager::newVoice(vector<IXAudio2SourceVoice> * list, SENUM_TYPE type) {

}*/


