
// Project includes
#include <client/graphics/Sprite.h>
#include <client/Gbls.h>

Sprite::Sprite() :
/*	m_vPos(0.0f,0.0f,0.0f),*/
	m_vCenter(0.0f,0.0f,0.0f),
	m_pTexture(NULL)
{
}

Sprite::~Sprite() {
	//if(m_pTexture)
	//	m_pTexture->Release();
	m_pTexture = NULL;
}

void Sprite::setTexture(LPDIRECT3DTEXTURE9 pTexture) {
	
	if(pTexture) {
		// Release any existing texture
		//if(m_pTexture)
		//	m_pTexture->Release();
		m_pTexture = pTexture;
		//pTexture->AddRef();
	}
}

void Sprite::setCenterToTextureMidpoint() {
	if (m_pTexture) {
		D3DSURFACE_DESC desc;
		HRESULT hResult = m_pTexture->GetLevelDesc(0, &desc);
		if(SUCCEEDED(hResult))
		{
			m_vCenter.x = (desc.Width / 2.0f);
			m_vCenter.y =  (desc.Height / 2.0f);
		}
	}
}

//void Sprite::draw(ID3DXSprite* pRenderer)
//{
//	pRenderer->Draw(m_pTexture, NULL, &m_vCenter, &m_vPos, D3DCOLOR_XRGB(255, 255, 255));
//}