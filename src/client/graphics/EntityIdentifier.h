//=================================================================================================
// SPRITE.h - Sprite handling class
//=================================================================================================

#ifndef ENTITY_IDENTIFIER_H_INCLUDED
#define ENTITY_IDENTIFIER_H_INCLUDED

// Defines
#define WIN32_LEAN_AND_MEAN

// Global includes
#include <d3dx9.h>
#include <client/graphics/Sprite.h>
#include <client/graphics/entities/C_Entity.h>
#include <client/graphics/Camera.h>

class EntityIdentifier
{
public:
	D3DXMATRIX m_scaleOffsetMatrix;
	C_Entity * targetEntity;
	EntityIdentifier();
	~EntityIdentifier();
	Sprite m_sprite;
	void setScaleOffset(float scaleFactor, float x, float y, float z);
	void draw(Camera * cam, ID3DXSprite* pSpriteRenderer);
};



#endif // ENTITY_IDENTIFIER_H_INCLUDED
