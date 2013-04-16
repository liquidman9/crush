/*
 * Renderable.h
 */

#ifndef RENDERABLE_H_INCLUDED
#define RENDERABLE_H_INCLUDED

// External includes
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

// Project includes
#include <client/graphics/Mesh.h>

class RenderableEntity {
public:
	//Mesh* m_mesh; give each subclass a static mesh or mesh pointer

	virtual void draw() = 0;
};


#endif