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
vector<C_Asteroid*> GameResources::asteroidList;
std::map<int, C_Entity*> GameResources::entityMap;
bool GameResources::debugCamOn = true;
Camera GameResources::debugCam;
Camera GameResources::playerCam;
Camera* GameResources::curCam = &debugCam;
C_Ship* GameResources::playerShip = NULL;
//std::vector<R_Ship*> GameResources::r_ShipList;
//std::vector<Entity*> GameResources::entityList;
//std::vector<std::vector<Renderable*>*> GameResources::renderList;
struct GameResources::KeyboardState GameResources::m_ks;

HRESULT GameResources::initState() {
	HRESULT hres;
	
	curCam = &debugCam;

	// Init state that must be init every time device is reset
	reInitState();

	//debugCam.updateProjection();

	//debugCam.updateView();

	//// Tell the device to automatically normalize surface normals to keep them normal after scaling
	//Gbls::pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	
	////set backface cullling off TODO remove after models are fixed
	//Gbls::pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	//// Create lights for scene and set light properties
	//hres = GameResources::initLights();
	//if(FAILED (hres))
	//	return hres;

	// Initialize the skybox
	hres = Skybox::initSkybox();
	if(FAILED (hres))
		return hres;
	
	// create meshes
	hres = GameResources::initMeshes();
	if(FAILED (hres))
		return hres;



	// Clear keyboard state (at the moment only used for debug camera 4/13/2013)
	memset(&GameResources::m_ks, 0, sizeof(GameResources::KeyboardState));

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
	//pos.y = 0.3f; //pos.z = -0.6f;
	//pNum = 2;
	//color.r = 0.3f; color.g = 0.3f; color.b = 0.8f;
	//stmp = new Ship(pos, dir, pNum, tBeamOn);
	//etmp = createEntity(stmp);
	//entityMap[etmp->getID()] = etmp;

	//a bit ugly, probably easier to just loop through all the entity lists (left here in case we want to switch back)
	//renderList.push_back((std::vector<Renderable*>*)(&r_ShipList));

	/*end TODO remove*/

	return S_OK;
}

HRESULT GameResources::reInitState() {
	HRESULT hres;
	
	curCam->updateProjection();

	curCam->updateView();

	// Tell the device to automatically normalize surface normals to keep them normal after scaling
	Gbls::pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// Initialize the skybox
	//hres = Skybox::initSkybox();
	//if(FAILED (hres))
	//	return hres;

	// Create lights for scene and set light properties
	hres = GameResources::initLights();
	if(FAILED (hres))
		return hres;

	//// create meshes
	//hres = GameResources::initMeshes();
	//if(FAILED (hres))
	//	return hres;

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

	if(FAILED(hres = Gbls::asteroidMesh.Create(Gbls::asteroidMeshFilepath)))
			return hres;

	return S_OK;
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

void GameResources::drawAll()
{
	Skybox::drawSkybox();

	//TODO this draws objects in no particular order, resulting in many loads and unloads (probably) for textures and models. Should be fixed
	// Loop through all lists. Set up shaders, etc, as needed for each.
	for( map<int,C_Entity*>::iterator ii=entityMap.begin(); ii!=entityMap.end(); ++ii)
    {
		(*ii).second->draw();
	}
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
		playerCam.m_vEye = playerShip->m_pos - (*D3DXVec3Normalize(&dir, &dir))*PLAYER_CAM_DISTANCE + (PLAYER_CAM_HEIGHT * playerCam.m_vUp);
		playerCam.m_vAt = playerShip->m_pos + (*D3DXVec3Normalize(&dir, &dir))*PLAYER_CAM_LOOKAT_DISTANCE;

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
	
	updateKeyboardState(); // Clear out keyboard state bits
	
	// Update state of entitites
	for (DWORD i = 0; i < newGameState.size(); i++) {
		int id = newGameState[i]->getID();
		if(entityMap.find(id) == entityMap.end()) {
			entityMap[id] = createEntity(newGameState[i].get());
		} else {
			entityMap[id]->update(newGameState[i]);
		}
	}

	// Update current camera
	if (debugCamOn) {
		updateDebugCamera();
	} else {
		updatePlayerCamera();
	}

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
	}
	return ret;
}

void GameResources::releaseResources() {
	//TODO fix hardcoded destroys
	for(int i = 0; i < Gbls::numShipMeshes; i++) {
		Gbls::shipMesh[0].Destroy();
	}
	Skybox::releaseSkybox();
}