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

//#define MYNETWORKOFF  //don't use network

//Our defines
class Gbls
{
public:
	static int numShipMeshes;

	static LPDIRECT3D9 pD3D;                            // D3D interface pointer
	static LPDIRECT3DDEVICE9 pd3dDevice;                   // D3D device pointer
	static D3DPRESENT_PARAMETERS thePresentParams;      // Present params used to create the device
	
	// Meshes
	static Mesh * shipMesh;

	static float debugCamMoveSpeed;		//scales the movement speed of the Debug Cam
	static float debugCamTurnSpeed;		//scales the turn speed of the Debug Cam
	
	static float debugCamMaxPitch;
	static float debugCamMinPitch;
	
	// Should be changed to load in from a config file later
	//static std::wstring shipMeshFilepath_1;
	//static std::wstring shipMeshFilepath_2;
	static std::wstring * shipMeshFilepath;
	
	static std::wstring skyboxTextureFilepath_Front;
	static std::wstring skyboxTextureFilepath_Back;
	static std::wstring skyboxTextureFilepath_Left;
	static std::wstring skyboxTextureFilepath_Right;
	static std::wstring skyboxTextureFilepath_Top;
	static std::wstring skyboxTextureFilepath_Bottom;

	static D3DXCOLOR lightDiffuseColor;
	static D3DXCOLOR lightSpecularColor;
    static D3DXVECTOR3 lightDirection;
	static D3DCOLOR lightAmbient;
	
	static int windowWidth;
	static int windowHeight;
	static int fullScreenWidth;
	static int fullScreenHeight;

	static void initFromConfig();
};

#endif // GBLS_H_INCLUDED