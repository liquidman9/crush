//=================================================================================================
// Main.cpp - Program entry point
//=================================================================================================

// Project includes
#include <client/network/NetworkClient.h>
#include <client/graphics/D3DWindow.h>
#include <client/GameResources.h>
#include <client/Gbls.h>

#pragma comment(lib, "WINMM.LIB")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// Init globals from config file
	Gbls::initFromConfig();

	// Create window
	if(D3DWindow::Create(hInstance))
	{
		//init space
		if(SUCCEEDED(GameResources::initState())) {
			
			GameInput input;
#ifndef MYNETWORKOFF  // defined in Gbls
			//networking init
			//try {
			NetworkClient nc(8887);
			nc.bindToServer("192.168.5.149", 8889);
			//nc.bindToServer("127.0.0.1", 8888);
			GameResources::playerNum = nc.getClientID();
			//} catch (exception & e) {
			//	cerr << e.what();
			//}
#else
			MessageBox( NULL, L"Network not enabled.\nRecompile with MYNETWORKOFF undefined to enable.", L"CRUSH.exe", MB_OK );
#endif
			// Main loop
			MSG msg;

			//for checking fps
			//DWORD startTime = timeGetTime();
			//DWORD count = 0;
			for(;;) // "forever"
			{

				input.refreshState();
				input.vibrate(input.input.thrust*200,input.input.thrust*200);
				//for checking fps
				//count++;
				//if(!(count%600)) {
				//	DWORD x = count / ((timeGetTime() - startTime)/1000.0);
				//	std::stringstream os;
				//	os << "FPS:" << x;
				//	string s =os.str();
				//	std::wstring wsTmp(s.begin(), s.end());
				//	MessageBox( NULL, wsTmp.c_str(), wsTmp.c_str(), MB_OK );
				//}
				
				GameState<Entity> newGameState;

#ifndef MYNETWORKOFF  // defined in Gbls
				if (!GameResources::debugCamOn) {
					nc.sendToServer(&(input.input));
				}


				// Get game state from network
				if(nc.newStateAvailable()) {
					newGameState = nc.getGameState();					
				}
#endif
				// Process all pending window messages
				while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				GameResources::updateGameState(newGameState);

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

	return 0;
}

//=================================================================================================
