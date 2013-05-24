#include "XBoxController.h"

XBoxController::XBoxController(InputState * input) : Controller(input)
{ 
}

void XBoxController::getState()
{
    // Zeroise the state
    ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
    // Get the state
    XInputGetState(0, &controllerState);
}

void XBoxController::mapInputs()
{
	inputs->thrust = (unsigned char)controllerState.Gamepad.bLeftTrigger;
	inputs->tractBeam = (unsigned char)controllerState.Gamepad.bRightTrigger;
	inputs->pitch = (controllerState.Gamepad.sThumbLY>0)?controllerState.Gamepad.sThumbLY-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE:controllerState.Gamepad.sThumbLY+XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	if ((inputs->pitch<0 && controllerState.Gamepad.sThumbLY>0) || (inputs->pitch>0 && controllerState.Gamepad.sThumbLY<0)) { inputs->pitch=0;}
	inputs->turn = (controllerState.Gamepad.sThumbLX>0)?controllerState.Gamepad.sThumbLX-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE:controllerState.Gamepad.sThumbLX+XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	if ((inputs->turn<0 && controllerState.Gamepad.sThumbLX>0) || (inputs->turn>0 && controllerState.Gamepad.sThumbLX<0)) { inputs->turn=0;}
	
	inputs->brake = (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B)?true:false;
	inputs->reorient = (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A)?true:false;
	inputs->push = (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)?true:false;
	inputs->powerup = (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y)?true:false;
	inputs->mash = (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X)?true:false;
}

bool XBoxController::isConnected()
{
    // Zeroise the state
    ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
    // Get the state
    DWORD Result = XInputGetState(0, &controllerState);

    if(Result == ERROR_SUCCESS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void XBoxController::vibrate(int leftVal, int rightVal)
{
    // Create a Vibraton State
    XINPUT_VIBRATION Vibration;

    // Zeroise the Vibration
    ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

    // Set the Vibration Values
    Vibration.wLeftMotorSpeed = leftVal;
    Vibration.wRightMotorSpeed = rightVal;

    // Vibrate the controller
    XInputSetState(0, &Vibration);
}