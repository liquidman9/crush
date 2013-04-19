
// Global includes
#include <iostream>

// Project includes
#include <server/network/NetworkServer.h>
#include <shared/InputState.h>

using namespace std;

void printEntity(Entity const &e) {
	cout << e.getID() << " " << e.m_pos.x << " " << e.m_pos.y << " " << e.m_pos.z
		<< e.m_dir.x << " " << e.m_dir.y << " " << e.m_dir.z << endl;
	}

int main(){
	try {
		cout << "Server:" << endl;
		NetworkServer server(8888);
		cout << "socket created:" << endl;
		EventBuff_t eventBuff;
		GameState gameState;
		D3DXVECTOR3 m_pos(2,2,2);
		D3DXVECTOR3 m_dir(1,1,1);
		Entity *test = new Ship(m_pos, m_dir, 1, false);
		gameState.push_back(test);
		D3DXVECTOR3 m_pos1(1,1,1);
		D3DXVECTOR3 m_dir1(0,0,1);
		test = new Ship(m_pos1, m_dir, 2, true);
		gameState.push_back(test);

		for(;;) {
			server.broadcastGameState(gameState);
			
				eventBuff = server.getEvents();
				if(!eventBuff.empty()) {
					cout << "Event recieved." << endl;
					//cout << (eventBuff[i] << endl;
					gameState[1]->m_pos.z += (float)((((InputState *)eventBuff[0].get())->thrust))/250.0;
					cout << (float)((((InputState *)eventBuff[0].get())->thrust)) << endl;
					//gameState[1]->m_pos.z += (float)((((InputState *)eventBuff[i].get())->thrust))/250.0;
					//gameState[1]->m_dir.z += (float)((((InputState *)eventBuff[i].get())->pitch))/2500.0;
				}
		
			if(eventBuff.size() < 50 && !eventBuff.empty()) {
				//gameState[1]->m_pos.z += .025;
				//gameState[0]->m_pos.z += .05;
				server.broadcastGameState(gameState);
			}
			if(!eventBuff.empty()) {
				eventBuff.clear();
			}
			Sleep(16);
		}
	} catch (exception &e) {
		cout << "exception occured!" << endl;
		cerr << e.what() << endl;
		system("pause");
	}

}
