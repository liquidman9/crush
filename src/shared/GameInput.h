//=================================================================================================
// GameState.h This class maintains an InputState for player input and handles the current Controller.
//=================================================================================================

#pragma once
#ifndef GAMEINPUT_H
#define GAMEINPUT_H

#include "XBoxController.h"
#include "KeyboardController.h"
#include "ConfigSettings.h"

class GameInput {

public:
	GameInput();
	Controller * controller;
	InputState input;
	void refreshState();
	void vibrate();
	void vibrate(int l, int r);
	void vibrateThrust(int thrust);
	void vibrateLock(bool lock);
	~GameInput();
private:
	int vibrateL;
	int vibrateR;
	int vibrateTh;
	bool vibrateLockOn;
};

#endif