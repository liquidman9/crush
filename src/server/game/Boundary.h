/*
 * plane.hpp
 */

#ifndef BOUNDARY_H_INCLUDED
#define BOUNDARY_H_INCLUDED

// Global includes
#include <d3dx9.h>

// Project includes
#include <server/game/ServerEntity.h>

class Boundary {
public:
	D3DXVECTOR3 m_normal;
	D3DXVECTOR3 m_point;
	Boundary(D3DXVECTOR3, D3DXVECTOR3);
};


#endif BOUNDARY_H_INCLUDED