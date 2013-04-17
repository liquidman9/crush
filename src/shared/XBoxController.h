#pragma once
#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H

// No MFC
#define WIN32_LEAN_AND_MEAN

// We need the Windows Header and the XInput Header
//#include "GameInput.h"
#include <windows.h>
#include <XInput.h>
#include "Controller.h"

// Now, the XInput Library
// NOTE: COMMENT THIS OUT IF YOU ARE NOT USING
// A COMPILER THAT SUPPORTS THIS METHOD OF LINKING LIBRARIES
#pragma comment(lib, "XInput.lib")

// XBOX Controller Class Definition
class XBoxController : public Controller
{
	private:
    XINPUT_STATE controllerState;

	public:
	XBoxController(InputState * input);
	void getState();
    bool isConnected();
    void vibrate(int leftVal = 0, int rightVal = 0);
	void mapInputs();
};

#endif