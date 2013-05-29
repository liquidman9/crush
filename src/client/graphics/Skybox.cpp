/*
 * Skybox.cpp
 * Creates a skybox (cube based, from 6 seperate textures) and displays it correctly
 */

// Project includes
#include <client/graphics/Skybox.h>
#include <client/Gbls.h>
#include <client/GameResources.h>


LPDIRECT3DVERTEXBUFFER9 Skybox::s_pVertexBuffer = NULL;

LPDIRECT3DTEXTURE9 Skybox::s_SkyTextures[6];

Skybox::TVertex Skybox::s_SkyboxMesh[24] =
{
	// Front quad, NOTE: All quads face inward
	{-10.0f, -10.0f,  10.0f,  0.0f, 1.0f },
	{-10.0f,  10.0f,  10.0f,  0.0f, 0.0f },
	{ 10.0f, -10.0f,  10.0f,  1.0f, 1.0f },
	{ 10.0f,  10.0f,  10.0f,  1.0f, 0.0f },
	
	// Back quad
	{ 10.0f, -10.0f, -10.0f,  0.0f, 1.0f },
	{ 10.0f,  10.0f, -10.0f,  0.0f, 0.0f },
	{-10.0f, -10.0f, -10.0f,  1.0f, 1.0f },
	{-10.0f,  10.0f, -10.0f,  1.0f, 0.0f },
	
	// Left quad
	{-10.0f, -10.0f, -10.0f,  0.0f, 1.0f },
	{-10.0f,  10.0f, -10.0f,  0.0f, 0.0f },
	{-10.0f, -10.0f,  10.0f,  1.0f, 1.0f },
	{-10.0f,  10.0f,  10.0f,  1.0f, 0.0f },
	
	// Right quad
	{ 10.0f, -10.0f,  10.0f,  0.0f, 1.0f },
	{ 10.0f,  10.0f,  10.0f,  0.0f, 0.0f },
	{ 10.0f, -10.0f, -10.0f,  1.0f, 1.0f },
	{ 10.0f,  10.0f, -10.0f,  1.0f, 0.0f },

	// Top quad
	{-10.0f,  10.0f,  10.0f,  0.0f, 1.0f },
	{-10.0f,  10.0f, -10.0f,  0.0f, 0.0f },
	{ 10.0f,  10.0f,  10.0f,  1.0f, 1.0f },
	{ 10.0f,  10.0f, -10.0f,  1.0f, 0.0f },
	
	// Bottom quad
	{-10.0f, -10.0f, -10.0f,  0.0f, 1.0f },
	{-10.0f, -10.0f,  10.0f,  0.0f, 0.0f },
	{ 10.0f, -10.0f, -10.0f,  1.0f, 1.0f },
	{ 10.0f, -10.0f,  10.0f,  1.0f, 0.0f }
};


HRESULT Skybox::initSkybox()
{
	HRESULT hRes;

	// Create our vertex buffer ( 24 vertices (4 verts * 6 faces) )
	hRes = Gbls::pd3dDevice->CreateVertexBuffer( sizeof(TVertex) * 24,			// size of vertex buffer in bytes
											 0,								// Usage - 0 is hardware vertex processing
											 D3DFVF_XYZ | D3DFVF_TEX1,		// FVF flags - this case is untransformed vertex and 1 texture coords
											 D3DPOOL_MANAGED,				// Memory pool is managed by the device
											 &s_pVertexBuffer,				// the vertex buffer !NULL is successful
											 NULL );						// Reserved
    if ( FAILED( hRes ) )
	{
		::MessageBox(NULL, L"Failed to create the vertex buffer!", L"Error in Skybox::initSkybox()", MB_OK | MB_ICONSTOP);
		s_pVertexBuffer->Release();
		s_pVertexBuffer = NULL;
		return hRes;
	}

	void *pVertices = NULL;

	// Copy the skybox mesh into the vertex buffer. 
	s_pVertexBuffer->Lock( 0, sizeof(TVertex) * 24, (void**)&pVertices, 0 );
    memcpy( pVertices, s_SkyboxMesh, sizeof(TVertex) * 24 );
    s_pVertexBuffer->Unlock();

    hRes  = D3DXCreateTextureFromFile( Gbls::pd3dDevice, Gbls::skyboxTextureFilepath_Front.c_str(), &s_SkyTextures[0] );
    hRes |= D3DXCreateTextureFromFile( Gbls::pd3dDevice, Gbls::skyboxTextureFilepath_Back.c_str(), &s_SkyTextures[1] );
    hRes |= D3DXCreateTextureFromFile( Gbls::pd3dDevice, Gbls::skyboxTextureFilepath_Left.c_str(), &s_SkyTextures[2] );
    hRes |= D3DXCreateTextureFromFile( Gbls::pd3dDevice, Gbls::skyboxTextureFilepath_Right.c_str(), &s_SkyTextures[3] );
    hRes |= D3DXCreateTextureFromFile( Gbls::pd3dDevice, Gbls::skyboxTextureFilepath_Top.c_str(), &s_SkyTextures[4] );
    hRes |= D3DXCreateTextureFromFile( Gbls::pd3dDevice, Gbls::skyboxTextureFilepath_Bottom.c_str(), &s_SkyTextures[5] );
    if ( FAILED(hRes) )
	{
		::MessageBox(NULL, L"Failed to load skybox!", L"Error Opening Texture Files", MB_OK | MB_ICONSTOP);
		s_pVertexBuffer->Release();
		s_pVertexBuffer = NULL;
		return hRes;
	}

	return hRes;
}

void Skybox::drawSkybox() {
	// turn zbuffer and lighting off for skybox
    Gbls::pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, false );
	Gbls::pd3dDevice->SetRenderState( D3DRS_LIGHTING, false );

    Gbls::pd3dDevice->SetFVF( D3DFVF_XYZ | D3DFVF_TEX1 );
    Gbls::pd3dDevice->SetStreamSource( 0, s_pVertexBuffer, 0, sizeof(TVertex));

	//set default world matrix for skybox
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	Gbls::pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld);

	// Set camera view to zero for skybox
	GameResources::curCam->setCenteredView();

	for(int i = 0; i < 6; ++i)
    {
		// Set the texture for this primitive
        Gbls::pd3dDevice->SetTexture( 0, s_SkyTextures[i] );

		// Render the face (one strip per face from the vertex buffer)  There are 2 primitives per face.
        Gbls::pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, i * 4, 2 );
    }

	// Set camera view back to previous position
	GameResources::curCam->updateView();

	// turn lighting and zbuffer back on
    Gbls::pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );
	Gbls::pd3dDevice->SetRenderState( D3DRS_LIGHTING, true );
}

void Skybox::releaseSkybox() {
	for (int i = 0; i < 6; i++)
	{
		if(s_SkyTextures[i]) {
			s_SkyTextures[i]->Release();
			s_SkyTextures[i] = NULL;
		}
	}

	// Release the vertex buffer
	if (s_pVertexBuffer)
	{
		s_pVertexBuffer->Release();
		s_pVertexBuffer = NULL;
	}
}