#include "GameInput.h"


GameInput::GameInput() {
	controller = new XBoxController(&input);
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