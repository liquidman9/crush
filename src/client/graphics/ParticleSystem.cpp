/*
 * ParticleSystem.cpp
 */

// Project includes
#include <client/graphics/ParticleSystem.h>

//-----------------------------------------------------------------------------
// Name: getRandomMinMax()
// Desc: Gets a random number between min/max boundaries
//-----------------------------------------------------------------------------
float ParticleSystem::getRandomMinMax( float fMin, float fMax )
{
    float fRandNum = (float)rand () / RAND_MAX;
    return fMin + (fMax - fMin) * fRandNum;
}

//-----------------------------------------------------------------------------
// Name: getRandomVector()
// Desc: Generates a random vector where X,Y, and Z components are between
//       -1.0 and 1.0
//-----------------------------------------------------------------------------
D3DXVECTOR3 ParticleSystem::getRandomVector( void )
{
	D3DXVECTOR3 vVector;

    // Pick a random Z between -1.0f and 1.0f.
    vVector.z = getRandomMinMax( -1.0f, 1.0f );
    
    // Get radius of this circle
    float radius = (float)sqrt(1 - vVector.z * vVector.z);
    
    // Pick a random point on a circle.
    float t = getRandomMinMax( -D3DX_PI, D3DX_PI );

    // Compute matching X and Y for our Z.
    vVector.x = (float)cosf(t) * radius;
    vVector.y = (float)sinf(t) * radius;

	return vVector;
}

//-----------------------------------------------------------------------------
// Name: CParticleSystem()
// Desc:
//-----------------------------------------------------------------------------
ParticleSystem::ParticleSystem()
{
    m_dwVBOffset       = 0;    // Gives the offset of the vertex buffer chunk that's currently being filled
    m_dwFlush          = 512;  // Number of point sprites to load before sending them to hardware(512 = 2048 divided into 4 chunks)
    m_dwDiscard        = 2048; // Max number of point sprites the vertex buffer can load until we are forced to discard and start over
    m_pActiveList      = NULL; // Head node of point sprites that are active
    m_pFreeList        = NULL; // Head node of point sprites that are inactive and waiting to be recycled.
	m_dwActiveCount    = 0;
    m_pVB              = NULL; // The vertex buffer where point sprites are to be stored
    m_dwMaxParticles   = 1;
}

//-----------------------------------------------------------------------------
// Name: ~CParticleSystem()
// Desc:
//-----------------------------------------------------------------------------
ParticleSystem::~ParticleSystem()
{
    InvalidateDeviceObjects();

    while( m_pActiveList )
    {
        Particle* pParticle = m_pActiveList;
        m_pActiveList = pParticle->m_pNext;
        delete pParticle;
    }
    m_pActiveList = NULL;

    while( m_pFreeList )
    {
        Particle *pParticle = m_pFreeList;
        m_pFreeList = pParticle->m_pNext;
        delete pParticle;
    }
    m_pFreeList = NULL;
}

HRESULT ParticleSystem::update(ParticleGroup * pGroup, float newTime) {

	pGroup->updateGroup();

    Particle  *pParticle;
    Particle **ppParticle;

	float elapsedTime = newTime - pGroup->m_currentTime;
    pGroup->m_currentTime += newTime;  // Update our particle system timer

    ppParticle = &pGroup->m_partList; // Start at the head of the active list

    while( *ppParticle )
    {
        pParticle = *ppParticle; // Set a pointer to the head
		
        //float fTimePassed  = m_currentTime - pParticle->m_fInitTime;

		/* Update particle position
		 * TBeam particles don't use normal velocity based physics.
		 * They share a constant velocity in the z direction and 
		 * rotation around the z-axis
		 */
		if (pGroup->updateParticle(pParticle, elapsedTime)) { // go to next particle
            ppParticle = &pParticle->m_pNext;
        } else { // Particle no longer valid. Place back on free list
            *ppParticle = pParticle->m_pNext;
            pParticle->m_pNext = m_pFreeList;
            m_pFreeList = pParticle;
        }
    }

    //-------------------------------------------------------------------------
    // Emit new particles in accordance to the flow rate...
    //-------------------------------------------------------------------------

    if( pGroup->m_currentTime - pGroup->m_lastUpdate > pGroup->m_releaseInterval )
    {
        // Reset update timing...
        pGroup->m_lastUpdate = pGroup->m_currentTime;
    
        // Emit new particles at specified flow rate...
        for( DWORD i = 0; i < pGroup->m_numToRelease; ++i )
        {
            // Do we have any free particles to put back to work?
            if( m_pFreeList )
            {
                // If so, hand over the first free one to be reused.
                pParticle = m_pFreeList;
                // Then make the next free particle in the list next to go!
                m_pFreeList = pParticle->m_pNext;
            }
            else
            {
                // There are no free particles to recycle...
                // We'll have to create a new one from scratch!
                if( NULL == ( pParticle = new Particle ) )
                    return E_OUTOFMEMORY;
            }

            pParticle->m_pNext = pGroup->m_partList; // Make it the new head
            pGroup->m_partList = pParticle;

            pParticle->m_fInitTime  = pGroup->m_currentTime;
            pGroup->initNewParticle(pParticle);
        }
    }

    return S_OK;
}

////-----------------------------------------------------------------------------
//// Name: Update()
//// Desc:
////-----------------------------------------------------------------------------
//HRESULT ParticleSystem::Update( FLOAT fElpasedTime )
//{
//    Particle  *pParticle;
//    Particle **ppParticle;
//    Plane     *pPlane;
//    Plane    **ppPlane;
//    D3DXVECTOR3 vOldPosition;
//
//    m_fCurrentTime += fElpasedTime;     // Update our particle system timer...
//
//    ppParticle = &m_pActiveList; // Start at the head of the active list
//
//    while( *ppParticle )
//    {
//        pParticle = *ppParticle; // Set a pointer to the head
//
//        // Calculate new position
//        float fTimePassed  = m_fCurrentTime - pParticle->m_fInitTime;
//
//        if( fTimePassed >= m_fLifeCycle )
//        {
//            // Time is up, put the particle back on the free list...
//            *ppParticle = pParticle->m_pNext;
//            pParticle->m_pNext = m_pFreeList;
//            m_pFreeList = pParticle;
//
//            --m_dwActiveCount;
//        }
//        else
//        {
//            // Update particle position and velocity
//
//            // Update velocity with respect to Gravity (Constant Acceleration)
//            pParticle->m_vCurVel += m_vGravity * fElpasedTime;
//
//            // Update velocity with respect to Wind (Acceleration based on 
//            // difference of vectors)
//            if( m_bAirResistence == true )
//                pParticle->m_vCurVel += (m_vWind - pParticle->m_vCurVel) * fElpasedTime;
//
//            // Finally, update position with respect to velocity
//            vOldPosition = pParticle->m_vCurPos;
//            pParticle->m_vCurPos += pParticle->m_vCurVel * fElpasedTime;
//
//            //-----------------------------------------------------------------
//            // BEGIN Checking the particle against each plane that was set up
//
//            ppPlane = &m_pPlanes; // Set a pointer to the head
//
//            while( *ppPlane )
//            {
//                pPlane = *ppPlane;
//                int result = classifyPoint( &pParticle->m_vCurPos, pPlane );
//
//                if( result == CP_BACK /*|| result == CP_ONPLANE */ )
//                {
//                    if( pPlane->m_nCollisionResult == CR_BOUNCE )
//                    {
//                        pParticle->m_vCurPos = vOldPosition;
//
//            //-----------------------------------------------------------------
//            //
//            // The new velocity vector of a particle that is bouncing off
//            // a plane is computed as follows:
//            //
//            // Vn = (N.V) * N
//            // Vt = V - Vn
//            // Vp = Vt - Kr * Vn
//            //
//            // Where:
//            // 
//            // .  = Dot product operation
//            // N  = The normal of the plane from which we bounced
//            // V  = Velocity vector prior to bounce
//            // Vn = Normal force
//            // Kr = The coefficient of restitution ( Ex. 1 = Full Bounce, 
//            //      0 = Particle Sticks )
//            // Vp = New velocity vector after bounce
//            //
//            //-----------------------------------------------------------------
//
//                        float Kr = pPlane->m_fBounceFactor;
//
//                        D3DXVECTOR3 Vn = D3DXVec3Dot( &pPlane->m_vNormal, 
//                                                      &pParticle->m_vCurVel ) * 
//                                                      pPlane->m_vNormal;
//                        D3DXVECTOR3 Vt = pParticle->m_vCurVel - Vn;
//                        D3DXVECTOR3 Vp = Vt - Kr * Vn;
//
//                        pParticle->m_vCurVel = Vp;
//                    }
//                    else if( pPlane->m_nCollisionResult == CR_RECYCLE )
//                    {
//                        pParticle->m_fInitTime -= m_fLifeCycle;
//                    }
//
//                    else if( pPlane->m_nCollisionResult == CR_STICK )
//                    {
//                        pParticle->m_vCurPos = vOldPosition;
//                        pParticle->m_vCurVel = D3DXVECTOR3(0.0f,0.0f,0.0f);
//                    }
//                }
//
//                ppPlane = &pPlane->m_pNext;
//            }
//
//            // END Plane Checking
//            //-----------------------------------------------------------------
//
//            ppParticle = &pParticle->m_pNext;
//        }
//    }
//
//    //-------------------------------------------------------------------------
//    // Emit new particles in accordance to the flow rate...
//    // 
//    // NOTE: The system operates with a finite number of particles.
//    //       New particles will be created until the max amount has
//    //       been reached, after that, only old particles that have
//    //       been recycled can be reintialized and used again.
//    //-------------------------------------------------------------------------
//
//    if( m_fCurrentTime - m_fLastUpdate > m_fReleaseInterval )
//    {
//        // Reset update timing...
//        m_fLastUpdate = m_fCurrentTime;
//    
//        // Emit new particles at specified flow rate...
//        for( DWORD i = 0; i < m_dwNumToRelease; ++i )
//        {
//            // Do we have any free particles to put back to work?
//            if( m_pFreeList )
//            {
//                // If so, hand over the first free one to be reused.
//                pParticle = m_pFreeList;
//                // Then make the next free particle in the list next to go!
//                m_pFreeList = pParticle->m_pNext;
//            }
//            else
//            {
//                // There are no free particles to recycle...
//                // We'll have to create a new one from scratch!
//                if( m_dwActiveCount < m_dwMaxParticles )
//                {
//                    if( NULL == ( pParticle = new Particle ) )
//                        return E_OUTOFMEMORY;
//                }
//            }
//
//            if( m_dwActiveCount < m_dwMaxParticles )
//            {
//                pParticle->m_pNext = m_pActiveList; // Make it the new head...
//                m_pActiveList = pParticle;
//                
//                // Set the attributes for our new particle...
//                pParticle->m_vCurVel = m_vVelocity;
//
//                if( m_fVelocityVar != 0.0f )
//                {
//                    D3DXVECTOR3 vRandomVec = getRandomVector();
//                    pParticle->m_vCurVel += vRandomVec * m_fVelocityVar;
//                }
//
//                pParticle->m_fInitTime  = m_fCurrentTime;
//                pParticle->m_vCurPos    = m_vPosition;
//                
//                ++m_dwActiveCount;
//            }
//        }
//    }
//
//    return S_OK;
//}

////-----------------------------------------------------------------------------
//// Name: RestartParticleSystem()
//// Desc:
////-----------------------------------------------------------------------------
//void ParticleSystem::RestartParticleSystem( void )
//{
//	Particle  *pParticle;
//	Particle **ppParticle;
//
//	ppParticle = &m_pActiveList; // Start at the head of the active list
//
//	while( *ppParticle )
//	{
//		pParticle = *ppParticle; // Set a pointer to the head
//
//		// Put the particle back on the free list...
//		*ppParticle = pParticle->m_pNext;
//		pParticle->m_pNext = m_pFreeList;
//		m_pFreeList = pParticle;
//
//		--m_dwActiveCount;
//	}
//}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Renders the particle system using pointsprites loaded in a vertex 
//       buffer.
//
// Note: D3DLOCK_DISCARD:
//
//       The application overwrites, with a write-only operation, the entire 
//       index buffer. This enables Direct3D to return a pointer to a new 
//       memory area so that the dynamic memory access (DMA) and rendering 
//       from the old area do not stall.
//
//       D3DLOCK_NOOVERWRITE:
//
//       Indicates that no vertices that were referred to in drawing calls 
//       since the start of the frame or the last lock without this flag will 
//       be modified during the lock. This can enable optimizations when the 
//       application is appending data only to the vertex buffer. 
//-----------------------------------------------------------------------------
HRESULT ParticleSystem::Render( LPDIRECT3DDEVICE9 pd3dDevice, ParticleGroup * pGroup)
{
    HRESULT hr;

	//
    // Set the render states for using point sprites..
	//
	
    pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );       // Turn on point sprites
    pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE );       // Allow sprites to be scaled with distance
	pd3dDevice->SetRenderState( D3DRS_POINTSIZE,     FtoDW(pGroup->m_size) ); // Float value that specifies the size to use for point size computation in cases where point size is not specified for each vertex.
    pd3dDevice->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(1.0f) );    // Float value that specifies the minimum size of point primitives. Point primitives are clamped to this size during rendering. 
    pd3dDevice->SetRenderState( D3DRS_POINTSCALE_A,  FtoDW(0.0f) );    // Default 1.0
    pd3dDevice->SetRenderState( D3DRS_POINTSCALE_B,  FtoDW(0.0f) );    // Default 0.0
    pd3dDevice->SetRenderState( D3DRS_POINTSCALE_C,  FtoDW(1.0f) );    // Default 0.0
	pd3dDevice->SetTransform( D3DTS_WORLD, &pGroup->m_worldTransformMat ); // Set world transform matrix for this particle 

    Particle    *pParticle = m_pActiveList;
    PointVertex *pVertices;
    DWORD        dwNumParticlesToRender = 0;

    // Lock the vertex buffer.  We fill the vertex buffer in small
    // chunks, using D3DLOCK_NOOVERWRITE.  When we are done filling
    // each chunk, we call DrawPrim, and lock the next chunk.  When
    // we run out of space in the vertex buffer, we start over at
    // the beginning, using D3DLOCK_DISCARD.

    // Move the offset forward so we can fill the next chunk of the vertex buffer
    m_dwVBOffset += m_dwFlush;

    // If we're about to overflow the buffer, reset the offset counter back to 0
    if( m_dwVBOffset >= m_dwDiscard )
        m_dwVBOffset = 0;

    if( FAILED( hr = m_pVB->Lock( m_dwVBOffset * sizeof(PointVertex), // Offset to lock
                                  m_dwFlush * sizeof(PointVertex),    // Size to lock
                                  (void**) &pVertices, 
                                  m_dwVBOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD)))
    {
        return hr;
    }

    // Render each particle
    while( pParticle )
    {
        D3DXVECTOR3 vPos(pParticle->m_vCurPos);
        D3DXVECTOR3 vVel(pParticle->m_vCurVel);

        pVertices->posit = vPos;
		pVertices->color = pGroup->m_color;
        pVertices++;

        if( ++dwNumParticlesToRender == m_dwFlush )
        {
            // Done filling this chunk of the vertex buffer.  Lets unlock and
            // draw this portion so we can begin filling the next chunk.

            m_pVB->Unlock();

			pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(PointVertex) );
			pd3dDevice->SetFVF( PointVertex::FVF_Flags );

            if( FAILED(hr = pd3dDevice->DrawPrimitive( D3DPT_POINTLIST, 
                m_dwVBOffset, dwNumParticlesToRender)))
            {
                return hr;
            }

            // Lock the next chunk of the vertex buffer.  If we are at the 
            // end of the vertex buffer, DISCARD the vertex buffer and start
            // at the beginning.  Otherwise, specify NOOVERWRITE, so we can
            // continue filling the VB while the previous chunk is drawing.
            m_dwVBOffset += m_dwFlush;

            // If we're about to overflow the buffer, reset the offset counter back to 0
            if( m_dwVBOffset >= m_dwDiscard )
                m_dwVBOffset = 0;

            if( FAILED( hr = m_pVB->Lock( 
                m_dwVBOffset * sizeof(PointVertex), // Offset to lock
                m_dwFlush    * sizeof(PointVertex), // Size to lock
                (void**) &pVertices, 
                m_dwVBOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD)))
            {
                return hr;
            }

            dwNumParticlesToRender = 0;
        }

        pParticle = pParticle->m_pNext;
    }

    // Unlock the vertex buffer
    m_pVB->Unlock();

    // Render any remaining particles
    if( dwNumParticlesToRender )
    {
		pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(PointVertex) );
		pd3dDevice->SetFVF( PointVertex::FVF_Flags );

        if(FAILED(hr = pd3dDevice->DrawPrimitive( D3DPT_POINTLIST, m_dwVBOffset, 
                                                  dwNumParticlesToRender )))
            return hr;
    }

	//
    // Reset render states...
	//

    pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
    pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc:
// Note: If your application performs its own vertex processing and passes 
//       transformed, lit, and clipped vertices to rendering methods, then the 
//       application can directly write vertices to a vertex buffer allocated  
//       in driver-optimal memory. This technique prevents a redundant copy  
//       operation later. Note that this technique will not work well if your 
//       application reads data back from a vertex buffer, because read 
//       operations done by the host from driver-optimal memory can be very 
//       slow. Therefore, if your application needs to read during processing 
//       or writes data to the buffer erratically, a system-memory vertex 
//       buffer is a better choice.
//
//       When using the Direct3D vertex processing features (by passing 
//       untransformed vertices to vertex-buffer rendering methods), processing 
//       can occur in either hardware or software depending on the device type  
//       and device creation flags. It is recommended that vertex buffers be 
//       allocated in pool D3DPOOL_DEFAULT for best performance in virtually 
//       all cases. When a device is using hardware vertex processing, there 
//       is a number of additional optimizations that may be done based on the 
///      flags  D3DUSAGE_DYNAMIC and D3DUSAGE_WRITEONLY. 
//       See IDirect3DDevice9::CreateVertexBuffer for more information on 
//       using these flags.
//-----------------------------------------------------------------------------
HRESULT ParticleSystem::RestoreDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice )
{
    HRESULT hr;

    // Create a vertex buffer for the particle system.  The size of this buffer
    // does not relate to the number of particles that exist.  Rather, the
    // buffer is used as a communication channel with the device. we fill in 
    // a chunck, and tell the device to draw. While the device is drawing, we
    // fill in the next chunck using NOOVERWRITE. We continue doing this until 
    // we run out of vertex buffer space, and are forced to DISCARD the buffer
    // and start over at the beginning.

    if( FAILED( hr = pd3dDevice->CreateVertexBuffer( 
        m_dwDiscard * sizeof(PointVertex), 
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, 
        PointVertex::FVF_Flags, // Our custom FVF
        D3DPOOL_DEFAULT, 
        &m_pVB, NULL )))
    {
        return E_FAIL;
    }

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT ParticleSystem::InvalidateDeviceObjects()
{
	if( m_pVB != NULL )
        m_pVB->Release();

    return S_OK;
}
