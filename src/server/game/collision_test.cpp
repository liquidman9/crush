
// Global includes
#include <iostream>

// Project includes
#include <server/network/NetworkServer.h>
#include <shared/InputState.h>
#include <server/game/S_Ship.h>
#include <server/game/S_Asteroid.h>
#include <server/game/PhysicsWorld.h>

using namespace std;


int main9(){
	try {
		cout << "Server:" << endl;
		NetworkServer server(8888);
		cout << "socket created:" << endl;
		EventBuff_t eventBuff;
		GameState<Entity> gameState;

		PhysicsWorld world;
		/*
		float bound = 30;
		Boundary left = Boundary(D3DXVECTOR3(1.0f,0.0f,0.0f), D3DXVECTOR3(-bound,0.0f,0.0f));
		Boundary right = Boundary(D3DXVECTOR3(-1.0f,0.0f,0.0f), D3DXVECTOR3(bound,0.0f,0.0f));
		Boundary top = Boundary(D3DXVECTOR3(0.0f,-1.0f,0.0f), D3DXVECTOR3(0.0f,bound,0.0f));
		Boundary down = Boundary(D3DXVECTOR3(0.0f,1.0f,0.0f), D3DXVECTOR3(0.0f,-bound,0.0f));
		Boundary front = Boundary(D3DXVECTOR3(0.0f,0.0f,1.0f), D3DXVECTOR3(0.0f,0.0f,-bound));
		Boundary back = Boundary(D3DXVECTOR3(0.0f,0.0f,-1.0f), D3DXVECTOR3(0.0f,0.0f,bound));
		
		world.boundaries.push_back(left);
		world.boundaries.push_back(right);
		world.boundaries.push_back(top);
		world.boundaries.push_back(down);
		world.boundaries.push_back(front);
		world.boundaries.push_back(back);
		*/
		D3DXVECTOR3 m_pos(-10,2,2);
		Quaternion m_dir(0, 0, 0, 1);
		S_Ship *test = new S_Ship(m_pos, m_dir, 1);
		gameState.push_back(test);
		world.entities.push_back(test);

		D3DXVECTOR3 m_pos1(10,1,1);
		Quaternion m_dir1(0.25, 0.25, 0.25, 0.25);
		S_Ship *test2 = new S_Ship(m_pos1, m_dir1, 2);
		gameState.push_back(test2);
		world.entities.push_back(test2);

		/*
		D3DXVECTOR3 m_pos2(0,0,0);
		Quaternion m_dir2(0, 0, 0, 0);
		S_Asteroid *test3 = new S_Asteroid(m_pos2, m_dir2);
		gameState.push_back(test3);
		world.entities.push_back(test3);

		
		D3DXVECTOR3 m_pos3(0,0,15);
		Quaternion m_dir3(0, 0, 0, 0);
		S_Asteroid *test4 = new S_Asteroid(m_pos3, m_dir3);
		gameState.push_back(test4);
		world.entities.push_back(test4);

		
		D3DXVECTOR3 m_pos4(0,0,-15);
		S_Asteroid *test5 = new S_Asteroid(m_pos4, m_dir3);
		gameState.push_back(test5);
		world.entities.push_back(test5);

		D3DXVECTOR3 m_pos5(0,0,-35);
		S_Asteroid *test6 = new S_Asteroid(m_pos5, m_dir3);
		gameState.push_back(test6);
		world.entities.push_back(test6);

		*/

		for(;;) {
			server.broadcastGameState(gameState);

			eventBuff = server.getEvents();
			if(!eventBuff.empty()) {
				cout << "Event recieved." << endl;
				//(*((S_Ship **)(&gameState[1])))->addPlayerInput(*((InputState *)eventBuff[0].get()));
				test->addPlayerInput(*((InputState *)eventBuff[0].get()));
				//cout << (eventBuff[i] << endl;
				//gameState[1]->m_pos.z += (float)((((InputState *)eventBuff[0].get())->thrust))/250.0;
				//cout << (float)((((InputState *)eventBuff[0].get())->thrust)) << endl;
				//gameState[1]->m_pos.z += (float)((((InputState *)eventBuff[i].get())->thrust))/250.0;
				//gameState[1]->m_dir.z += (float)((((InputState *)eventBuff[i].get())->pitch))/2500.0;
			}
				
			world.update(1.0f/60.0f);
			if(eventBuff.size() < 50 && !eventBuff.empty()) {
				//gameState[1]->m_pos.z += .025;
				//gameState[0]->m_pos.z += .05;
				server.broadcastGameState(gameState);
			}
			if(!eventBuff.empty()) {
				eventBuff.clear();
			}
			Sleep(1000/60);
		}
	} catch (exception &e) {
		cout << "exception occured!" << endl;
		cerr << e.what() << endl;
		system("pause");
	}
	return 0;
}