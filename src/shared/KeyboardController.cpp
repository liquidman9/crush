#include "KeyboardController.h"
#include <iostream>
KeyboardController::KeyboardController(InputState * input) : Controller(input)
{ 
	HRESULT result;
	result = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if(FAILED(result))
	{
		exit(0);
	}
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if(FAILED(result))
	{
		exit(0);
	}

	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(result))
	{
		exit(0);
	}
	result = m_keyboard->Acquire();
	if(FAILED(result))
	{
		exit(0);
	}
}

void KeyboardController::getState()
{
    HRESULT result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if(FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return exit(0);
		}
	}
}

void KeyboardController::mapInputs()
{
	inputs->thrust = (m_keyboardState[DIK_W]&0x80)?255:0;
	inputs->tractBeam = (m_keyboardState[DIK_SPACE]&0x80)?255:0;
	inputs->pitch = (m_keyboardState[DIK_DOWN]&0x80)?-20000:(m_keyboardState[DIK_UP]&0x80)?20000:0;
	inputs->turn = (m_keyboardState[DIK_LEFT]&0x80)?-20000:(m_keyboardState[DIK_RIGHT]&0x80)?20000:0;
	inputs->brake = (m_keyboardState[DIK_S]&0x80)?true:false;
	inputs->reorient = (m_keyboardState[DIK_LSHIFT]&0x80)?true:false;
	inputs->push = (m_keyboardState[DIK_LCONTROL]&0x80)?true:false;
	inputs->powerup = (m_keyboardState[DIK_Q]&0x80)?true:false;
}

bool KeyboardController::isConnected()
{
    return m_keyboard != 0;
}