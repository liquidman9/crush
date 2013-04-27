//=================================================================================================
// GameResources.h Static container for game resources including meshes, objects, cameras, etc.
//=================================================================================================

#ifndef GameResources_H_INCLUDED
#define GameResources_H_INCLUDED

// Global includes
#include <vector>
#include <memory>
#include <map>

// Project includes
#include <client/graphics/Mesh.h>
#include <client/graphics/Camera.h>
#include <client/graphics/Renderable.h>
#include <shared/game/Entity.h>
//typedef vector<shared_ptr<Entity>> GameState;
#include <shared/game/GameState.h>
//#include <client/graphics/entities/R_Ship.h>
#include <shared/GameInput.h>
#include <client/graphics/entities/C_Entity.h>
#include <client/graphics/entities/C_Ship.h>
#include <client/graphics/EntityIdentifier.h>
#include <client/graphics/entities/C_Asteroid.h>

class GameResources {
public:

	//static enum cameras {DEBUG_CAM, PLAYER_CAM};  //better to use boolean with only two cameras, can extend later

		//static struct cmp_entity
	//{
	//	bool operator()(Entity const *a, Entity const *b)
	//	{
	//		return a->m_type < b->m_type;
	//	}
	//};

	static vector<C_Ship*> shipList;
	static vector<C_Asteroid*> asteroidList;
	static const float PLAYER_CAM_DISTANCE;
	static const float PLAYER_CAM_HEIGHT;
	static const float PLAYER_CAM_LOOKAT_DISTANCE;
	static std::map<int, C_Entity*> entityMap;
	static bool debugCamOn;
	static Camera debugCam;
	static Camera playerCam;
	static Camera * curCam;
	static C_Ship * playerShip;
	static int playerNum;
	static LPD3DXSPRITE pd3dSprite;
	static vector<EntityIdentifier*> eIDList;
	static LPDIRECT3DTEXTURE9 shipEIDTexture;
	//static vector<Sprite*> spriteList;
	//static std::vector<Entity*> entityList;
	//static std::vector<R_Ship*> r_ShipList;
	//static std::vector<std::vector<Renderable*>*> renderList;

	static struct KeyboardState{
		bool wDown;
		bool aDown;
		bool sDown;
		bool dDown;
		bool upDown;
		bool leftDown;
		bool downDown;
		bool rightDown;
		bool wUp;
		bool aUp;
		bool sUp;
		bool dUp;
		bool upUp;
		bool leftUp;
		bool downUp;
		bool rightUp;
	};

	static struct GameResources::KeyboardState m_ks;

	//static Camera playerCam;

	static void updateKeyboardState();
	static HRESULT initState();
	static HRESULT reInitState();
	static HRESULT initMeshes();
	static HRESULT initAdditionalTextures();
	static void releaseAdditionalTextures();
	static HRESULT initLights();
	//static HRESULT initSprites();
	static void drawAll();
	static void drawAllEID();
	//static void drawAllSprites();
	static void updateDebugCamera();
	static void updatePlayerCamera();
	static void switchCamera();
	static void updateGameState(GameState<Entity> & newGameState);
	static C_Entity * createEntity(Entity * newEnt);
	static void releaseResources();

};



#endif // GameResources_H_INCLUDED
