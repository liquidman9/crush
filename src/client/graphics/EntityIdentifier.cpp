
// Project includes
#include <client/graphics/EntityIdentifier.h>
#include <client/Gbls.h>
#include <client/graphics/Camera.h>
#include <math.h>

EntityIdentifier::EntityIdentifier() :
	targetEntity(NULL)
{
	//D3DXMatrixIdentity(&m_scaleOffsetMatrix);
}

EntityIdentifier::~EntityIdentifier()
{
}

void EntityIdentifier::draw(Camera * cam, ID3DXSprite* pSpriteRenderer)
{
	if (targetEntity) {

		D3DXVECTOR3 out;
		D3DXMATRIX mat, tmp;
		D3DXMatrixIdentity(&mat);
		mat = *cam->getProjMatrix(tmp);
		mat = *cam->getViewMatrix(tmp) * mat;  //tmp now has view matrix
		D3DXVec3TransformCoord(&out, &targetEntity->m_pos, &mat);  // out now has projection coords
		int screenWidth;
		int screenHeight;
		if (Gbls::thePresentParams.Windowed) {
			screenWidth = Gbls::windowWidth;
			screenHeight = Gbls::windowHeight;
		} else {
			screenWidth = Gbls::fullScreenWidth;
			screenHeight = Gbls::fullScreenWidth;
		}
		int pixel_x = (int)(((out.x + 1.0f)/2.0f)*screenWidth);
		int pixel_y = (int)((-(out.y - 1.0f)/2.0f)*screenHeight);
		bool infront = out.z > 0.0f && out.z < 1.0f;
		bool inside = out.x > -1.0f && out.x < 1.0f && out.y > -1.0f && out.y < 1.0f;
		if (infront && inside) { // infront && inside fov
			D3DXVec3TransformCoord(&out, &targetEntity->m_pos, &tmp); //out now has view coords
			float length = D3DXVec3Length(&out);
			float scale = 30.0f/length;
			float sizeScale = max(0.35f, min(0.71f, scale));
			float distScale = (sizeScale+scale)/2.0f;
			D3DXVECTOR2 centerV(m_onScreenSprite.m_vCenter.x, m_onScreenSprite.m_vCenter.y);
			D3DXVECTOR2 scaleV(sizeScale, sizeScale);
			D3DXVECTOR2 transV((float)pixel_x, pixel_y-(distScale*100));
			D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &scaleV, &centerV, 0.0f, &transV);
			pSpriteRenderer->SetTransform(&mat);
			pSpriteRenderer->Draw(m_onScreenSprite.m_pTexture, NULL, &m_onScreenSprite.m_vCenter, NULL, 0XFFFFFFFF);
		} else {
			float rotate;
			Sprite * tarSprite;
			D3DXVECTOR2 centerV;
			if (!infront && inside) { // behind && inside fov
				tarSprite = &m_edgeScreenSprite;
				if (abs(out.x) > abs(out.y)) {
					centerV.x = tarSprite->m_vCenter.y; // inverse center x and y for rotated sprite
					centerV.y = tarSprite->m_vCenter.x;
					if (out.x < 0) {

					} else {

					}
				} else {
					if (out.y < 0) {
					centerV.x = tarSprite->m_vCenter.x; // inverse center x and y for rotated sprite
					centerV.y = tarSprite->m_vCenter.y;

					} else {

					}
				}
			} else { // outside fov

			}
		}
	}
}

//void EntityIdentifier::setScaleOffset(float scaleFactor, float x, float y, float z)
//{
//	D3DXMATRIX matScale, matTranslate;
//	D3DXMatrixScaling(&matScale, scaleFactor, scaleFactor, scaleFactor);
//	D3DXMatrixTranslation(&matTranslate, x, y, z);
//	m_scaleOffsetMatrix = matScale * matTranslate;
//}