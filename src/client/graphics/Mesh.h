//=================================================================================================
// D3DWindow.h - D3D Window wrapper class
//=================================================================================================

#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

// Global includes
#define WIN32_LEAN_AND_MEAN
#include <d3dx9.h>
#include <string>

class Mesh
{
public:
	Mesh();
	~Mesh();

	// Create the Mesh
	HRESULT Create(std::wstring filepath);
	void Destroy();
	void draw();

	static void setScaleRotate(D3DXMATRIX & dest, float scaleFactor, float degX, float degY, float degZ);


//private: //maybe need to be public
	LPD3DXMESH          m_pMesh; // Our mesh object in sysmem
	D3DMATERIAL9*       m_pMeshMaterials; // Materials for our mesh
	LPDIRECT3DTEXTURE9* m_pMeshTextures; // Textures for our mesh
	DWORD               m_dwNumMaterials;   // Number of mesh materials

};



#endif // MESH_H_INCLUDED
