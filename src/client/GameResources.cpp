//=================================================================================================
// GameResources.cpp Static container for game resources including meshes, objects, cameras, etc.
//=================================================================================================


// Global includes
#include <algorithm>
#include <typeinfo>

// Project includes
#include <shared/game/Entity.h>
#include <shared/game/Ship.h>

#include <client/GameResources.h>
#include <client/Gbls.h>
#include <client/graphics/Skybox.h>
#include <client/graphics/entities/C_Resource.h>
#include <client/graphics/entities/C_TractorBeam.h>
#include <client/graphics/entities/C_Mothership.h>
#include <client/graphics/entities/C_Asteroid.h>
#include <client/graphics/entities/C_Ship.h>
#include <client/graphics/entities/C_Entity.h>

//static member initializations
//static enum cameras {DEBUG_CAM, PLAYER_CAM};  //better to use boolean with only two cameras, can extend later

const float GameResources::PLAYER_CAM_DISTANCE = 15.0f;
const float GameResources::PLAYER_CAM_HEIGHT = 4.0f;
const float GameResources::PLAYER_CAM_LOOKAT_DISTANCE = 15.0f;

int GameResources::playerNum = -1;
vector<C_Ship*> GameResources::shipList;
vector<C_Mothership*> GameResources::mothershipList;
vector<C_TractorBeam*> GameResources::tractorBeamList;
vector<C_Resource*> GameResources::resourceList;
vector<C_Asteroid*> GameResources::asteroidList;
vector<EntityIdentifier*> GameResources::eIDList;
vector<EnginePGroup *> GameResources::enginePGroupList;
std::map<int, C_Entity*> GameResources::entityMap;
bool GameResources::debugCamOn = true;
Camera GameResources::debugCam;
Camera GameResources::playerCam;
Camera* GameResources::curCam = &debugCam;
float GameResources::playerCamScale = 1.0f;
int GameResources::playerCamScaleLevel = 0;
C_Ship* GameResources::playerShip = NULL;
LPD3DXSPRITE GameResources::pd3dSprite = NULL;
LPD3DXFONT GameResources::pd3dFont = NULL;
LPDIRECT3DTEXTURE9 GameResources::shipEIDTexture = NULL;
LPDIRECT3DTEXTURE9 GameResources::tBeamPartTexture = NULL;
LPDIRECT3DTEXTURE9 GameResources::EnginePartTexture = NULL;
ParticleSystem * GameResources::partSystem = NULL;
TBeamPGroup * GameResources::tBeamPGroup = NULL;

// for debugging collisions
LPD3DXMESH GameResources::collisionSphere = NULL;
LPD3DXMESH GameResources::collisionCylinder = NULL;
static const D3DXCOLOR WHITE( D3DCOLOR_XRGB(255, 255, 255) );
static const D3DXCOLOR BLACK( D3DCOLOR_XRGB(0, 0, 0) );
static const D3DMATERIAL9 MATERIAL_WHITE = {WHITE, WHITE, WHITE, BLACK, 2.0f};
//std::vector<R_Ship*> GameResources::r_ShipList;
//std::vector<Entity*> GameResources::entityList;
//std::vector<std::vector<Renderable*>*> GameResources::renderList;
struct GameResources::KeyboardState GameResources::m_ks;
std::wstring GameResources::timeStr;
std::wstring GameResources::playerNameStr[4];
int GameResources::playerScore[4];

HRESULT GameResources::initState() {
	HRESULT hres;
	
	curCam = &debugCam;

	// Initialize the skybox
	hres = Skybox::initSkybox();
	if(FAILED (hres))
		return hres;
	
	// create meshes
	hres = initMeshes();
	if(FAILED (hres))
		return hres;

	// load in extra textures
	hres = initAdditionalTextures();
	if(FAILED (hres))
		return hres;

	// create sprite renderer
	hres = D3DXCreateSprite(Gbls::pd3dDevice, &pd3dSprite);
	if (FAILED(hres)) {
		MessageBox( NULL, L"Sprite Renderer Creation Failed", L"CRUSH.exe", MB_OK );
		return hres;
	}

	// create font(s)
	hres = loadFont(&pd3dFont, Gbls::fontHeight, Gbls::fontStyle);
	if (FAILED(hres)) {
		MessageBox( NULL, L"Sprite Renderer Creation Failed", L"CRUSH.exe", MB_OK );
		return hres;
	}

	// create particle system
	partSystem = new ParticleSystem();
	tBeamPGroup = new TBeamPGroup(tBeamPartTexture);
	tBeamPGroup->initBeamToFull();

	// Clear keyboard state (at the moment only used for debug camera 4/13/2013)
	memset(&GameResources::m_ks, 0, sizeof(GameResources::KeyboardState));

	// Init state that must be init every time device is reset
	reInitState();

	/*set up temp entities for test rendering TODO remove this and replace with normal object creation from network*/
#ifdef MYNETWORKOFF  //defined in Gbls

	D3DXVECTOR3 pos(0.0f, -1.0f, 10.0f);
	//D3DXVECTOR3 dir(0.0f, 1.0f, 1.0f);
	int pNum = 1;
	playerNum = pNum; // used for testing player cam
	bool tBeamOn = false;
	Ship * stmp = new Ship(pNum);
	stmp->m_pos = pos;
	D3DXQuaternionRotationAxis(&stmp->m_orientation, &D3DXVECTOR3(0, 1, 1), D3DXToRadian(45));
	//Ship * stmp = new Ship(pos, dir, pNum, tBeamOn);
	C_Entity * etmp = createEntity(stmp);
	entityMap[etmp->getID()] = etmp;

#endif
	/*end TODO remove*/

	return S_OK;
}

void GameResources::releaseResources() {
	
	if(tBeamPGroup) {
		delete tBeamPGroup;
		tBeamPGroup = NULL;
	}

	if(partSystem) {
		delete partSystem;
		partSystem = NULL;
	}

	if(pd3dFont) {
		pd3dFont->Release();
		pd3dFont = NULL;
	}

	if(pd3dSprite) {
		pd3dSprite->Release();
		pd3dSprite = NULL;
	}

	releaseAdditionalTextures();
	
	// release collision shape meshes
	if( collisionCylinder != NULL ) {
        collisionCylinder->Release();
		collisionCylinder = NULL;
	}
	if( collisionSphere != NULL ) {
        collisionSphere->Release();
		collisionSphere = NULL;
	}

	//Gbls::tractorBeamMesh.Destroy();
	Gbls::resourceMesh.Destroy();
	Gbls::asteroidMesh.Destroy();

	for(int i = 0; i < Gbls::numShipMeshes; i++) {
		Gbls::mothershipMesh[0].Destroy();
	}

	for(int i = 0; i < Gbls::numShipMeshes; i++) {
		Gbls::shipMesh[0].Destroy();
	}


	Skybox::releaseSkybox();
}

HRESULT GameResources::reInitState() {
	HRESULT hres;
	
	curCam->updateProjection();

	curCam->updateView();

	// Tell the device to automatically normalize surface normals to keep them normal after scaling
	Gbls::pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// Create lights for scene and set light properties
	hres = GameResources::initLights();
	if(FAILED (hres))
		return hres;

	// init particle system vertex buffer
	partSystem->init(Gbls::pd3dDevice);

	//set backface cullling off TODO remove after models are fixed
	Gbls::pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	
	return S_OK;
}

HRESULT GameResources::initMeshes()
{
	HRESULT hres;

	//do for all needed meshes
	for (int i = 0; i < Gbls::numShipMeshes; i++) {
		if(FAILED(hres = Gbls::shipMesh[i].Create(Gbls::shipMeshFilepath[i])))
			return hres;
	}

	for (int i = 0; i < Gbls::numShipMeshes; i++) {
		if(FAILED(hres = Gbls::mothershipMesh[i].Create(Gbls::mothershipMeshFilepath[i])))
			return hres;
	}

	if(FAILED(hres = Gbls::asteroidMesh.Create(Gbls::asteroidMeshFilepath)))
			return hres;
	if(FAILED(hres = Gbls::resourceMesh.Create(Gbls::resourceMeshFilepath)))
			return hres;
	//if(FAILED(hres = Gbls::tractorBeamMesh.Create(Gbls::tractorBeamMeshFilepath)))
	//		return hres;

	// for debugging collisions:

	if (FAILED(hres = D3DXCreateSphere(Gbls::pd3dDevice, 1.0f, 10, 10, &collisionSphere, NULL)))
		return hres;
	if (FAILED(hres = D3DXCreateCylinder(Gbls::pd3dDevice, 1.0f, 1.0f, 1.0f, 20, 10, &collisionCylinder, NULL)))
		return hres;

	return S_OK;
}

HRESULT GameResources::loadTexture(LPDIRECT3DTEXTURE9 * pTextureOut, std::wstring filepath) {
	HRESULT hres = D3DXCreateTextureFromFileEx(Gbls::pd3dDevice, filepath.c_str(), D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0, NULL, NULL, pTextureOut);
	if (FAILED(hres)) {
		MessageBox( NULL, (L"Could not find "+filepath).c_str(), L"CRUSH.exe", MB_OK );
        return hres;
	}
	return S_OK;
}

HRESULT GameResources::initAdditionalTextures()
{
	HRESULT hres;

	// load arrow spirte
	hres = loadTexture(&shipEIDTexture, Gbls::shipEIDTextureFilepath);
	if (FAILED(hres)) {
		return hres;
	}

	// load tractor beam particle spirte
	hres = loadTexture(&tBeamPartTexture, Gbls::tBeamPartTexFilepath);
	if (FAILED(hres)) {
		return hres;
	}

	// load engine particle spirte
	hres = loadTexture(&EnginePartTexture, Gbls::enginePartTexFilepath);
	if (FAILED(hres)) {
		return hres;
	}

	return S_OK;
}

void GameResources::releaseAdditionalTextures() {
	if (tBeamPartTexture) {
		tBeamPartTexture->Release();
		tBeamPartTexture = NULL;
	}
	if (shipEIDTexture) {
		shipEIDTexture->Release();
		shipEIDTexture = NULL;
	}
}

HRESULT GameResources::initLights() {
	// Tell the device not to do any dynamic lighting
	//Gbls::pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// turn on specular highlights
	Gbls::pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	// ambient light
	Gbls::pd3dDevice->SetRenderState(D3DRS_AMBIENT, Gbls::lightAmbient);

	// directional light
    D3DLIGHT9 light;    // create the light struct

    ZeroMemory(&light, sizeof(light));         // clear out the light struct for use
    light.Type = D3DLIGHT_DIRECTIONAL;         // make the light type 'directional light'
    light.Diffuse = Gbls::lightDiffuseColor;   // set the light's diffuse color
	light.Specular = Gbls::lightSpecularColor; // set the light's specular color
    light.Direction = Gbls::lightDirection;    // set the light's direction
    //D3DXVec3Normalize((D3DXVECTOR3*) &light.Direction, &D3DXVECTOR3(-1.0f, -0.3f, -1.0f));
	light.Range = 1000;
	light.Falloff = 0;
	light.Attenuation0 = 1;
	light.Attenuation1 = 0;
	light.Attenuation2 = 0;

    Gbls::pd3dDevice->SetLight(0, &light);    // send the light struct properties to light #0
    Gbls::pd3dDevice->LightEnable(0, TRUE);    // turn on light #0
	
	return S_OK;
}

HRESULT GameResources::loadFont(LPD3DXFONT * pFont, int height, std::wstring fontStyle) {
	HRESULT hres = D3DXCreateFont(Gbls::pd3dDevice, height, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontStyle.c_str(),
		pFont);
	if(FAILED(hres)) {
		MessageBox( NULL, (L"Could not load font: " + fontStyle).c_str(), L"CRUSH.exe", MB_OK );
		return hres;
	}
	return S_OK;
}

void GameResources::drawCollisionBounds(D3DXVECTOR3 & pt1, D3DXVECTOR3 & pt2, float radius) {
	Gbls::pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	Gbls::pd3dDevice->SetMaterial(&MATERIAL_WHITE);

	D3DXVECTOR3 shapeVec = pt2 - pt1;
	D3DXVECTOR3 midpoint = (pt2 + pt1)/2.0f;
	D3DXMATRIX scaleMat, rotMat, transMat;
	//create scale matrix
	D3DXMatrixScaling(&scaleMat, radius, radius, D3DXVec3Length(&shapeVec));
	//rotate to direction
	D3DXMatrixIdentity(&rotMat);
	if (!(shapeVec.x == 0 && shapeVec.y == 0)) { // only do if new vec isn't z-axis vec
		D3DXVec3Normalize(&shapeVec, &shapeVec);
		D3DXVECTOR3 right;
		D3DXVECTOR3 up(0,1.0f,0);
		D3DXVec3Cross(&right, &up, &shapeVec);
		D3DXVec3Normalize(&right, &right);
		D3DXVec3Cross(&up, &shapeVec, &right);
		rotMat._11 = right.x;   rotMat._12 = right.y;   rotMat._13 = right.z;
		rotMat._21 = up.x;      rotMat._22 = up.y;      rotMat._23 = up.z;
		rotMat._31 = shapeVec.x; rotMat._32 = shapeVec.y; rotMat._33 = shapeVec.z;
	}
	//translate to tBeamEnt->m_start
	D3DXMatrixTranslation(&transMat, midpoint.x, midpoint.y, midpoint.z);
	Gbls::pd3dDevice->SetTransform(D3DTS_WORLD, &(scaleMat*rotMat*transMat));
	collisionCylinder->DrawSubset(0);
	D3DXMatrixScaling(&scaleMat, radius, radius, radius);
	D3DXMatrixTranslation(&transMat, pt1.x, pt1.y, pt1.z);
	Gbls::pd3dDevice->SetTransform(D3DTS_WORLD, &(scaleMat*rotMat*transMat));
	collisionSphere->DrawSubset(0);
	D3DXMatrixTranslation(&transMat, pt2.x, pt2.y, pt2.z);
	Gbls::pd3dDevice->SetTransform(D3DTS_WORLD, &(scaleMat*rotMat*transMat));
	collisionSphere->DrawSubset(0);

	Gbls::pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
}

void GameResources::drawAllTractorBeams() {
		// Set state for particle rendering
	Gbls::pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	Gbls::pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    Gbls::pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    Gbls::pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	// render particles
	for (UINT i = 0; i < tractorBeamList.size(); i++) {
		if(tractorBeamList[i]->m_isOn) {
			tBeamPGroup->tBeamEnt = tractorBeamList[i];
			tBeamPGroup->updateGroup();
			partSystem->render(Gbls::pd3dDevice, tBeamPGroup);
		}
	}

	// Reset state after particle rendering
    Gbls::pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	Gbls::pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    Gbls::pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

void GameResources::drawAllEngines() {
	// Set state for particle rendering
	Gbls::pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	Gbls::pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    Gbls::pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    Gbls::pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	// render particles
	for (UINT i = 0; i < enginePGroupList.size(); i++) {
		partSystem->render(Gbls::pd3dDevice, enginePGroupList[i]);
	}

	// Reset state after particle rendering
    Gbls::pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	Gbls::pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    Gbls::pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

}

void GameResources::drawAllEID() {

	HRESULT hResult = pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	if(SUCCEEDED(hResult)) {
		for (UINT i = 0; i < eIDList.size(); i++) {
			//TODO add this back in when ID's work correctly
			if(debugCamOn || eIDList[i]->targetEntity != playerShip) {
				eIDList[i]->draw(curCam, pd3dSprite);
			}
		}
		pd3dSprite->End();
	}

}

void GameResources::placeTextCenterCeiling(LPCWSTR str, UINT x) {
	D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);
		
		//std::wstring timeStr = L"Time goes here!!!";
		//int pixel_x = Gbls::thePresentParams.BackBufferWidth/2;
		pd3dSprite->SetTransform(&mat);
		RECT rect = {0, 0, 0, 0};
		pd3dFont->DrawText(pd3dSprite, str, -1, &rect, DT_CALCRECT,
			NULL);
		//rect.top += pixel_y - (rect.bottom/2);
		//rect.bottom += pixel_y - (rect.bottom/2);
		rect.left += x - (rect.right/2);
		rect.right += x - (rect.right/2);
		pd3dFont->DrawText(pd3dSprite, str, -1, &rect, DT_CENTER | DT_WORDBREAK,
			D3DCOLOR_XRGB(255, 255, 255));
}

void GameResources::placeTextCenterFloor(LPCWSTR str, UINT x) {
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);
		
		//std::wstring timeStr = L"Time goes here!!!";
		//int pixel_x = Gbls::thePresentParams.BackBufferWidth/2;
		pd3dSprite->SetTransform(&mat);
		RECT rect = {0, 0, 0, 0};
		pd3dFont->DrawText(pd3dSprite, str, -1, &rect, DT_CALCRECT,
			NULL);
		rect.top += Gbls::thePresentParams.BackBufferHeight - rect.bottom;
		rect.bottom = Gbls::thePresentParams.BackBufferHeight;
		rect.left += x - (rect.right/2);
		rect.right += x - (rect.right/2);
		pd3dFont->DrawText(pd3dSprite, str, -1, &rect, DT_CENTER | DT_WORDBREAK,
			D3DCOLOR_XRGB(255, 255, 255));
}

void GameResources::drawStaticHudElements() {
	HRESULT hres = pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	if(SUCCEEDED(hres))
	{
		placeTextCenterCeiling(timeStr.c_str(), Gbls::thePresentParams.BackBufferWidth/2);
		placeTextCenterFloor((L"Player 1\n" + std::to_wstring((long long)playerScore[0])).c_str(), Gbls::thePresentParams.BackBufferWidth * (1.0f/9.0f));
		placeTextCenterFloor((L"Player 2\n" + std::to_wstring((long long)playerScore[1])).c_str(), Gbls::thePresentParams.BackBufferWidth * (3.0f/9.0f));
		placeTextCenterFloor((L"Player 3\n" + std::to_wstring((long long)playerScore[2])).c_str(), Gbls::thePresentParams.BackBufferWidth * (6.0f/9.0f));
		placeTextCenterFloor((L"Player 4\n" + std::to_wstring((long long)playerScore[3])).c_str(), Gbls::thePresentParams.BackBufferWidth * (8.0f/9.0f));
		// End sprite rendering
		pd3dSprite->End();
	}
}

void GameResources::drawAll()
{
	Skybox::drawSkybox();

	// TODO this draws objects in no particular order, resulting in many loads and unloads (probably) for textures and models. Should be fixed if performance becomes an issue.
	// Loop through all lists. Set up shaders, etc, as needed for each.
	for( map<int,C_Entity*>::iterator ii=entityMap.begin(); ii!=entityMap.end(); ++ii)
    {
		(*ii).second->draw();
	}

	//drawCollisionBounds(D3DXVECTOR3(10, 0, 0), D3DXVECTOR3(10, 10, 10), 2.0f);
	
	// Render tractor beams
	drawAllTractorBeams();
	
	//Render all engine exhausts
	drawAllEngines();

	// Render entity indicators
	drawAllEID();

	// Render static hud elements
	drawStaticHudElements();
}

// called each frame after processing keyboard state from that frame
// clears out up keys
void GameResources::updateKeyboardState() {
	m_ks.wUp = false;
	m_ks.aUp = false;
	m_ks.sUp = false;
	m_ks.dUp = false;
	m_ks.upUp = false;
	m_ks.leftUp = false;
	m_ks.downUp = false;
	m_ks.rightUp = false;
}

void GameResources::switchCamera() {
	GameResources::debugCamOn =  !GameResources::debugCamOn;
	if(debugCamOn) {
		curCam = &debugCam;
	} else {
		curCam = &playerCam;
	}
}

void GameResources::switchPlayerCameraScale() {
	playerCamScaleLevel = ((playerCamScaleLevel + 1) % 3);
	switch (playerCamScaleLevel) {
	case 0:
		playerCamScale = 1.0f;
		break;
	case 1:
		playerCamScale = 1.75f;
		break;
	case 2:
		playerCamScale = 2.5f;
		break;
	default:
		playerCamScale = 1.0f;
		break;
	}
}

void GameResources::updatePlayerCamera() {
	if(playerShip) {
		//TODO update to work with quaternions
		/*
		playerCam.m_vEye = playerShip->m_pos - (*D3DXVec3Normalize(&playerShip->m_dir, &playerShip->m_dir))*PLAYER_CAM_DISTANCE + D3DXVECTOR3(0.0f, PLAYER_CAM_HEIGHT, 0.0f);
		//playerCam.m_vUp = playerShip->FIGURE OUT UP VECTOR (once we have quaternions)
		playerCam.m_vAt = playerShip->m_pos + (*D3DXVec3Normalize(&playerShip->m_dir, &playerShip->m_dir))*PLAYER_CAM_LOOKAT_DISTANCE;
		*/

		D3DXMATRIX matRotate;
		D3DXQUATERNION temp_q;
		D3DXMatrixRotationQuaternion(&matRotate, D3DXQuaternionNormalize(&temp_q, &(playerShip->m_orientation)));
		
		D3DXVECTOR3 dir(0,0,1);
		D3DXVec3TransformCoord(&dir, &dir, &matRotate);

		D3DXVECTOR3 up(0,1,0);
		D3DXVec3TransformCoord(&up, &up, &matRotate);
		
		playerCam.m_vUp = up;
		playerCam.m_vEye = playerShip->m_pos - (*D3DXVec3Normalize(&dir, &dir)) * PLAYER_CAM_DISTANCE * playerCamScale + (PLAYER_CAM_HEIGHT * playerCamScale * playerCam.m_vUp);
		playerCam.m_vAt = playerShip->m_pos + (*D3DXVec3Normalize(&dir, &dir))*PLAYER_CAM_LOOKAT_DISTANCE * playerCamScale;

		//playerCam.setOrientation(*D3DXQuaternionNormalize(&(playerShip->m_orientation), &(playerShip->m_orientation)));
		playerCam.updateView();
	}
}

void GameResources::updateDebugCamera() {
	
	INT8	updateFwd = (INT8)(m_ks.wDown || m_ks.wUp) - (INT8)(m_ks.sDown || m_ks.wUp);
	INT8	updateStrafe = (INT8)(m_ks.dDown || m_ks.dUp) - (INT8)(m_ks.aDown || m_ks.aUp);
	INT8	updateYaw = (INT8)(m_ks.rightDown || m_ks.rightUp) - (INT8)(m_ks.leftDown || m_ks.leftUp);
	INT8	updatePitch =  (INT8)(m_ks.downDown || m_ks.downUp) - (INT8)(m_ks.upDown || m_ks.upUp);

	if(!debugCamOn || !updateFwd && !updateStrafe && !updateYaw && !updatePitch)
		return;

	//if(!debugCamOn) {
	//	return;
	//}
	
	D3DXVECTOR3 fwdVec, rightVec;

	if(updateYaw || updatePitch) { // update direction
		debugCam.m_yaw += updateYaw*D3DXToRadian(Gbls::debugCamTurnSpeed);
		debugCam.m_pitch += updatePitch*D3DXToRadian(Gbls::debugCamTurnSpeed);
		debugCam.m_pitch = max(Gbls::debugCamMinPitch, min(Gbls::debugCamMaxPitch, debugCam.m_pitch)); // clip m_pitch
	}

	if(updateFwd || updateStrafe) { // update position
		fwdVec = debugCam.m_vAt-debugCam.m_vEye; // get fwd vector
		D3DXVec3Normalize(&fwdVec, &fwdVec);
		D3DXVec3Cross(&rightVec, &(debugCam.m_vUp), &fwdVec); // get right vector
		D3DXVec3Normalize(&rightVec, &rightVec);

		if(updateFwd) {
			debugCam.m_vEye = debugCam.m_vEye + fwdVec*(Gbls::debugCamMoveSpeed*updateFwd);
			//D3DXVec3Normalize(&debugCam.m_vEye, &debugCam.m_vEye);
		}

		if(updateStrafe) {
			debugCam.m_vEye = debugCam.m_vEye + rightVec*(Gbls::debugCamMoveSpeed*updateStrafe);
			//D3DXVec3Normalize(&debugCam.m_vEye, &debugCam.m_vEye);
		}
	}

	//update lookAt vector & up vector (must be done for both position and direction udpates)
	D3DXMATRIX yawMatrix;
	D3DXMATRIX pitchMatrix;

	//define direction vectors
	debugCam.m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    rightVec  = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
    fwdVec  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXMatrixRotationAxis(&pitchMatrix, &rightVec, debugCam.m_pitch);
	D3DXMatrixRotationAxis(&yawMatrix, &debugCam.m_vUp, debugCam.m_yaw);
	
	//compute direction (fwd) vector
    D3DXVec3TransformCoord(&fwdVec, &fwdVec, &pitchMatrix);
    D3DXVec3TransformCoord(&fwdVec, &fwdVec, &yawMatrix);
    D3DXVec3Normalize(&fwdVec, &fwdVec);

	//compute right vector (to update up vector)
    D3DXVec3Cross(&rightVec, &debugCam.m_vUp, &fwdVec);
    D3DXVec3Normalize(&rightVec, &rightVec);

	//update up vector
    D3DXVec3Cross(&debugCam.m_vUp, &fwdVec, &rightVec);

	//update lookAt vector
	debugCam.m_vAt = debugCam.m_vEye + fwdVec;

	//update directx camera view
	debugCam.updateView();
}

void GameResources::updateGameState(GameState<Entity> & newGameState) {

	double curTime = timeGetTime();
	static double s_lastTime = curTime; // first time initialization, static otherwise
	float elapsedTime = (float)((curTime - s_lastTime) * 0.001);
	s_lastTime = curTime;

	updateKeyboardState(); // Clear out keyboard state bits

	//used for individual deletes if we ever implement that
	//for( map<int,C_Entity*>::iterator ii=entityMap.begin(); ii!=entityMap.end(); ++ii)
	//   {
	//	(*ii).second->updated = false;
	//}
	timeStr = newGameState.getRemainingTimeString();
	scoreList_t scores = newGameState.getScore();
	
	for( scoreList_t::iterator ii=scores.begin(); ii!=scores.end(); ++ii)
	{
		UINT pNum = (*ii).first;
		//if (pNum >= 1 && pNum <= 4) {
			playerScore[pNum] = (*ii).second;
		//}
	}

	//for( UINT i = 0; i < scores.size(); ++i)
	//{
	//	UINT pNum = scores[i].first;
	//	if (pNum >= 1 && pNum <= 4) {
	//		playerScore[pNum-1] = scores[i].second;
	//	}
	//}



	if (newGameState.size() == 0) {
		resetGameState();
	} else {
		// Update state of entitites
		for (DWORD i = 0; i < newGameState.size(); i++) {
			int id = newGameState[i]->getID();
			if(entityMap.find(id) == entityMap.end()) {
				entityMap[id] = createEntity(newGameState[i].get());
			} else {
				entityMap[id]->update(newGameState[i]);
				//entityMap[id]->updated = true; 	//used for individual deletes if we ever implement that
			}
		}
	}

	//used for individual deletes if we ever implement that
	//for( map<int,C_Entity*>::iterator ii=entityMap.begin(); ii!=entityMap.end(); ++ii)
	//   {
	//	if(false == (*ii).second->updated) {
	//		(*ii).second->updated = false;
	//	}
	//}

	// Update current camera
	if (debugCamOn) {
		updateDebugCamera();
	} else {
		updatePlayerCamera();
	}

	// Update particle system
	partSystem->update(tBeamPGroup, elapsedTime);
	for (UINT i = 0; i < enginePGroupList.size(); i++) {
		partSystem->update(enginePGroupList[i], elapsedTime);
	}
}

void GameResources::resetGameState() {
	playerShip = NULL;
	shipList.clear();
	asteroidList.clear();
	mothershipList.clear();
	tractorBeamList.clear();
	resourceList.clear();
	for (UINT i = 0; i < eIDList.size(); i++) {
		delete eIDList[i];
	}
	eIDList.clear();
	for (UINT i = 0; i < enginePGroupList.size(); i++) {
		delete enginePGroupList[i];
	}
	enginePGroupList.clear();
	for( map<int,C_Entity*>::iterator ii=entityMap.begin(); ii!=entityMap.end(); ++ii)
	{
		delete (*ii).second;
	}
	entityMap.clear();
}

C_Entity * GameResources::createEntity(Entity * newEnt) {
	C_Entity * ret = NULL;
	/*typeid(*newEnt);
	if (typeid(*newEnt) == typeid(Ship)) {
		ret = new C_Ship(newEnt);
	} else {
		//Do nothing! We can't instantiate anything but a C_Ship.
		//ret = new C_Entity(newEnt);
	}*/
	switch (newEnt->m_type) {
	//case ENTITY :
	//	ret = new Entity(*newEnt);
	//	break;
	case SHIP :
		{
		C_Ship * tmp = new C_Ship(newEnt);
		shipList.push_back(tmp);
		if (tmp->m_playerNum == playerNum) {
			playerShip = tmp;
		}
		EntityIdentifier * shipEID = new EntityIdentifier();
		shipEID->targetEntity = tmp;
		shipEID->m_onScreenSprite.setTexture(shipEIDTexture);
		shipEID->m_onScreenSprite.setCenterToTextureMidpoint();
		shipEID->m_offScreenSprite.setTexture(shipEIDTexture);
		shipEID->m_offScreenSprite.setCenterToTextureMidpoint();
		eIDList.push_back(shipEID);
		EnginePGroup * epg = new EnginePGroup(EnginePartTexture);
		epg->shipEnt = tmp;
		enginePGroupList.push_back(epg);
		ret = tmp;
		}
		break;
	//case BASE :
	//	break;
	case ASTEROID :
		{
		C_Asteroid * tmp = new C_Asteroid(newEnt);
		asteroidList.push_back(tmp);
		ret = tmp;
		}
		break;
	case TRACTORBEAM :
		{
		C_TractorBeam * tmp = new C_TractorBeam(newEnt);
		tractorBeamList.push_back(tmp);
		ret = tmp;
		}
		break;
	case MOTHERSHIP :
		{
		C_Mothership * tmp = new C_Mothership(newEnt);
		mothershipList.push_back(tmp);
		ret = tmp;
		}
		break;
	case RESOURCE :
		{
		C_Resource * tmp = new C_Resource(newEnt);
		resourceList.push_back(tmp);
		ret = tmp;
		}
		break;
	}
	//ret->updated = true; 	//used for individual deletes if we ever implement that
	return ret;
}