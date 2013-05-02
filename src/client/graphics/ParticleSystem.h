//=================================================================================================
// ParticleSystem.h - Sprite handling class
//=================================================================================================

#ifndef PARTICLESYSTEM_H_INCLUDED
#define PARTICLESYSTEM_H_INCLUDED

// Project includes
#include <client/graphics/Particle.h>
#include <client/graphics/ParticleGroup.h>

//-----------------------------------------------------------------------------
// CLASSES
//-----------------------------------------------------------------------------

class ParticleSystem
{

public:
	// Custom vertex and FVF declaration for point sprite vertex points
	struct PointVertex
	{
		D3DXVECTOR3 posit;
		D3DCOLOR    color;

		enum FVF
		{
			FVF_Flags = D3DFVF_XYZ | D3DFVF_DIFFUSE
		};
	};

	// Helper function to stuff a FLOAT into a DWORD argument
	inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }

    ParticleSystem(void);
   ~ParticleSystem(void);

	HRESULT init( LPDIRECT3DDEVICE9 pd3dDevice );

    HRESULT render(LPDIRECT3DDEVICE9 pd3dDevice, ParticleGroup * pGroup);

    HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
    HRESULT InvalidateDeviceObjects(void);
    
	//void RestartParticleSystem(void);

	HRESULT update(ParticleGroup * pGroup, float newTime);

	static float getRandomMinMax( float fMin, float fMax );
	static D3DXVECTOR3 getRandomVector( void );
	

private:

    DWORD       m_dwVBOffset;
    DWORD       m_dwFlush;
    DWORD       m_dwDiscard;
    Particle   *m_pFreeList;

    //float       m_fMaxPointSize;

    LPDIRECT3DVERTEXBUFFER9 m_pVB;          // Vertex buffer for point sprites
};



#endif // PARTICLESYSTEM_H_INCLUDED
