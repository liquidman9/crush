//=================================================================================================
// GameState.cpp This class maintains an InputState for player input and handles the current Controller.
//=================================================================================================

#include "GameInput.h"

GameInput::GameInput() {
	string cont = "k";
	ConfigSettings conf = *ConfigSettings::config;
	//if (!conf.checkIfLoaded()) {conf.loadSettingsFile();}
	conf.getValue("Controller",cont);
	switch (cont[0]) {
	case 'k':
		controller = new KeyboardController(&input);
		break;
	case 'x':
		controller = new XBoxController(&input);
		break;
	default:
		controller = new XBoxController(&input);
		break;
	}
	vibrateT = 0;
	vibrateR = 0;
	vibrateL = 0;
}

void GameInput::refreshState() {
	(*controller).refresh();
	vibrate();
}

void GameInput::vibrate() {
	int rTemp = vibrateR+(2*vibrateTh);
	int lTemp = vibrateL+vibrateTh;
	if (vibrateL > 0) {
		vibrateL-=10000;
	} else {
		vibrateL = 0;
	}
	if (vibrateR > 0) {
		vibrateR-=10000;
	} else {
		vibrateR = 0;
	}
	(*controller).vibrate(lTemp,rTemp);
}

void GameInput::vibrate(int l, int r, int time) {
	vibrateL += l;
	vibrateR += r;
	vibrateT = time;
}

void GameInput::vibrateThrust(int thrust) {
	vibrateTh = thrust*20;
}

GameInput::~GameInput() {
	delete(controller);
}