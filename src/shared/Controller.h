#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "InputState.h"

// Abstract Controller
class Controller
{
	protected:
	InputState * inputs;

	public:
    Controller(InputState * input) { inputs = input; };
    virtual void getState() = 0;
    virtual bool isConnected() = 0;
    virtual void vibrate(int leftVal = 0, int rightVal = 0) = 0;
	virtual void mapInputs() = 0;
	void refresh() { getState(); mapInputs(); }
};

#endif