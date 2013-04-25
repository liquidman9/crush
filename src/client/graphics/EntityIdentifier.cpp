
// Project includes
#include <client/graphics/EntityIdentifier.h>
#include <client/Gbls.h>
#include <client/graphics/Camera.h>

EntityIdentifier::EntityIdentifier() :
	targetEntity(NULL)
{
	D3DXMatrixIdentity(&m_scaleOffsetMatrix);
}

EntityIdentifier::~EntityIdentifier()
{
}

void EntityIdentifier::draw(Camera * cam, ID3DXSprite* pSpriteRenderer)
{
	if (targetEntity) {
		D3DXMATRIX entityTranslate, viewMatrix;
		D3DXMatrixTranslation(&entityTranslate, targetEntity->m_pos.x, targetEntity->m_pos.y, targetEntity->m_pos.z);
		pSpriteRenderer->SetWorldViewLH(&(m_scaleOffsetMatrix*entityTranslate), cam->getViewMatrix(viewMatrix));
		HRESULT hResult = pSpriteRenderer->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_BILLBOARD);
		if(SUCCEEDED(hResult)) {
			m_sprite.draw(pSpriteRenderer);
			pSpriteRenderer->End();
		}


		//test
		static int y = 0;
		D3DXVECTOR3 out;
		D3DXMATRIX mat, tmp;
		D3DXMatrixIdentity(&mat);
		//mat = mat * entityTranslate;
		mat = mat * *cam->getViewMatrix(tmp);
		mat = mat * *cam->getProjMatrix(tmp);
		D3DXVec3TransformCoord(&out, &targetEntity->m_pos, &mat);
		int pixel_x = ((out.x + 1.0f)/2.0f)*Gbls::windowWidth;
		int pixel_y = (-(out.y - 1.0f)/2.0f)*Gbls::windowHeight;
		y++;
	}
}

void EntityIdentifier::setScaleOffset(float scaleFactor, float x, float y, float z)
{
	D3DXMATRIX matScale, matTranslate;
	D3DXMatrixScaling(&matScale, scaleFactor, scaleFactor, scaleFactor);
	D3DXMatrixTranslation(&matTranslate, x, y, z);
	m_scaleOffsetMatrix = matScale * matTranslate;
}