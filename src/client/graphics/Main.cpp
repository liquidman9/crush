//=================================================================================================
// Main.cpp - Program entry point
//=================================================================================================

// Project includes
#include <client/network/NetworkClient.h>
#include <client/graphics/D3DWindow.h>
#include <client/GameResources.h>

#pragma comment(lib, "WINMM.LIB")

//#define MYNETWORKON

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// Create window
	if(D3DWindow::Create(hInstance))
	{
		//init space
		if(SUCCEEDED(GameResources::initState())) {

#ifdef MYNETWORKON
			//networking init
			//try {
			NetworkClient nc(8888);
			nc.bindToServer("192.168.5.149", 8888);
			//} catch (exception & e) {
			//	cerr << e.what();
			//}
#endif
			// Main loop
			MSG msg;

			//for checking fps
			//DWORD startTime = timeGetTime();
			//DWORD count = 0;
			for(;;) // "forever"
			{
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
				
				vector<Entity> newGameState;

#ifdef MYNETWORKON
				nc.sendToServer(Entity());


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
