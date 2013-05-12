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
	void vibrate(int l=0, int r=0);
	~GameInput();
};

#endif