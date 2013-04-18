#include<iostream>
#include<server/network/NetworkServer.h>
#include <shared/InputState.h>
#include <server/game/S_Ship.h>
#include <server/game/PhysicsWorld.h>

using namespace std;



int main(){
	try{
		cout << "Server:" << endl;
		NetworkServer server(8888);
		cout << "socket created:" << endl;
		EventBuff_t eventBuff;
		State_t gameState;
		PhysicsWorld world;
	
		D3DXVECTOR3 m_pos(-20,0,50);//(2,2,2);
		D3DXVECTOR3 m_dir(1,0,0);//(1,1,1);
		//gameState.push_back(Entity());
		S_Ship *test = new S_Ship(m_pos, m_dir, 1, false);
		test->m_acceleratorOn = true;
		gameState.push_back(test);
		world.entities.push_back(test);

		D3DXVECTOR3 m_pos1(20,0,50);//(1,1,1);
		D3DXVECTOR3 m_dir1(-1,0,0);
		S_Ship *test2 = new S_Ship(m_pos1, m_dir1, 2, true);
		test2->m_acceleratorOn = true;
		gameState.push_back(test2);
	    world.entities.push_back(test2);

		//ServerEntity *test2 = new S_Ship(m_pos1, m_dir, 2, true);
		//gameState.push_back(test2);
	//	world.entities.push_back(test2);
		int count = 0;
		for(;;) {
			if (server.eventsAvailable()) {
				eventBuff = server.getEvents();
				for(unsigned int i = 0; i < eventBuff.size(); i++) {
					
					//cout << *eventBuff[i] << endl;
					//gameState[1]->m_pos.z += (float)((((InputState *)eventBuff[i])->thrust))/250.0;
				
					cout << "Event recieved." << endl;
					world.update();
			
					
				}
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