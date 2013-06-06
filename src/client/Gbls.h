//=================================================================================================
// Globals - File with globals in seperate namespace to prevent polution.
// There is probably a better option than this. Could be just ANOTHER static class. :/
// Many of these vairables will eventually be loaded in from config files
//=================================================================================================

#ifndef GBLS_H_INCLUDED
#define GBLS_H_INCLUDED

// Global includes
#define WIN32_LEAN_AND_MEAN 

#include <d3d9.h>
#include <d3dx9.h>
#include <d3d9types.h>

// Project includes
#include <client/graphics/Mesh.h>
#include <client/graphics/Sprite.h>

//#define MYNETWORKOFF  //don't use network

//Our defines
class Gbls
{
public:

	// Not loaded in from config
	static LPDIRECT3D9 pD3D;                            // D3D interface pointer
	static LPDIRECT3DDEVICE9 pd3dDevice;                   // D3D device pointer
	static D3DPRESENT_PARAMETERS thePresentParams;      // Present params used to create the device
	
	// Meshes
	static Mesh shipMesh;
	static Mesh mothershipMesh;
	static Mesh * asteroidMesh;
	//static Mesh tractorBeamMesh;
	static Mesh resourceMesh;
	static Mesh extractorMesh;
	static Mesh powerupMesh; // probably would be changed to multiple
	
	static float debugCamMaxPitch;
	static float debugCamMinPitch;

	static int fontHeight;
	static float percentMissedFrames;
	
	// Loaded in from config file
	static float debugCamMoveSpeed;		//scales the movement speed of the Debug Cam
	static float debugCamTurnSpeed;		//scales the turn speed of the Debug Cam

	static int numAsteroidMeshes;
	
	static std::wstring shipMeshFilepath;
	static std::wstring mothershipMeshFilepath;
	static std::wstring * asteroidMeshFilepath;
	//static std::wstring tractorBeamMeshFilepath;
	static std::wstring resourceMeshFilepath;
	static std::wstring extractorMeshFilepath;
	static std::wstring powerupMeshFilepath;

	
	static std::wstring shipTexFilepath1;
	static std::wstring shipTexFilepath2;
	static std::wstring shipTexFilepath3;
	static std::wstring shipTexFilepath4;
	static std::wstring mothershipTexFilepath1;
	static std::wstring mothershipTexFilepath2;
	static std::wstring mothershipTexFilepath3;
	static std::wstring mothershipTexFilepath4;
	static std::wstring scoreScreenTexFilepath1;
	static std::wstring scoreScreenTexFilepath2;
	static std::wstring scoreScreenTexFilepath3;
	static std::wstring scoreScreenTexFilepath4;
	static std::wstring pScoreScrenAlphaTexFilepath;
	
	
	static LPDIRECT3DTEXTURE9 powerupTexture;
	static LPDIRECT3DTEXTURE9 shipTexture1;
	static LPDIRECT3DTEXTURE9 shipTexture2; 
	static LPDIRECT3DTEXTURE9 shipTexture3; 
	static LPDIRECT3DTEXTURE9 shipTexture4;
	static LPDIRECT3DTEXTURE9 mothershipTexture1; 
	static LPDIRECT3DTEXTURE9 mothershipTexture2; 
	static LPDIRECT3DTEXTURE9 mothershipTexture3; 
	static LPDIRECT3DTEXTURE9 mothershipTexture4;
	static LPDIRECT3DTEXTURE9 scoreScreenTexture[4];
	static LPDIRECT3DTEXTURE9 pScoreScrenAlphaTexture;
	
	static std::wstring skyboxTextureFilepath;
	//static std::wstring skyboxTextureFilepath_Front;
	//static std::wstring skyboxTextureFilepath_Back;
	//static std::wstring skyboxTextureFilepath_Left;
	//static std::wstring skyboxTextureFilepath_Right;
	//static std::wstring skyboxTextureFilepath_Top;
	//static std::wstring skyboxTextureFilepath_Bottom;
	
	static std::wstring shipEIDTextureFilepath_resource;
	static std::wstring ship1EIDTextureFilepath_insig;
	static std::wstring ship1EIDTextureFilepath_arrow;
	static std::wstring ship2EIDTextureFilepath_insig;
	static std::wstring ship2EIDTextureFilepath_arrow;
	static std::wstring ship3EIDTextureFilepath_insig;
	static std::wstring ship3EIDTextureFilepath_arrow;
	static std::wstring ship4EIDTextureFilepath_insig;
	static std::wstring ship4EIDTextureFilepath_arrow;
	
	static std::wstring powerupTextureFilepath;

	static std::wstring player1HudFilepath_insig;
	static std::wstring player2HudFilepath_insig;
	static std::wstring player3HudFilepath_insig;
	static std::wstring player4HudFilepath_insig;

	static std::wstring alertTextureFilepath;

	static std::wstring player1SplashTextureFilepath;
	static std::wstring player2SplashTextureFilepath;
	static std::wstring player3SplashTextureFilepath;
	static std::wstring player4SplashTextureFilepath;

	static std::wstring powerupTexture1Filepath;
	static std::wstring powerupTexture2Filepath;
	static std::wstring powerupTexture3Filepath;

	static std::wstring consumedPowerupTexture1Filepath;
	static std::wstring consumedPowerupTexture2Filepath;
	static std::wstring consumedPowerupTexture3Filepath;

	static std::wstring extractorEIDTextureOnScreenFilepath;
	static std::wstring extractorEIDTextureOffScreenFilepath_arrow;
	static std::wstring extractorEIDTextureOffScreenFilepath_insig;

	static std::wstring resourceEIDTextureFilepath;
	static std::wstring mothershipEIDTextureFilepath_arrow;
	static std::wstring mothershipEIDTextureFilepath_insig;


	static std::wstring tBeamPartTexFilepath;
	static std::wstring enginePartTexNormFilepath;
	static std::wstring enginePartTexSpeedupFilepath;
	static std::wstring fontStyle;
	static int fontSize;

	static D3DXCOLOR lightDiffuseColor;
	static D3DXCOLOR lightSpecularColor;
    static D3DXVECTOR3 lightDirection;
	static D3DCOLOR lightAmbient;
	
	static int windowWidth;
	static int windowHeight;
	static int fullScreenWidth;
	static int fullScreenHeight;

	// Does not actually do loading from config, only sets variables from already-loaded Settings class
	static void initFromConfig();
};

#endif // GBLS_H_INCLUDED