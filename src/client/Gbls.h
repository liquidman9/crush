//=================================================================================================
// Globals - File with globals in seperate namespace to prevent polution.
// There is probably a better option than this. Could be just ANOTHER static class. :/
// Many of these vairables will eventually be loaded in from config files
//=================================================================================================

#ifndef GBLS_H_INCLUDED
#define GBLS_H_INCLUDED

// Global includes
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

// Project includes
#include <client/graphics/Mesh.h>

namespace Gbls
{
	extern LPDIRECT3D9 pD3D;                            // D3D interface pointer
	extern LPDIRECT3DDEVICE9 pd3dDevice;                   // D3D device pointer
	extern D3DPRESENT_PARAMETERS thePresentParams;      // Present params used to create the device
	
	// Meshes
	extern Mesh shipMesh1;
	extern Mesh shipMesh2;

	extern float debugCamMoveSpeed;		//scales the movement speed of the Debug Cam
	extern float debugCamTurnSpeed;		//scales the turn speed of the Debug Cam
	
	extern float debugCamMaxPitch;
	extern float debugCamMinPitch;
	
	// Should be changed to load in from a config file later
	extern std::wstring shipMeshFilepath_1;
	extern std::wstring shipMeshFilepath_2;
	
	extern std::wstring skyboxTextureFilepath_Front;
	extern std::wstring skyboxTextureFilepath_Back;
	extern std::wstring skyboxTextureFilepath_Left;
	extern std::wstring skyboxTextureFilepath_Right;
	extern std::wstring skyboxTextureFilepath_Top;
	extern std::wstring skyboxTextureFilepath_Bottom;

	extern D3DXCOLOR lightDiffuseColor;
	extern D3DXCOLOR lightSpecularColor;
    extern D3DXVECTOR3 lightDirection;
}

#endif // GBLS_H_INCLUDED