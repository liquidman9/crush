/*
 * Skybox.h
 */

#ifndef Skybox_H_INCLUDED
#define Skybox_H_INCLUDED
 
#define WIN32_LEAN_AND_MEAN

#include <d3dx9.h>

class Skybox {
public:

	// Vertex structure for skybox texture
	struct TVertex
	{
		float x, y, z;		// 3D world position
		float tu, tv;		// Texture coordinates
	};
	
	static LPDIRECT3DVERTEXBUFFER9 s_pVertexBuffer;
	static LPDIRECT3DTEXTURE9 s_SkyTextures[6];
	static TVertex s_SkyboxMesh [24];

	static HRESULT initSkybox();
	static void drawSkybox();
	static void releaseSkybox();
};


#endif