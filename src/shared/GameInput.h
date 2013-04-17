#pragma once
#ifndef GAMEINPUT_H
#define GAMEINPUT_H

#include "XBoxController.h"

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