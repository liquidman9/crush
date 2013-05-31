//=================================================================================================
// SPRITE.h - Sprite handling class
//=================================================================================================

#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

// Defines
#define WIN32_LEAN_AND_MEAN

// Global includes
#include <d3dx9.h>
//#include <string>

class Sprite
{
public:
	D3DXVECTOR3 /*m_vPos,*/ m_vCenter;
	LPDIRECT3DTEXTURE9 m_pTexture;

	Sprite();
	~Sprite();
	void setTexture(LPDIRECT3DTEXTURE9 pTexture);
	void setCenterToTextureMidpoint();
	void setCenterToTopRight();
	//void Sprite::draw(ID3DXSprite* pRenderer);
};



#endif // SPRITE_H_INCLUDED
