
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
			screenHeight = Gbls::fullScreenHeight;
		}
		float scaleMod = sqrt((screenWidth*screenHeight)/(1280*720));
		scaleMod = ((scaleMod - 1)/2)+1; // SO MANY MAGIC NUMBERS
		int pixel_x = (int)(((out.x + 1.0f)/2.0f)*screenWidth);
		int pixel_y = (int)((-(out.y - 1.0f)/2.0f)*screenHeight);
		bool infront = out.z > 0.0f && out.z < 1.0f;
		bool inside = out.x > -1.0f && out.x < 1.0f && out.y > -1.0f && out.y < 1.0f;
		if (infront && inside) { // infront && inside fov
			D3DXVec3TransformCoord(&out, &targetEntity->m_pos, &tmp); //out now has view coords
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
			// TODO move this code duped line to outer if_else if we use this
			D3DXVec3TransformCoord(&out, &targetEntity->m_pos, &tmp); //out now has view coords
			D3DXVECTOR2 arrowV(out.x, out.y);
			D3DXVec2Normalize(&arrowV, &arrowV);

			float angle = atan(arrowV.y/arrowV.x);
			// if(arrowV.x >= 0 && arrowV.y >= 0) do nothing, angle is correct
			if(arrowV.x >= 0 && arrowV.y < 0)
				angle = D3DX_PI*2.0f + angle;
			if(arrowV.x < 0) /*&& arrowV.y >= 0)
				angle = D3DX_PI + angle;
			if(arrowV.x < 0 && arrowV.y < 0)*/
				angle = D3DX_PI + angle;

			D3DXVECTOR2 centerV(m_onScreenSprite.m_vCenter.x, m_onScreenSprite.m_vCenter.y);
			
			arrowV *= 1.25f;
			pixel_x = (int)(((arrowV.x + 1.0f)/2.0f)*screenWidth);
			pixel_x = max(centerV.x, min(screenWidth - centerV.x, pixel_x));
			pixel_y = (int)((-(arrowV.y - 1.0f)/2.0f)*screenHeight);
			pixel_y = max(centerV.y, min(screenHeight - centerV.y, pixel_y));
			
			D3DXMatrixTransformation2D(&mat, NULL, 0.0f, NULL, &centerV, (D3DX_PI*2.0f-angle)-(D3DX_PI/2.0f), NULL);
			mat._41 = pixel_x;
			mat._42 = pixel_y;
			pSpriteRenderer->SetTransform(&mat);
			pSpriteRenderer->Draw(m_onScreenSprite.m_pTexture, NULL, &D3DXVECTOR3(centerV.x, centerV.y, 0), NULL, 0XFFFFFFFF);



			/*START - ugh, looks like shit, scrap for now */
			//float rotate;
			//Sprite * tarSprite;
			//D3DXVECTOR2 centerV;
			//D3DXVECTOR2 transV;
			//if (!infront && inside) { // behind && inside fov
			//	tarSprite = &m_edgeScreenSprite;
			//	if (abs(out.x) > abs(out.y)) { //pointer on side edge
			//		centerV.x = tarSprite->m_vCenter.y; // inverse center x and y for rotated sprite
			//		centerV.y = tarSprite->m_vCenter.x;
			//		if (out.x < 0) { //pointer on right edge
			//			rotate = D3DXToRadian(270);
			//			transV.x = screenWidth - centerV.x;
			//		} else {  // pointer on left edge
			//			rotate = D3DXToRadian(90);
			//			transV.x = centerV.x;
			//		}
			//		transV.y = (float)pixel_y;
			//	} else {  //pointer on bottom or top edge
			//		centerV.x = tarSprite->m_vCenter.x; // normal center for sprite
			//		centerV.y = tarSprite->m_vCenter.y;
			//		if (out.y < 0) { // pointer on top edge
			//			rotate = D3DXToRadian(180);
			//			transV.y = centerV.y;
			//		} else {  // pointer on bottom edge
			//			rotate = 0.0f;
			//			transV.y = screenHeight - centerV.y;
			//		}
			//		transV.x = (float)pixel_x;
			//	}
			//} else { // outside fov
			//	if (abs(out.y) < 1.0) { // pointer on side edge
			//		tarSprite = &m_edgeScreenSprite;
			//		centerV.x = tarSprite->m_vCenter.y; // inverse center x and y for rotated sprite
			//		centerV.y = tarSprite->m_vCenter.x;
			//		if ((out.x < 1.0 && !infront) || (out.x > 1.0 && infront)) { // pointer on right edge
			//			rotate = D3DXToRadian(270);
			//			transV.x = screenWidth - centerV.x;
			//		} else { // pointer on left edge
			//			rotate = D3DXToRadian(90);
			//			transV.x = centerV.x;
			//		}
			//		transV.y = (float)pixel_y;
			//	} else if (abs(out.x) < 1.0) { // pointer on top or bottom edge
			//		tarSprite = &m_edgeScreenSprite;
			//		centerV.x = tarSprite->m_vCenter.x; // normal center for sprite
			//		centerV.y = tarSprite->m_vCenter.y;
			//		if((out.y < 1.0 && !infront) || (out.y > 1.0 && infront)) { // pointer on top edge
			//			rotate = D3DXToRadian(180);
			//			transV.y = centerV.y;
			//		} else { // pointer on bototm edge
			//			rotate = 0.0f;
			//			transV.y = screenHeight - centerV.y;
			//		}
			//		transV.x = (float)pixel_x;
			//	} else { // pointer in corner
			//		return; //todo remove
			//	}
			//}			D3DXMatrixTransformation2D(&mat, NULL, 0.0f, NULL, &centerV, rotate, NULL);
			//mat._41 = transV.x;
			//mat._42 = transV.y;
			//pSpriteRenderer->SetTransform(&mat);
			//pSpriteRenderer->Draw(tarSprite->m_pTexture, NULL, &D3DXVECTOR3(centerV.x, centerV.y, 0), NULL, 0XFFFFFFFF);
			/*END - ugh, looks like shit, scrap for now */
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