/*
 * Renderable.h
 */

#ifndef RENDERABLE_H_INCLUDED
#define RENDERABLE_H_INCLUDED

// Global includes
//#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

class Renderable {
public:
	virtual void draw() = 0;
};


#endif