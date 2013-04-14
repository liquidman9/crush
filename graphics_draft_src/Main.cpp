//=================================================================================================
// Main.cpp - Program entry point
//=================================================================================================
#include "D3DWindow.h"
#include "GameResources.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// Create window
	if(D3DWindow::Create(hInstance))
	{
		//init space
		if(SUCCEEDED(GameResources::initState())) {

			// Main loop
			MSG msg;
			//LARGE_INTEGER time;
			//LARGE_INTEGER deltaTime;
			for(;;) // "forever"
			{
				// Process all pending window messages
				while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				GameResources::updateGameState();

				// Render
				if(D3DWindow::Tick())
					break;
			}
		}
	}
	D3DWindow::Destroy();

	// See if there's any error messsage pending
	const std::wstring& strError = D3DWindow::GetError();
	if(!strError.empty())
	{
		MessageBox(NULL, strError.c_str(), L"Error", MB_OK | MB_ICONERROR);
	}

	return -1;
}

//=================================================================================================
