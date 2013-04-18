#include "GameInput.h"

GameInput::GameInput() {
	int cont;
	ConfigSettings conf = *ConfigSettings::config;
	conf.getValue("Controller",cont);
	switch ((char)cont) {
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