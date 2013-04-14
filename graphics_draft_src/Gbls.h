//=================================================================================================
// Globals - File with globals in seperate namespace to prevent polution.
// There is probably a better option than this.
//=================================================================================================

#ifndef GBLS_H_INCLUDED
#define GBLS_H_INCLUDED

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Mesh.h"

namespace Gbls
{
	extern LPDIRECT3D9 pD3D;                            // D3D interface pointer
	extern LPDIRECT3DDEVICE9 pd3dDevice;                   // D3D device pointer
	extern D3DPRESENT_PARAMETERS thePresentParams;      // Present params used to create the device

	extern float debugCamMoveSpeed;		//scales the movement speed of the Debug Cam
	extern float debugCamTurnSpeed;		//scales the turn speed of the Debug Cam
	
	extern float debugCamMaxPitch;
	extern float debugCamMinPitch;

	// Meshes
	extern Mesh shipMesh1;
	extern Mesh shipMesh2;
}

#endif // GBLS_H_INCLUDED