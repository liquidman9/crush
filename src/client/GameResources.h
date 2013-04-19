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
#include <shared/game/Entity.h>
#include <shared/game/GameState.h>
#include <shared/GameInput.h>

#include <client/graphics/Mesh.h>
#include <client/graphics/Camera.h>
#include <client/graphics/Renderable.h>
#include <client/graphics/entities/C_Entity.h>

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

	// A map of Client Entities -- this allows entity updating and rendering (C_Entity extends both Entity and Renderable)
	static std::map<int, C_Entity*> entityMap;
	static bool debugCamOn;
	static Camera debugCam;
	static Camera* curCam;
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
	static HRESULT initMeshes();
	static HRESULT initLights();
	static void drawAll();
	static void updateDebugCamera();
	static void updateGameState(GameState & newGameState);
	static C_Entity * createEntity(Entity * newEnt);
	static void releaseResources();

};



#endif // GameResources_H_INCLUDED
