//=================================================================================================
// GameResources.h Static container for game resources including meshes, objects, cameras, etc.
//=================================================================================================

#ifndef GameResources_H_INCLUDED
#define GameResources_H_INCLUDED

// Global includes
#include <vector>
#include <memory>
#include <map>
#include <mmsystem.h>

// Project includes
#include <client/graphics/Mesh.h>
#include <client/graphics/Camera.h>
#include <shared/game/Entity.h>
//typedef vector<shared_ptr<Entity>> GameState;
#include <shared/game/GameState.h>
//#include <client/graphics/entities/R_Ship.h>
#include <shared/GameInput.h>
#include <client/graphics/EntityIdentifier.h>
#include <client/graphics/entities/C_Extractor.h>
#include <client/graphics/entities/C_Resource.h>
#include <client/graphics/entities/C_TractorBeam.h>
#include <client/graphics/entities/C_Mothership.h>
#include <client/graphics/entities/C_Asteroid.h>
#include <client/graphics/entities/C_Ship.h>
#include <client/graphics/entities/C_Entity.h>
#include <client/graphics/entities/C_Powerup.h>
#include <client/graphics/ParticleSystem.h>
#include <client/graphics/TBeamPGroup.h>
#include <client/graphics/EnginePGroup.h>
#include <client/graphics/BurstPGroup.h>
#include <client/SoundManager.h>

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
	static vector<C_Mothership*> mothershipList;
	static vector<C_TractorBeam*> tractorBeamList;
	static vector<C_Resource*> resourceList;
	static vector<C_Extractor*> extractorList;
	static vector<C_Powerup*> powerupList;
	static vector<EntityIdentifier*> eIDList;
	static vector<EnginePGroup *> enginePGroupList;
	static const float PLAYER_CAM_DISTANCE;
	static const float PLAYER_CAM_HEIGHT;
	static const float PLAYER_CAM_LOOKAT_DISTANCE;
	static std::map<int, C_Entity*> entityMap;
	static bool debugCamOn;
	static Camera debugCam;
	static Camera playerCam;
	static Camera * curCam;
	static float playerCamScale;
	static int playerCamScaleLevel;
	static C_Ship * playerShip;
	static C_Mothership * playerMothership;
	static int playerNum;
	static LPD3DXSPRITE pd3dSprite;
	static LPD3DXFONT GameResources::pd3dFont;
	
	//ship indicators
	static LPDIRECT3DTEXTURE9 shipEIDTexture_resource;
	static LPDIRECT3DTEXTURE9 ship1EIDTexture_insig;
	static LPDIRECT3DTEXTURE9 ship1EIDTexture_arrow;
	static LPDIRECT3DTEXTURE9 ship2EIDTexture_insig;
	static LPDIRECT3DTEXTURE9 ship2EIDTexture_arrow;
	static LPDIRECT3DTEXTURE9 ship3EIDTexture_insig;
	static LPDIRECT3DTEXTURE9 ship3EIDTexture_arrow;
	static LPDIRECT3DTEXTURE9 ship4EIDTexture_insig;
	static LPDIRECT3DTEXTURE9 ship4EIDTexture_arrow;
	static LPDIRECT3DTEXTURE9* shipEIDTextureArray_arrow[4];
	static LPDIRECT3DTEXTURE9* shipEIDTextureArray_insig[4];

	static LPDIRECT3DTEXTURE9 extractorEIDTextureOnScreen;
	static LPDIRECT3DTEXTURE9 extractorEIDTextureOffScreen;

	static LPDIRECT3DTEXTURE9 resourceEIDTexture;
	static LPDIRECT3DTEXTURE9 mothershipEIDTexture;
	static LPDIRECT3DTEXTURE9 tBeamPartTexture;
	static LPDIRECT3DTEXTURE9 EnginePartTexture;
	static ParticleSystem * partSystem;
	static TBeamPGroup * tBeamPGroup;
	static BurstPGroup * burstPowerupPGroup;
	static std::wstring timeStr;
	static std::wstring playerNameStr[4];
	static int playerScore[4];
	static SoundManager sound;

	// effects (shaders)
	static ID3DXEffect * pEffectDefault;
	static ID3DXEffect * pEffectGlowmap;
	static ID3DXEffect * pEffectTexToScreen;
	static ID3DXEffect * pEffectBlend;
	static ID3DXEffect * pEffectBlur;

	static D3DXMATRIX sunWorldMat;
	static LPD3DXMESH sunMesh;
	static LPD3DXMESH shieldMesh;
	static LPDIRECT3DTEXTURE9 pGlowmapTexture;
	static LPDIRECT3DSURFACE9 pGlowmapSurface;
	static LPDIRECT3DTEXTURE9 pTmpBlurTexture;
	static LPDIRECT3DTEXTURE9 pDefaultRenderTexture;
	static LPDIRECT3DSURFACE9 pDefaultRenderSurface;
	static LPDIRECT3DSURFACE9 pBackBuffer;

	// for debugging collisions
	static bool renderCBWireframe;
	static LPD3DXMESH collisionSphere;
	static LPD3DXMESH collisionCylinder;
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
	static HRESULT loadEffect(ID3DXEffect ** pEffect, std::wstring effectLoc);
	static HRESULT loadTexture(LPDIRECT3DTEXTURE9 * pTextureOut, std::wstring filepath);
	static HRESULT loadTextureWithFormat(LPDIRECT3DTEXTURE9 * pTextureOut, std::wstring filepath, _D3DFORMAT format);
	static HRESULT initAdditionalTextures();
	static HRESULT loadFont(LPD3DXFONT * pFont, int height, std::wstring fontStyle);
	static void releaseAdditionalTextures();
	static HRESULT initLights();
	//static HRESULT initSprites();
	static void drawAll();
	static void createGlowmap();
	static void DrawBGSun();
	//static void drawTexToScreen(LPDIRECT3DSURFACE9 surface, UINT surfaceHeight, UINT surfaceWidth);
	static void drawTexToSurface(LPDIRECT3DTEXTURE9 tex, LPDIRECT3DVERTEXBUFFER9 quadVBuffer);
	static void blendTexesToSurface(LPDIRECT3DTEXTURE9 tex1, LPDIRECT3DTEXTURE9 tex2, LPDIRECT3DVERTEXBUFFER9 quadVBuffer);
	static void blurTexture(LPDIRECT3DTEXTURE9 tex1, LPDIRECT3DTEXTURE9 tex2);
	static void drawModel(C_Entity * cEnt);
	static void drawAllModels();
	static void drawAllTractorBeams();
	static void drawAllShields();
	static void releaseBurstPowerupParticles();
	static void drawAllEngines();
	static void drawAllEID();
	static void drawShield(C_Entity * target);
	static void drawStaticHudElements();
	static void drawCollisionBounds(D3DXVECTOR3 & pt1, D3DXVECTOR3 & pt2, float radius);
	static void playSounds();
	//static void drawAllSprites();
	static void updateDebugCamera();
	static void updatePlayerCamera();
	static void switchCamera();
	static void switchPlayerCameraScale();
	static void updateGameState(GameState<Entity> & newGameState);
	static void resetGameState();
	static C_Entity * createEntity(Entity * newEnt);
	static void releaseResources();

private:
	static void placeTextCenterCeiling(LPCWSTR str, UINT x);
	static void placeTextCenterFloor(LPCWSTR str, UINT x);
};



#endif // GameResources_H_INCLUDED