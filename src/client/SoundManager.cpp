#include "SoundManager.h"

#include <iostream>
#include "SoundLoader.h"

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

			loadSound(wfx, buffer);
	
	
	
			if( FAILED(hr = pXAudio2->CreateSourceVoice( &pSourceVoice, (WAVEFORMATEX*)(&wfx) ) ) ) 
				std::cout << "failure 2!" << std::endl;

			if( FAILED(hr = pSourceVoice->SubmitSourceBuffer( &buffer ) ) )
				std::cout << "failure 3!" << std::endl;
			

			if ( FAILED(hr = pSourceVoice->Start( 0 ) ) )
				std::cout << "failure 4!" << std::endl;
			if( FAILED(hr = pSourceVoice->SubmitSourceBuffer( &buffer ) ) )
				std::cout << "failure 3!" << std::endl;
			pSourceVoice->Start(0);
		}
	}
}

SoundManager::~SoundManager() {
	pXAudio2->Release();
	CoUninitialize();
}


