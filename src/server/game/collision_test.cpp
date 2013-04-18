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
		int bound = 30;
		Boundary left = Boundary(D3DXVECTOR3(1,0,0), D3DXVECTOR3(-bound,0,0));
		Boundary right = Boundary(D3DXVECTOR3(-1,0,0), D3DXVECTOR3(bound,0,0));
		Boundary top = Boundary(D3DXVECTOR3(0,-1,0), D3DXVECTOR3(0,bound,0));
		Boundary down = Boundary(D3DXVECTOR3(0,1,0), D3DXVECTOR3(0,-bound,0));
		Boundary front = Boundary(D3DXVECTOR3(0,0,1), D3DXVECTOR3(0,0,-bound));
		Boundary back = Boundary(D3DXVECTOR3(0,0,-1), D3DXVECTOR3(0,0,bound));
		world.boundaries.push_back(left);
		world.boundaries.push_back(right);
		world.boundaries.push_back(top);
		world.boundaries.push_back(down);
		world.boundaries.push_back(front);
		world.boundaries.push_back(back);

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

		D3DXVECTOR3 m_pos2(0,0,20);//(2,2,2);
		D3DXVECTOR3 m_dir2(0,0,-1);//(1,1,1);
		//S_Ship *test3 = new S_Ship(m_pos2, m_dir2, 3, true);
		//test3->m_acceleratorOn = true;
		//gameState.push_back(test3);
	   // world.entities.push_back(test3);

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
				if(count < 200) {
					cout << "Event recieved." << endl;
					world.update();
					//count++;
				}
					
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