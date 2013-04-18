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
}

void GameInput::refreshState() {
	(*controller).refresh();
}

void GameInput::vibrate(int l, int r) {
	(*controller).vibrate(l,r);
}

GameInput::~GameInput() {
	delete(controller);
}