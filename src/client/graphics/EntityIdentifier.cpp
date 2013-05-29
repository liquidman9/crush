
// Project includes
#include <client/graphics/EntityIdentifier.h>
#include <client/Gbls.h>
#include <client/graphics/Camera.h>
#include <math.h>

EntityIdentifier::EntityIdentifier() :
	targetEntity(NULL),
	rotateOn(TRUE)
{
	//D3DXMatrixIdentity(&m_scaleOffsetMatrix);
}

EntityIdentifier::~EntityIdentifier()
{
}

void EntityIdentifier::draw(Camera * cam, ID3DXSprite* pSpriteRenderer)
{
	if (targetEntity && targetEntity->m_enableIdentifiers) {

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
			screenHeight = Gbls::fullScreenHeight;
		}
		float scaleMod = sqrt((float)(screenWidth*screenHeight)/(float)(1280*720));
		scaleMod = ((scaleMod - 1)/2)+1; // SO MANY MAGIC NUMBERS
		int pixel_x = (int)(((out.x + 1.0f)/2.0f)*screenWidth);
		int pixel_y = (int)((-(out.y - 1.0f)/2.0f)*screenHeight);
		bool inside = out.x > -1.0f && out.x < 1.0f && out.y > -1.0f && out.y < 1.0f;
		D3DXVec3TransformCoord(&out, &targetEntity->m_pos, &tmp); //out now has view coords
		bool infront = out.z > 0.0f /* && out.z < 1.0f*/;
		if (infront && inside) { // infront && inside fov
			float length = D3DXVec3Length(&out);
			float scale = (30.0f/length)*scaleMod;
			float sizeScale = max(0.35f, min(0.71f, scale))*scaleMod;
			float distScale = ((sizeScale+scale)/2.0f)*scaleMod;
			D3DXVECTOR2 centerV(m_onScreenSprite.m_vCenter.x, m_onScreenSprite.m_vCenter.y);
			D3DXVECTOR2 scaleV(sizeScale, sizeScale);
			D3DXVECTOR2 transV((float)pixel_x, pixel_y-(distScale*100));
			D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &scaleV, &centerV, 0.0f, &transV);
			pSpriteRenderer->SetTransform(&mat);
			pSpriteRenderer->Draw(m_onScreenSprite.m_pTexture, NULL, &m_onScreenSprite.m_vCenter, NULL, 0XFFFFFFFF);
		} else {
			if (screenHeight > screenWidth) {
				float ratio = (float)screenWidth/(float)screenHeight;
				out.y *= ratio;
			} else {
				float ratio = (float)screenHeight/(float)screenWidth;
				out.x *= ratio;
			}
			D3DXVECTOR2 arrowV(out.x, out.y);
			D3DXVec2Normalize(&arrowV, &arrowV);

			float angle = 0.0f;
			if (rotateOn) {
				angle = atan(arrowV.y/arrowV.x);
				// if(arrowV.x >= 0 && arrowV.y >= 0) first quardrent, do nothing, angle is correct
				if(arrowV.x >= 0 && arrowV.y < 0) // fourth quadrent
					angle = D3DX_PI*2.0f + angle;
				if(arrowV.x < 0) // second and third quadrent
					angle = D3DX_PI + angle;
				angle = (D3DX_PI*2.0f-angle)-(D3DX_PI/2.0f);
			}

			D3DXVECTOR2 centerV(m_offScreenSprite.m_vCenter.x, m_offScreenSprite.m_vCenter.y);
			
			float scaleFactor = scaleMod*0.5f;

			arrowV *= 1.25f;
			pixel_x = (int)(((arrowV.x + 1.0f)/2.0f)*screenWidth);
			pixel_x = (int)(max(centerV.x*scaleFactor, min(screenWidth - centerV.x*scaleFactor, pixel_x)));
			pixel_y = (int)((-(arrowV.y - 1.0f)/2.0f)*screenHeight);
			pixel_y = (int)(max(centerV.y*scaleFactor, min(screenHeight - centerV.y*scaleFactor, pixel_y)));
			
			D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &D3DXVECTOR2(scaleFactor, scaleFactor), &centerV, angle, NULL);
			mat._41 = (float)pixel_x;
			mat._42 = (float)pixel_y;
			pSpriteRenderer->SetTransform(&mat);
			pSpriteRenderer->Draw(m_offScreenSprite.m_pTexture, NULL, &D3DXVECTOR3(centerV.x, centerV.y, 0), NULL, 0XFFFFFFFF);
		}
	}
}