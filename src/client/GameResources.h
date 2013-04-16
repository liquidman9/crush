//=================================================================================================
// GameResources.h Static container for game resources including meshes, objects, cameras, etc.
//=================================================================================================

#ifndef GameResources_H_INCLUDED
#define GameResources_H_INCLUDED

// Global includes
#include <vector>

// Project includes
#include <client/graphics/Mesh.h>
#include <client/graphics/Camera.h>
#include <client/graphics/Renderable.h>
#include <client/graphics/entities/R_Ship.h>

class GameResources {
public:

	//static enum cameras {DEBUG_CAM, PLAYER_CAM};  //better to use boolean with only two cameras, can extend later

	static bool debugCamOn;
	static Camera debugCam;
	static Camera* curCam;
	static std::vector<R_Ship*> r_ShipList;
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
	static void updateGameState();
	static void releaseResources();

};



#endif // GameResources_H_INCLUDED
