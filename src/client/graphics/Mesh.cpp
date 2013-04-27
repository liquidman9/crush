
// Project includes
#include <client/graphics/Mesh.h>
#include <client/Gbls.h>

Mesh::Mesh() :
	m_pMesh(NULL),
	m_pMeshMaterials(NULL),
	m_pMeshTextures(NULL),
	m_dwNumMaterials(0L)
{
	D3DXMatrixIdentity(&m_matInitScaleRot);
}

Mesh::~Mesh()
{
	Destroy();
}

HRESULT Mesh::Create(std::wstring filepath)
{
	
    LPD3DXBUFFER pD3DXMtrlBuffer;

    // Load the mesh from the specified file
	if( FAILED( D3DXLoadMeshFromX( filepath.c_str(), D3DXMESH_SYSTEMMEM,
                                   Gbls::pd3dDevice, NULL,
                                   &pD3DXMtrlBuffer, NULL, &m_dwNumMaterials,
                                   &m_pMesh ) ) )
    {
        // If model is not in current folder, try parent folder
        if( FAILED( D3DXLoadMeshFromX( (L"..\\"+filepath).c_str(), D3DXMESH_SYSTEMMEM,
                                       Gbls::pd3dDevice, NULL,
                                       &pD3DXMtrlBuffer, NULL, &m_dwNumMaterials,
                                       &m_pMesh ) ) )
        {
            MessageBox( NULL, (L"Could not find "+filepath).c_str(), L"CRUSH.exe", MB_OK );
            return E_FAIL;
        }
    }

    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterials = ( D3DXMATERIAL* )pD3DXMtrlBuffer->GetBufferPointer();
    m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
    if( m_pMeshMaterials == NULL )
        return E_OUTOFMEMORY;
    m_pMeshTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];
    if( m_pMeshTextures == NULL )
        return E_OUTOFMEMORY;

    for( DWORD i = 0; i < m_dwNumMaterials; i++ )
    {
        // Copy the material
        m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;
		//TODO get power settings right for each material, no hard coding, remove this line
        //m_pMeshMaterials[i].Power = 15;

        m_pMeshTextures[i] = NULL;
        if( d3dxMaterials[i].pTextureFilename != NULL &&
            lstrlenA( d3dxMaterials[i].pTextureFilename ) > 0 )
        {
            // Create the texture
            if( FAILED( D3DXCreateTextureFromFileA( Gbls::pd3dDevice,
                                                    d3dxMaterials[i].pTextureFilename,
                                                    &m_pMeshTextures[i] ) ) )
            {
                // If texture is not in current folder, try parent folder
                const CHAR* strPrefix = "..\\";
                CHAR strTexture[MAX_PATH];
                strcpy_s( strTexture, MAX_PATH, strPrefix );
                strcat_s( strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename );
                // If texture is not in current folder, try parent folder
                if( FAILED( D3DXCreateTextureFromFileA( Gbls::pd3dDevice,
                                                        strTexture,
                                                        &m_pMeshTextures[i] ) ) )
                {
                    MessageBox( NULL, L"Could not find texture map", L"CRUSH.exe", MB_OK );
                }
            }
        }
    }

    // Done with the material buffer
    pD3DXMtrlBuffer->Release();
	pD3DXMtrlBuffer = NULL;

    return S_OK;
}

void Mesh::Destroy()
{
	if( m_pMeshMaterials != NULL ) {
        delete[] m_pMeshMaterials;
		m_pMeshMaterials = NULL;
	}

    if( m_pMeshTextures )
    {
        for( DWORD i = 0; i < m_dwNumMaterials; i++ )
        {
            if( m_pMeshTextures[i] ){
                m_pMeshTextures[i]->Release();
				m_pMeshTextures[i] = NULL;
			}
        }
        delete[] m_pMeshTextures;
		m_pMeshTextures = NULL;

    }
    if( m_pMesh != NULL ) {
        m_pMesh->Release();
		m_pMesh = NULL;
	}
}

void Mesh::draw()
{
	// Meshes are divided into subsets, one for each material. Render them in
    // a loop
    for( DWORD i = 0; i < m_dwNumMaterials; i++ )
    {
        // Set the material and texture for this subset
        Gbls::pd3dDevice->SetMaterial( &m_pMeshMaterials[i] );
        Gbls::pd3dDevice->SetTexture( 0, m_pMeshTextures[i] );

        // Draw the mesh subset
        m_pMesh->DrawSubset( i );
    }
}

void Mesh::setScaleRotate(float scaleFactor, float degX, float degY, float degZ)
{
	D3DXMATRIX matScale, matRotX, matRotY, matRotZ;
	D3DXMatrixScaling(&matScale, scaleFactor, scaleFactor, scaleFactor);
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(degX));
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(degY));
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(degZ));
	m_matInitScaleRot = matScale * matRotX * matRotY * matRotZ;
}