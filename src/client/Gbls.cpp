/*
 * Gbls.cpp
 * Provides a set of globals used across the project
 * This file should be replaced with a config loader (and/or local variables when possible)
 */

// Project includes
#include <client/Gbls.h>
#include <shared/ConfigSettings.h>

// Globals that aren't configs
LPDIRECT3D9 Gbls::pD3D = NULL;
LPDIRECT3DDEVICE9 Gbls::pd3dDevice = NULL;
D3DPRESENT_PARAMETERS Gbls::thePresentParams;
int Gbls::fontHeight; // calculated
float Gbls::percentMissedFrames;

LPDIRECT3DTEXTURE9 Gbls::shipTexture1 = NULL; 
LPDIRECT3DTEXTURE9 Gbls::shipTexture2 = NULL; 
LPDIRECT3DTEXTURE9 Gbls::shipTexture3 = NULL; 
LPDIRECT3DTEXTURE9 Gbls::shipTexture4 = NULL; 

// Still hardcoded:

float Gbls::debugCamMaxPitch = D3DXToRadian(89.0f);
float Gbls::debugCamMinPitch = D3DXToRadian(-89.0f);

// Loaded from config files

int Gbls::numAsteroidMeshes = 5;
std::wstring Gbls::shipMeshFilepath;
Mesh Gbls::shipMesh;
std::wstring Gbls::mothershipMeshFilepath;
Mesh Gbls::mothershipMesh;
std::wstring * Gbls::asteroidMeshFilepath;
Mesh * Gbls::asteroidMesh;
//std::wstring Gbls::tractorBeamMeshFilepath;
//Mesh Gbls::tractorBeamMesh;
std::wstring Gbls::resourceMeshFilepath;
Mesh Gbls::resourceMesh;
std::wstring Gbls::extractorMeshFilepath;
Mesh Gbls::extractorMesh;
std::wstring Gbls::powerupMeshFilepath;
Mesh Gbls::powerupMesh;


std::wstring Gbls::shipTexFilepath1 = L"player_ship_skin_001.png";
std::wstring Gbls::shipTexFilepath2 = L"player_ship_skin_002.png";
std::wstring Gbls::shipTexFilepath3 = L"player_ship_skin_003.png";
std::wstring Gbls::shipTexFilepath4 = L"player_ship_skin_004.png";

float Gbls::debugCamMoveSpeed = 0.5f;
float Gbls::debugCamTurnSpeed = 2.0f;
std::wstring Gbls::skyboxTextureFilepath_Front = L"SkyBox_Front.jpg";
std::wstring Gbls::skyboxTextureFilepath_Back = L"SkyBox_Back.jpg";
std::wstring Gbls::skyboxTextureFilepath_Left = L"SkyBox_Left.jpg";
std::wstring Gbls::skyboxTextureFilepath_Right = L"SkyBox_Right.jpg";
std::wstring Gbls::skyboxTextureFilepath_Top = L"SkyBox_Top.jpg";
std::wstring Gbls::skyboxTextureFilepath_Bottom = L"SkyBox_Bottom.jpg";
std::wstring Gbls::shipEIDTextureFilepath = L"arrow.png";
std::wstring Gbls::mothershipEIDTextureFilepath = L"arrowMothership.png";
std::wstring Gbls::tBeamPartTexFilepath = L"particle01.bmp"; 
std::wstring Gbls::enginePartTexFilepath = L"particle02.bmp"; 
std::wstring Gbls::fontStyle = L"Ariel";
int Gbls::fontSize = 48;
D3DXCOLOR Gbls::lightDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
D3DXCOLOR Gbls::lightSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
D3DXVECTOR3 Gbls::lightDirection(1.0f, 0.5f, 1.0f);
D3DCOLOR Gbls::lightAmbient = D3DCOLOR_XRGB(50, 50, 50);

int Gbls::windowWidth = 640;
int Gbls::windowHeight = 480;

int Gbls::fullScreenWidth = 1280;
int Gbls::fullScreenHeight = 720;

void Gbls::initFromConfig() {
	std::stringstream stream;
	string tmpString;
	ConfigSettings conf = *ConfigSettings::config;


	/* Ship Mesh Info */

	if(!conf.getValue("shipMeshFilepath", tmpString)) {  //default
			shipMeshFilepath = L"player_ship_001.x";
	} else {  //assign to wstring
			shipMeshFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	
	shipMesh.setScaleRotate(1.0, 0.0, 180.0, 0.0);

	if(!conf.getValue("mothershipMeshFilepath", tmpString)) {  //default
			mothershipMeshFilepath = L"mothership_001.x";
	} else {  //assign to wstring
			mothershipMeshFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	
	mothershipMesh.setScaleRotate(1.0, 0.0, 180.0, 0.0);

	//conf.getValue("numShipMeshes",numShipMeshes);
	//shipMeshFilepath = new wstring[numShipMeshes];
	//shipMesh = new Mesh[numShipMeshes];
	//for (int i = 0; i < numShipMeshes; i++) {
	//	stream.str(string());
	//	stream.clear();
	//	stream << i;
	//	if(!conf.getValue("shipMeshFilepath_"+stream.str(),tmpString)) {  //default
	//		shipMeshFilepath[i] = L"player_ship_001.x";
	//	} else {  //assign to wstring
	//		shipMeshFilepath[i] = wstring(tmpString.begin(), tmpString.end());
	//	}
	//}
	//for (int i = 0; i < numShipMeshes; i++) {
	//	float tmpF[4] = {1.0f, 0.0f, 0.0f, 0.0f};
	//	stream.str(string());
	//	stream.clear();
	//	stream << i;
	//	conf.getValue("shipMeshDefaultScale_" + stream.str(), tmpF[0]);
	//	conf.getValue("shipMeshDefaultDirDegX_" + stream.str(),tmpF[1]);
	//	conf.getValue("shipMeshDefaultDirDegY_" + stream.str(),tmpF[2]);
	//	conf.getValue("shipMeshDefaultDirDegZ_" + stream.str(),tmpF[3]);
	//	shipMesh[i].setScaleRotate(tmpF[0], tmpF[1], tmpF[2], tmpF[3]);
	//}

	/* Mothership Mesh Info */

	//conf.getValue("numShipMeshes",numShipMeshes);
	//mothershipMeshFilepath = new wstring[numShipMeshes];
	//mothershipMesh = new Mesh[numShipMeshes];
	//for (int i = 0; i < numShipMeshes; i++) {
	//	stream.str(string());
	//	stream.clear();
	//	stream << i;
	//	if(!conf.getValue("mothershipMeshFilepath_"+stream.str(),tmpString)) {  //default
	//		mothershipMeshFilepath[i] = L"player_ship_001.x";
	//	} else {  //assign to wstring
	//		mothershipMeshFilepath[i] = wstring(tmpString.begin(), tmpString.end());
	//	}
	//}
	//for (int i = 0; i < numShipMeshes; i++) {
	//	float tmpF[4] = {1.0f, 0.0f, 0.0f, 0.0f};
	//	stream.str(string());
	//	stream.clear();
	//	stream << i;
	//	conf.getValue("mothershipMeshDefaultScale_" + stream.str(), tmpF[0]);
	//	conf.getValue("mothershipMeshDefaultDirDegX_" + stream.str(),tmpF[1]);
	//	conf.getValue("mothershipMeshDefaultDirDegY_" + stream.str(),tmpF[2]);
	//	conf.getValue("mothershipMeshDefaultDirDegZ_" + stream.str(),tmpF[3]);
	//	mothershipMesh[i].setScaleRotate(tmpF[0], tmpF[1], tmpF[2], tmpF[3]);
	//}

	conf.getValue("numAsteroidMeshes",numAsteroidMeshes);
	asteroidMeshFilepath = new wstring[numAsteroidMeshes];
	asteroidMesh = new Mesh[numAsteroidMeshes];
	for (int i = 0; i < numAsteroidMeshes; i++) {
		stream.str(string());
		stream.clear();
		stream << i;
		if(!conf.getValue("asteroidMeshFilepath_"+stream.str(),tmpString)) {  //default
			asteroidMeshFilepath[i] = L"asteroid_001.x";
		} else {  //assign to wstring
			asteroidMeshFilepath[i] = wstring(tmpString.begin(), tmpString.end());
		}
	}

	/* TractorBeam */
	//if(!conf.getValue("tractorBeamMeshFilepath", tmpString)) {  //default
	//		tractorBeamMeshFilepath = L"player_ship_001.x";
	//} else {  //assign to wstring
	//		tractorBeamMeshFilepath = wstring(tmpString.begin(), tmpString.end());
	//}
	//tractorBeamMesh.setScaleRotate(1, 0, 0, 0);

	/* Resource */
	if(!conf.getValue("resourceMeshFilepath", tmpString)) {  //default
			resourceMeshFilepath = L"player_ship_001.x";
	} else {  //assign to wstring
			resourceMeshFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	resourceMesh.setScaleRotate(1, 0, 0, 0);

	/* Extractor */
	if(!conf.getValue("extractorMeshFilepath", tmpString)) {  //default
			extractorMeshFilepath = L"extractor.x";
	} else {  //assign to wstring
			extractorMeshFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	extractorMesh.setScaleRotate(1, 0, 0, 0);

	/* Powerup */
	if(!conf.getValue("powerupMeshFilepath", tmpString)) {  //default
			powerupMeshFilepath = L"tiger.x";
	} else {  //assign to wstring
			powerupMeshFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	powerupMesh.setScaleRotate(1, 0, 0, 0);

	/* Debug Cam */
	conf.getValue("debugCamMoveSpeed", debugCamMoveSpeed);
	conf.getValue("debugCamTurnSpeed", debugCamTurnSpeed);
	/* Skybox Texture Filepaths */

	if(conf.getValue("skyboxTextureFilepath_Front", tmpString)) {
		skyboxTextureFilepath_Front = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("skyboxTextureFilepath_Back", tmpString)) {
		skyboxTextureFilepath_Back = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("skyboxTextureFilepath_Left", tmpString)) {
		skyboxTextureFilepath_Left = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("skyboxTextureFilepath_Right", tmpString)) {
		skyboxTextureFilepath_Right = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("skyboxTextureFilepath_Top", tmpString)) {
		skyboxTextureFilepath_Top = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("skyboxTextureFilepath_Bottom", tmpString)) {
		skyboxTextureFilepath_Bottom = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("shipEIDTextureFilepath", tmpString)) {
		shipEIDTextureFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("mothershipEIDTextureFilepath", tmpString)) {
		mothershipEIDTextureFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("tBeamPartTexFilepath", tmpString)) {
		tBeamPartTexFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("enginePartTexFilepath", tmpString)) {
		enginePartTexFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("shipTexFilepath1", tmpString)) {
		shipTexFilepath1 = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("shipTexFilepath2", tmpString)) {
		shipTexFilepath2 = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("shipTexFilepath3", tmpString)) {
		shipTexFilepath3 = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("shipTexFilepath4", tmpString)) {
		shipTexFilepath4 = wstring(tmpString.begin(), tmpString.end());
	}

	if(conf.getValue("fontStyle", tmpString)) {
		fontStyle = wstring(tmpString.begin(), tmpString.end());
	}
	conf.getValue("fontSize", fontSize);

	/* Light info */

	conf.getValue("lightDiffuseColor_r", lightDiffuseColor.r);
	conf.getValue("lightDiffuseColor_g", lightDiffuseColor.g);
	conf.getValue("lightDiffuseColor_b", lightDiffuseColor.b);
	conf.getValue("lightDiffuseColor_a", lightDiffuseColor.a);
	
	conf.getValue("lightSpecularColor_r", lightSpecularColor.r);
	conf.getValue("lightSpecularColor_g", lightSpecularColor.g);
	conf.getValue("lightSpecularColor_b", lightSpecularColor.b);
	conf.getValue("lightSpecularColor_a", lightSpecularColor.a);

	conf.getValue("lightDirection_x", lightDirection.x);
	conf.getValue("lightDirection_y", lightDirection.y);
	conf.getValue("lightDirection_z", lightDirection.z);

	int tmp[3] = {50, 50, 50};
	conf.getValue("lightAmbient_r", tmp[0]);
	conf.getValue("lightAmbient_g", tmp[1]);
	conf.getValue("lightAmbient_b", tmp[2]);
	lightAmbient = D3DCOLOR_XRGB(tmp[0], tmp[1], tmp[2]);
	
	conf.getValue("windowWidth", windowWidth);
	conf.getValue("windowHeight", windowHeight);

	conf.getValue("fullScreenWidth", fullScreenWidth);
	conf.getValue("fullScreenHeight", fullScreenHeight);

}