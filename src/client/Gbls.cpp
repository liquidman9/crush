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

// Still hardcoded:

float Gbls::debugCamMaxPitch = D3DXToRadian(89.0f);
float Gbls::debugCamMinPitch = D3DXToRadian(-89.0f);

// Loaded from config files

int Gbls::numShipMeshes = 4;
std::wstring * Gbls::shipMeshFilepath;
Mesh * Gbls::shipMesh;
float Gbls::debugCamMoveSpeed = 0.5f;
float Gbls::debugCamTurnSpeed = 2.0f;
std::wstring Gbls::skyboxTextureFilepath_Front = L"SkyBox_Front.jpg";
std::wstring Gbls::skyboxTextureFilepath_Back = L"SkyBox_Back.jpg";
std::wstring Gbls::skyboxTextureFilepath_Left = L"SkyBox_Left.jpg";
std::wstring Gbls::skyboxTextureFilepath_Right = L"SkyBox_Right.jpg";
std::wstring Gbls::skyboxTextureFilepath_Top = L"SkyBox_Top.jpg";
std::wstring Gbls::skyboxTextureFilepath_Bottom = L"SkyBox_Bottom.jpg";
D3DXCOLOR Gbls::lightDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
D3DXCOLOR Gbls::lightSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
D3DXVECTOR3 Gbls::lightDirection(1.0f, 0.5f, 1.0f);
D3DCOLOR Gbls::lightAmbient = D3DCOLOR_XRGB(50, 50, 50);

int Gbls::windowWidth = 640;
int Gbls::windowHeight = 480;

void Gbls::initFromConfig() {
	std::stringstream stream;
	string tmpString;
	ConfigSettings conf = *ConfigSettings::config;


	/* Ship Mesh Info */

	conf.getValue("numShipMeshes",numShipMeshes);
	shipMeshFilepath = new wstring[numShipMeshes];
	shipMesh = new Mesh[numShipMeshes];
	for (int i = 0; i < numShipMeshes; i++) {
		stream.str(string());
		stream.clear();
		stream << i;
		if(!conf.getValue("shipMeshFilepath_"+stream.str(),tmpString)) {  //default
			shipMeshFilepath[i] = L"player_ship_001.x";
		} else {  //assign to wstring
			shipMeshFilepath[i] = wstring(tmpString.begin(), tmpString.end());
		}
	}
	for (int i = 0; i < numShipMeshes; i++) {
		float tmpF[4] = {1.0f, 0.0f, 0.0f, 0.0f};
		stream.str(string());
		stream.clear();
		stream << i;
		conf.getValue("shipMeshDefaultScale_" + stream.str(), tmpF[0]);
		conf.getValue("shipMeshDefaultDirDegX_" + stream.str(),tmpF[1]);
		conf.getValue("shipMeshDefaultDirDegY_" + stream.str(),tmpF[2]);
		conf.getValue("shipMeshDefaultDirDegZ_" + stream.str(),tmpF[3]);
		shipMesh[i].setScaleRotate(tmpF[0], tmpF[1], tmpF[2], tmpF[3]);
	}

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

}