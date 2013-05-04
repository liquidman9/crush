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
			ofstream error_f("client_error.log");
			cerr.rdbuf(error_f.rdbuf());

			GameInput input;
#ifndef MYNETWORKOFF  // defined in Gbls

			//networking init
			int c_port = DEFAULT_PORT-1;
			ConfigSettings::config->getValue("network_clientPort", c_port);
			NetworkClient nc(c_port);


			int s_port = DEFAULT_PORT;
			ConfigSettings::config->getValue("network_serverPort", s_port);

			string server_ip = "127.0.0.1";
			ConfigSettings::config->getValue("network_serverIP", server_ip);


			try {
				nc.bindToServer(server_ip, (unsigned short) s_port);
				GameResources::playerNum = nc.getClientID();
			} catch (runtime_error &e) {
				cerr << e.what() << endl;
				bool exception_occured_check_error_log = false;
				assert(exception_occured_check_error_log);
			}



#else
			MessageBox( NULL, L"Network not enabled.\nRecompile with MYNETWORKOFF undefined to enable.", L"CRUSH.exe", MB_OK );
#endif
			// Main loop
			MSG msg;

			//for checking fps
			//DWORD startTime = timeGetTime();
			//DWORD count = 0;

			GameState<Entity> newGameState;

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

#ifndef MYNETWORKOFF  // defined in Gbls
				if (!GameResources::debugCamOn) {
					nc.sendToServer(&(input.input));
				}


				// Get game state from network
				if(nc.newStateAvailable()) {
					try{
						newGameState = nc.getGameState();	
					}  catch (runtime_error &e) {
						cerr << e.what() << endl;
						bool exception_occured_check_error_log = false;
						assert(exception_occured_check_error_log);
					}
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
