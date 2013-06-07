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

LPDIRECT3DTEXTURE9 Gbls::powerupTexture = NULL; 
LPDIRECT3DTEXTURE9 Gbls::shipTexture1 = NULL; 
LPDIRECT3DTEXTURE9 Gbls::shipTexture2 = NULL; 
LPDIRECT3DTEXTURE9 Gbls::shipTexture3 = NULL; 
LPDIRECT3DTEXTURE9 Gbls::shipTexture4 = NULL; 
LPDIRECT3DTEXTURE9 Gbls::mothershipTexture1 = NULL; 
LPDIRECT3DTEXTURE9 Gbls::mothershipTexture2 = NULL; 
LPDIRECT3DTEXTURE9 Gbls::mothershipTexture3 = NULL; 
LPDIRECT3DTEXTURE9 Gbls::mothershipTexture4 = NULL; 
LPDIRECT3DTEXTURE9 Gbls::scoreScreenTexture[4] = {NULL, NULL, NULL, NULL};
LPDIRECT3DTEXTURE9 Gbls::pScoreScrenAlphaTexture = NULL;

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


std::wstring Gbls::shipTexFilepath1 = L"player_ship_skin_001.dds";
std::wstring Gbls::shipTexFilepath2 = L"player_ship_skin_002.dds";
std::wstring Gbls::shipTexFilepath3 = L"player_ship_skin_003.dds";
std::wstring Gbls::shipTexFilepath4 = L"player_ship_skin_004.dds";
std::wstring Gbls::mothershipTexFilepath1 = L"mothership_skin_001.dds";
std::wstring Gbls::mothershipTexFilepath2 = L"mothership_skin_002.dds";
std::wstring Gbls::mothershipTexFilepath3 = L"mothership_skin_003.dds";
std::wstring Gbls::mothershipTexFilepath4 = L"mothership_skin_004.dds";
std::wstring Gbls::scoreScreenTexFilepath1 = L"scoreboard_001.png";
std::wstring Gbls::scoreScreenTexFilepath2 = L"scoreboard_002.png";
std::wstring Gbls::scoreScreenTexFilepath3 = L"scoreboard_003.png";
std::wstring Gbls::scoreScreenTexFilepath4 = L"scoreboard_004.png";
std::wstring Gbls::pScoreScrenAlphaTexFilepath = L"scoreboard_alpha.png";

float Gbls::debugCamMoveSpeed = 0.5f;
float Gbls::debugCamTurnSpeed = 2.0f;
std::wstring Gbls::skyboxTextureFilepath = L"nebula_003.png";
//std::wstring Gbls::skyboxTextureFilepath_Front = L"nebula_003.png";
//std::wstring Gbls::skyboxTextureFilepath_Back = L"nebula_003.png";
//std::wstring Gbls::skyboxTextureFilepath_Left = L"nebula_003.png";
//std::wstring Gbls::skyboxTextureFilepath_Right = L"nebula_003.png";
//std::wstring Gbls::skyboxTextureFilepath_Top = L"nebula_003.png";
//std::wstring Gbls::skyboxTextureFilepath_Bottom = L"nebula_003.png";

std::wstring Gbls::shipEIDTextureFilepath_resource = L"arrow_icon_resource.png";
std::wstring Gbls::ship1EIDTextureFilepath_insig = L"arrow_icon_001.png";
std::wstring Gbls::ship1EIDTextureFilepath_arrow = L"arrow_color_001.png";
std::wstring Gbls::ship2EIDTextureFilepath_insig = L"arrow_icon_002.png";
std::wstring Gbls::ship2EIDTextureFilepath_arrow = L"arrow_color_002.png";
std::wstring Gbls::ship3EIDTextureFilepath_insig = L"arrow_icon_003.png";
std::wstring Gbls::ship3EIDTextureFilepath_arrow = L"arrow_color_003.png";
std::wstring Gbls::ship4EIDTextureFilepath_insig = L"arrow_icon_004.png";
std::wstring Gbls::ship4EIDTextureFilepath_arrow = L"arrow_color_004.png";
std::wstring Gbls::powerupTextureFilepath = L"powerup_skin.dds";

std::wstring Gbls::alertTextureFilepath = L"lock_on.png";

std::wstring Gbls::player1SplashTextureFilepath = L"splashscreen_001.png";
std::wstring Gbls::player2SplashTextureFilepath = L"splashscreen_002.png";
std::wstring Gbls::player3SplashTextureFilepath = L"splashscreen_003.png";
std::wstring Gbls::player4SplashTextureFilepath = L"splashscreen_004.png";

//std::wstring Gbls::extractorEIDTextureOnScreenFilepath = L"extractor_onscreen.png";
std::wstring Gbls::extractorEIDTextureOffScreenFilepath_insig = L"arrow_resource_icon.png";
std::wstring Gbls::extractorEIDTextureOffScreenFilepath_arrow = L"arrow_mothership_base.png";

std::wstring Gbls::consumedPowerupTexture1Filepath = L"powerup_speed_consumed.png";
std::wstring Gbls::consumedPowerupTexture2Filepath = L"powerup_blast_consumed.png";
std::wstring Gbls::consumedPowerupTexture3Filepath = L"powerup_shield_consumed.png";

std::wstring Gbls::powerupTexture1Filepath = L"powerup_speed.png";
std::wstring Gbls::powerupTexture2Filepath = L"powerup_blast.png";
std::wstring Gbls::powerupTexture3Filepath = L"powerup_shield.png";

std::wstring Gbls::player1HudFilepath_insig = L"score_icon_001.png";
std::wstring Gbls::player2HudFilepath_insig = L"score_icon_002.png";
std::wstring Gbls::player3HudFilepath_insig = L"score_icon_003.png";
std::wstring Gbls::player4HudFilepath_insig = L"score_icon_004.png";

std::wstring Gbls::resourceEIDTextureFilepath = L"arrow_resource.png";

std::wstring Gbls::mothershipEIDTextureFilepath_arrow = L"arrow_mothership.png";
std::wstring Gbls::mothershipEIDTextureFilepath_insig = L"arrow_mothership_icon.png";


std::wstring Gbls::tBeamPartTexFilepath = L"particle_001.bmp"; 
std::wstring Gbls::enginePartTexNormFilepath = L"particle_002.bmp"; 
std::wstring Gbls::enginePartTexSpeedupFilepath = L"particle_003.bmp"; 
std::wstring Gbls::fontStyle = L"GunShip";
int Gbls::fontSize = 24;
D3DXCOLOR Gbls::lightDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
D3DXCOLOR Gbls::lightSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
D3DXVECTOR3 Gbls::lightDirection(1.0f, 0.5f, 1.0f);
D3DCOLOR Gbls::lightAmbient = D3DCOLOR_XRGB(50, 50, 50);

int Gbls::windowWidth = 640;
int Gbls::windowHeight = 480;

int Gbls::fullScreenWidth = 1280;
int Gbls::fullScreenHeight = 720;

float Gbls::audio3DScale = 30.0f;
float Gbls::collision3DScale = 240.0f;
float Gbls::doppler3DScale = 20.0f;
float Gbls::musicLevel = 0.1f;
float Gbls::ambienceLevel = 1.0f;
float Gbls::tbeamLevel = 1.0f;
float Gbls::thrusterLevel = 1.0f;
float Gbls::reverseLevel = 1.0f;
float Gbls::speedLevel = 2.0f;
float Gbls::pulseLevel = 1.0f;
float Gbls::shieldLevel = 1.0f;
float Gbls::repickupLevel = 1.0f;
float Gbls::pupickupLevel = 1.0f;

float Gbls::standardImpulse = 200000.0f;
float Gbls::minFreqRatio = 0.05f;
float Gbls::maxFreqRatio = 6.0f;
float Gbls::maxImpulseVol = 100.0f;

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
	
	mothershipMesh.setScaleRotate(2.0, 0.0, 180.0, 0.0);

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
			resourceMeshFilepath = L"resource.x";
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
			powerupMeshFilepath = L"powerup.x";
	} else {  //assign to wstring
			powerupMeshFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	powerupMesh.setScaleRotate(1, 0, 0, 0);

	/* Debug Cam */
	conf.getValue("debugCamMoveSpeed", debugCamMoveSpeed);
	conf.getValue("debugCamTurnSpeed", debugCamTurnSpeed);
	/* Skybox Texture Filepaths */
	
	if(conf.getValue("skyboxTextureFilepath", tmpString)) {
		skyboxTextureFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	//if(conf.getValue("skyboxTextureFilepath_Front", tmpString)) {
	//	skyboxTextureFilepath_Front = wstring(tmpString.begin(), tmpString.end());
	//}
	//if(conf.getValue("skyboxTextureFilepath_Back", tmpString)) {
	//	skyboxTextureFilepath_Back = wstring(tmpString.begin(), tmpString.end());
	//}
	//if(conf.getValue("skyboxTextureFilepath_Left", tmpString)) {
	//	skyboxTextureFilepath_Left = wstring(tmpString.begin(), tmpString.end());
	//}
	//if(conf.getValue("skyboxTextureFilepath_Right", tmpString)) {
	//	skyboxTextureFilepath_Right = wstring(tmpString.begin(), tmpString.end());
	//}
	//if(conf.getValue("skyboxTextureFilepath_Top", tmpString)) {
	//	skyboxTextureFilepath_Top = wstring(tmpString.begin(), tmpString.end());
	//}
	//if(conf.getValue("skyboxTextureFilepath_Bottom", tmpString)) {
	//	skyboxTextureFilepath_Bottom = wstring(tmpString.begin(), tmpString.end());
	//}

	if(conf.getValue("consumedPowerupTexture1Filepath", tmpString)) {
		consumedPowerupTexture1Filepath = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("consumedPowerupTexture2Filepath", tmpString)) {
		consumedPowerupTexture2Filepath = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("consumedPowerupTexture3Filepath", tmpString)) {
		consumedPowerupTexture3Filepath = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("powerupTexture1Filepath", tmpString)) {
		powerupTexture1Filepath = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("powerupTexture2Filepath", tmpString)) {
		powerupTexture2Filepath = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("powerupTexture3Filepath", tmpString)) {
		powerupTexture3Filepath = wstring(tmpString.begin(), tmpString.end());
	}

	if(conf.getValue("shipEIDTextureFilepath_resource", tmpString)) {
		shipEIDTextureFilepath_resource = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("ship1EIDTextureFilepath_insig", tmpString)) {
		ship1EIDTextureFilepath_insig = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("ship1EIDTextureFilepath_arrow", tmpString)) {
		ship1EIDTextureFilepath_arrow = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("ship2EIDTextureFilepath_insig", tmpString)) {
		ship2EIDTextureFilepath_insig = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("ship2EIDTextureFilepath_arrow", tmpString)) {
		ship2EIDTextureFilepath_arrow = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("ship3EIDTextureFilepath_insig", tmpString)) {
		ship3EIDTextureFilepath_insig = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("ship3EIDTextureFilepath_arrow", tmpString)) {
		ship3EIDTextureFilepath_arrow = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("ship4EIDTextureFilepath_insig", tmpString)) {
		ship4EIDTextureFilepath_insig = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("ship4EIDTextureFilepath_arrow", tmpString)) {
		ship4EIDTextureFilepath_arrow = wstring(tmpString.begin(), tmpString.end());
	}

	if(conf.getValue("alertTextureFilepath", tmpString)) {
		alertTextureFilepath = wstring(tmpString.begin(), tmpString.end());
	}

	if(conf.getValue("player1HudFilepath_insig", tmpString)) {
		player1HudFilepath_insig = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("player2HudFilepath_insig", tmpString)) {
		player2HudFilepath_insig = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("player3HudFilepath_insig", tmpString)) {
		player3HudFilepath_insig = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("player4HudFilepath_insig", tmpString)) {
		player4HudFilepath_insig = wstring(tmpString.begin(), tmpString.end());
	}

	if(conf.getValue("player1SplashTextureFilepath", tmpString)) {
		player1SplashTextureFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("player2SplashTextureFilepath", tmpString)) {
		player2SplashTextureFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("player3SplashTextureFilepath", tmpString)) {
		player3SplashTextureFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("player4SplashTextureFilepath", tmpString)) {
		player4SplashTextureFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	
	if(conf.getValue("extractorEIDTextureOffScreenFilepath_arrow", tmpString)) {
		extractorEIDTextureOffScreenFilepath_arrow = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("extractorEIDTextureOffScreenFilepath_insig", tmpString)) {
		extractorEIDTextureOffScreenFilepath_insig = wstring(tmpString.begin(), tmpString.end());
	}
	//if(conf.getValue("extractorEIDTextureOnScreenFilepath", tmpString)) {
	//	extractorEIDTextureOnScreenFilepath = wstring(tmpString.begin(), tmpString.end());
	//}

	if(conf.getValue("mothershipEIDTextureFilepath_arrow", tmpString)) {
		mothershipEIDTextureFilepath_arrow = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("mothershipEIDTextureFilepath_insig", tmpString)) {
		mothershipEIDTextureFilepath_insig = wstring(tmpString.begin(), tmpString.end());
	}

	if(conf.getValue("resourceEIDTextureFilepath", tmpString)) {
		resourceEIDTextureFilepath = wstring(tmpString.begin(), tmpString.end());
	}


	if(conf.getValue("tBeamPartTexFilepath", tmpString)) {
		tBeamPartTexFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("enginePartTexNormFilepath", tmpString)) {
		enginePartTexNormFilepath = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("enginePartTexSpeedupFilepath", tmpString)) {
		enginePartTexSpeedupFilepath = wstring(tmpString.begin(), tmpString.end());
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
	if(conf.getValue("mothershipTexFilepath1", tmpString)) {
		mothershipTexFilepath1 = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("mothershipTexFilepath2", tmpString)) {
		mothershipTexFilepath2 = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("mothershipTexFilepath3", tmpString)) {
		mothershipTexFilepath3 = wstring(tmpString.begin(), tmpString.end());
	}
	if(conf.getValue("mothershipTexFilepath4", tmpString)) {
		mothershipTexFilepath4 = wstring(tmpString.begin(), tmpString.end());
	}


	AddFontResourceEx(L"gun4f.ttf", FR_PRIVATE, 0);

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

	conf.getValue("audio3DScale", audio3DScale);
	conf.getValue("collision3DScale", collision3DScale);
	conf.getValue("doppler3DScale", doppler3DScale);
	conf.getValue("musicLevel", musicLevel);
	conf.getValue("ambienceLevel", ambienceLevel);
	conf.getValue("thrusterLevel", thrusterLevel);
	conf.getValue("tbeamLevel", tbeamLevel);
	conf.getValue("reverseLevel", reverseLevel);
	conf.getValue("speedLevel", speedLevel);
	conf.getValue("pulseLevel", pulseLevel);
	conf.getValue("shieldLevel", shieldLevel);
	conf.getValue("repickupLevel", repickupLevel);
	conf.getValue("pupickupLevel", pupickupLevel);

	conf.getValue("standardImpulse", standardImpulse);
	conf.getValue("minFreqRatio", minFreqRatio);
	conf.getValue("maxFreqRatio", maxFreqRatio);
	conf.getValue("maxImpulseVol", maxImpulseVol);

}