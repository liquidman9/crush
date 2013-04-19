#pragma once

#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H


// We need the Windows Header and the XInput Header
//#include "GameInput.h"
//#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800

#include "Controller.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")




#include <dinput.h>

// XBOX Controller Class Definition
class KeyboardController : public Controller
{
	private:
    IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

	public:
	KeyboardController(InputState * input);
	void getState();
    bool isConnected();
	void vibrate(int leftVal = 0, int rightVal = 0) {};
	void mapInputs();
};

#endif