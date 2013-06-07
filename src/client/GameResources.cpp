//=================================================================================================
// GameResources.cpp Static container for game resources including meshes, objects, cameras, etc.
//=================================================================================================


// Global includes
#include <algorithm>
#include <typeinfo>
#include <sstream>

// Project includes
#include <shared/game/Entity.h>
#include <shared/game/Ship.h>

#include <client/GameResources.h>
#include <client/Gbls.h>
#include <client/graphics/util.h>
#include <client/graphics/Skybox.h>
#include <client/graphics/entities/C_Powerup.h>
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
vector<C_Extractor*> GameResources::extractorList; 
vector<C_Powerup*> GameResources::powerupList;
vector<C_Asteroid*> GameResources::asteroidList;
vector<EntityIdentifier*> GameResources::eIDList;
vector<EnginePGroup *> GameResources::enginePGroupList;
std::map<int, C_Entity*> GameResources::entityMap;
bool GameResources::debugCamOn = false;
Camera GameResources::debugCam;
Camera GameResources::playerCam;
Camera* GameResources::curCam = &playerCam;
float GameResources::playerCamScale = 1.0f;
int GameResources::playerCamScaleLevel = 0;
C_Ship* GameResources::playerShip = NULL;
C_Mothership* GameResources::playerMothership = NULL;
LPD3DXSPRITE GameResources::pd3dSprite = NULL;
LPD3DXFONT GameResources::pd3dFont = NULL;
bool GameResources::gameOver = FALSE;
bool GameResources::splashStart = FALSE;
scoreList_t GameResources::winnerList(4, std::pair<UINT, int>(0,0));

static float shipColors[4][4] =
{
	//1.0f, 0.7333f, 0.0f, 0.5f,
	0.0f, 0.0f,    1.0f, 0.5f,
	1.0f, 0.4333f, 0.0f, 0.5f,
	1.0f, 0.0f,    0.0f, 0.5f,
	1.0f, 1.0f,    0.0f, 0.5f,
};

static D3DCOLOR shipColorsInt[4] = 
{
	D3DCOLOR_XRGB((int)(shipColors[0][0]*255),(int)(shipColors[0][1]*255),(int)(shipColors[0][2]*255)),
	D3DCOLOR_XRGB((int)(shipColors[1][0]*255),(int)(shipColors[1][1]*255),(int)(shipColors[1][2]*255)),
	D3DCOLOR_XRGB((int)(shipColors[2][0]*255),(int)(shipColors[2][1]*255),(int)(shipColors[2][2]*255)),
	D3DCOLOR_XRGB((int)(shipColors[3][0]*255),(int)(shipColors[3][1]*255),(int)(shipColors[3][2]*255))
};


LPDIRECT3DTEXTURE9 GameResources::shipEIDTexture_resource = NULL;
LPDIRECT3DTEXTURE9 GameResources::ship1EIDTexture_insig = NULL;
LPDIRECT3DTEXTURE9 GameResources::ship1EIDTexture_arrow = NULL;
LPDIRECT3DTEXTURE9 GameResources::ship2EIDTexture_insig = NULL;
LPDIRECT3DTEXTURE9 GameResources::ship2EIDTexture_arrow = NULL;
LPDIRECT3DTEXTURE9 GameResources::ship3EIDTexture_insig = NULL;
LPDIRECT3DTEXTURE9 GameResources::ship3EIDTexture_arrow = NULL;
LPDIRECT3DTEXTURE9 GameResources::ship4EIDTexture_insig = NULL;
LPDIRECT3DTEXTURE9 GameResources::ship4EIDTexture_arrow = NULL;
LPDIRECT3DTEXTURE9* GameResources::shipEIDTextureArray_arrow[4] = {
	&GameResources::ship1EIDTexture_arrow, 
	&GameResources::ship2EIDTexture_arrow,
	&GameResources::ship3EIDTexture_arrow, 
	&GameResources::ship4EIDTexture_arrow
};
LPDIRECT3DTEXTURE9* GameResources::shipEIDTextureArray_insig[4] = {
	&GameResources::ship1EIDTexture_insig, 
	&GameResources::ship2EIDTexture_insig,
	&GameResources::ship3EIDTexture_insig, 
	&GameResources::ship4EIDTexture_insig
};

LPDIRECT3DTEXTURE9 GameResources::playerHudArray_insig[4] = {NULL};

LPDIRECT3DTEXTURE9 GameResources::alertTexture = NULL;

LPDIRECT3DTEXTURE9 GameResources::resourceEIDTexture = NULL;

LPDIRECT3DTEXTURE9 GameResources::extractorEIDTextureOnScreen = NULL;
LPDIRECT3DTEXTURE9 GameResources::extractorEIDTextureOffScreen_arrow = NULL;
LPDIRECT3DTEXTURE9 GameResources::extractorEIDTextureOffScreen_insig = NULL;

LPDIRECT3DTEXTURE9 GameResources::powerupTextureArray[3] = {
	NULL,
	NULL,
	NULL
};
LPDIRECT3DTEXTURE9 GameResources::powerupConsumedTexture[3] = {
	NULL,
	NULL,
	NULL
};



LPDIRECT3DTEXTURE9 GameResources::mothershipEIDTexture_arrow = NULL;
LPDIRECT3DTEXTURE9 GameResources::mothershipEIDTexture_insig = NULL;

LPDIRECT3DTEXTURE9 GameResources::playerSplashTexture[4] = {NULL};

LPDIRECT3DTEXTURE9 GameResources::tBeamPartTexture = NULL;
LPDIRECT3DTEXTURE9 GameResources::EnginePartNormTexture = NULL;
LPDIRECT3DTEXTURE9 GameResources::EnginePartSpeedupTexture = NULL;
ParticleSystem * GameResources::partSystem = NULL;
TBeamPGroup * GameResources::tBeamPGroup = NULL;
BurstPGroup * GameResources::burstPowerupPGroup = NULL;
PowerupPGroup * GameResources::powerupPGroup = NULL;
LPDIRECT3DTEXTURE9 GameResources::pGlowmapTexture = NULL;
LPDIRECT3DSURFACE9 GameResources::pGlowmapSurface = NULL; 
LPDIRECT3DTEXTURE9 GameResources::pScoreScreenTexture = NULL;
LPDIRECT3DSURFACE9 GameResources::pScoreScreenSurface = NULL;
LPDIRECT3DTEXTURE9 GameResources::pTmpBlurTexture = NULL;
LPDIRECT3DTEXTURE9 GameResources::pDefaultRenderTexture = NULL;
LPDIRECT3DSURFACE9 GameResources::pDefaultRenderSurface = NULL;
LPDIRECT3DSURFACE9 GameResources::pBackBuffer = NULL;
D3DXMATRIX GameResources::sunWorldMat;
LPD3DXMESH GameResources::sunMesh = NULL;
LPD3DXMESH GameResources::shieldMesh = NULL;

// for debugging collisions
bool GameResources::renderCBWireframe = false;
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
SoundManager* GameResources::sound;
GameInput* GameResources::input;
ID3DXEffect * GameResources::pEffectDefault;
ID3DXEffect * GameResources::pEffectGlowmap; 
ID3DXEffect * GameResources::pEffectTexToScreen;
ID3DXEffect * GameResources::pEffectBlend;
ID3DXEffect * GameResources::pEffectBlur;

//debug vis toggles
bool GameResources::shieldVisToggle = FALSE;
bool GameResources::speedupVisToggle = FALSE;

//static LPDIRECT3DVERTEXBUFFER9 fsQuadVBuffer = NULL;
//static LPDIRECT3DVERTEXBUFFER9 bloomQuadVBuffer = NULL;
//initialize quads for screen without proper corner values
static CUSTOMQUAD fsQuad;
static CUSTOMQUAD bloomQuad;
static CUSTOMQUAD scoreScreenQuad[4];
static CUSTOMQUAD alertQuad;
static CUSTOMQUAD scoreScreenQuadMain;
static CUSTOMQUAD playerIconHudQuad[4];
static CUSTOMQUAD playerSplashQuad;

HRESULT GameResources::initState() {
	HRESULT hres;

	curCam = &playerCam;
	debugCamOn = false;
	
	if(FAILED (hres = loadEffect(&pEffectDefault, L"shaders/CRUSH_Default.fx")))
		return hres;
	if(FAILED (hres = loadEffect(&pEffectGlowmap, L"shaders/CRUSH_Glowmap.fx")))
		return hres;
	if(FAILED (hres = loadEffect(&pEffectTexToScreen, L"shaders/CRUSH_TexToScreen.fx")))
		return hres;
	if(FAILED (hres = loadEffect(&pEffectBlend, L"shaders/CRUSH_Blend.fx")))
		return hres;
	if(FAILED (hres = loadEffect(&pEffectBlur, L"shaders/CRUSH_Blur.fx")))
		return hres;

	// Init the SUN (mesh init in initmeshes)
	D3DXMatrixTranslation(&sunWorldMat, 10, 3.5, 10);

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

	//ugh ugly shouldn't be here sorry oh god last week
	
	Gbls::powerupMesh.m_dwNumMaterials = 1;
	Gbls::powerupMesh.m_pMeshMaterials = new D3DMATERIAL9[1];
	Gbls::powerupMesh.m_pMeshTextures = new LPDIRECT3DTEXTURE9[1];
	Gbls::powerupMesh.m_matInitScaleRot = Gbls::shipMesh.m_matInitScaleRot;
	Gbls::powerupMesh.m_pMeshTextures[0] = Gbls::powerupTexture;

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
	burstPowerupPGroup = new BurstPGroup(tBeamPartTexture);
	powerupPGroup = new PowerupPGroup(tBeamPartTexture);
	//GameResources::partSystem->releaseBurst(powerupPGroup);
	//TODO Remove
	//burstPowerupPGroup->releasePos = D3DXVECTOR3(5,5,5);

	sound = new SoundManager();

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

	if(burstPowerupPGroup) {
		delete burstPowerupPGroup;
		burstPowerupPGroup = NULL;
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
	Gbls::extractorMesh.Destroy();
	Gbls::resourceMesh.Destroy();
	Gbls::powerupMesh.Destroy();
	for(int i = 0; i < Gbls::numAsteroidMeshes; i++) {
		Gbls::asteroidMesh[0].Destroy();
	}
	Gbls::mothershipMesh.Destroy();
	Gbls::shipMesh.Destroy();

	Skybox::releaseSkybox();
}

HRESULT GameResources::reInitState() {
	HRESULT hres;
	
	curCam->updateProjection();

	curCam->updateView();

	// create the vertices for blitting a texture to screen
	float left = -0.5;
	float top = -0.5;
	float right = ((float) Gbls::thePresentParams.BackBufferWidth) - 0.5f;
	float bottom = ((float) Gbls::thePresentParams.BackBufferHeight) - 0.5f;

	
//static CUSTOMQUAD bloomQuad    = fsQuad;
//static CUSTOMQUAD scoreScreenQuad1 = fsQuad;
//static CUSTOMQUAD scoreScreenQuad2 = fsQuad;
//static CUSTOMQUAD scoreScreenQuad3 = fsQuad;
//static CUSTOMQUAD scoreScreenQuad4 = fsQuad;

	UINT screenWidth = Gbls::thePresentParams.BackBufferWidth;
	UINT screenHeight = Gbls::thePresentParams.BackBufferHeight;

	CUSTOMQUAD tmpQuad_fsQuad =
    {
		{ right, top,    0.5f, 1.0f, 1.0f, 0.0f },
		{ right, bottom, 0.5f, 1.0f, 1.0f, 1.0f },
		{ left,  top,    0.5f, 1.0f, 0.0f, 0.0f },
		{ left,  bottom, 0.5f, 1.0f, 0.0f, 1.0f },
    };

	float bRight = ((float) screenWidth) / 2.0f - 0.5f;
	float bBottom = ((float) screenHeight) / 2.0f - 0.5f;
	CUSTOMQUAD tmpQuad_bloomQuad =
    {
		{ bRight, top,    0.5f, 1.0f, 1.0f, 0.0f },
		{ bRight, bBottom, 0.5f, 1.0f, 1.0f, 1.0f },
		{ left,  top,    0.5f, 1.0f, 0.0f, 0.0f },
		{ left,  bBottom, 0.5f, 1.0f, 0.0f, 1.0f },
    };
	float screenAR = ((float)screenWidth)/screenHeight;
	float scoreScreenAR = 16.0f/9.0f;
	float scoreScreenWidth, scoreScreenHeight;
	float scoreScreenPercent = 0.8f;
	if ( screenAR > scoreScreenAR) { //screen wider than scoreScreen
		scoreScreenHeight = (float) screenHeight;
		scoreScreenWidth = scoreScreenHeight*scoreScreenAR;
		//float spacing = (screenWidth - scoreScreenWidth) / 2.0f;
		//left = left + spacing;
		//right = right - spacing;
	} else { //screen narrower than scoreScreen
		scoreScreenWidth = (float) screenWidth;
		scoreScreenHeight = scoreScreenWidth/scoreScreenAR;
		//scoreScreenHeight = screenHeight*(1.0f/(scoreScreenAR/screenAR));
		//float spacing = (screenHeight - scoreScreenHeight) / 2.0f;
		//top = top + spacing;
		//bottom = bottom - spacing;
	}
	float scoreScreenTexW = 1920;
	float scoreScreenTexH = 1080;
	float hs = scoreScreenTexH / 4.0f; // height spacing (height of a single score panel in pixels)
	float sRight = left + scoreScreenTexW;
	
	CUSTOMQUAD tmpQuad_scoreScreenQuad1 =
    {
		{ sRight, top,    0.5f, 1.0f, 1.0f, 0.0f },
		{ sRight, top + hs, 0.5f, 1.0f, 1.0f, 1.0f },
		{ left,   top,    0.5f, 1.0f, 0.0f, 0.0f },
		{ left,   top + hs, 0.5f, 1.0f, 0.0f, 1.0f },
    };
	CUSTOMQUAD tmpQuad_scoreScreenQuad2 =
    {
		{ sRight, top + hs,      0.5f, 1.0f, 1.0f, 0.0f },
		{ sRight, top + hs*2.0f, 0.5f, 1.0f, 1.0f, 1.0f },
		{ left,   top + hs,      0.5f, 1.0f, 0.0f, 0.0f },
		{ left,   top + hs*2.0f, 0.5f, 1.0f, 0.0f, 1.0f },
    };
	CUSTOMQUAD tmpQuad_scoreScreenQuad3 =
    {
		{ sRight, top + hs*2.0f, 0.5f, 1.0f, 1.0f, 0.0f },
		{ sRight, top + hs*3.0f, 0.5f, 1.0f, 1.0f, 1.0f },
		{ left,   top + hs*2.0f, 0.5f, 1.0f, 0.0f, 0.0f },
		{ left,   top + hs*3.0f, 0.5f, 1.0f, 0.0f, 1.0f },
    };
	CUSTOMQUAD tmpQuad_scoreScreenQuad4 =
    {
		{ sRight, top + hs*3.0f, 0.5f, 1.0f, 1.0f, 0.0f },
		{ sRight, top + hs*4.0f, 0.5f, 1.0f, 1.0f, 1.0f },
		{ left,   top + hs*3.0f, 0.5f, 1.0f, 0.0f, 0.0f },
		{ left,   top + hs*4.0f, 0.5f, 1.0f, 0.0f, 1.0f },
    };
    
    float widthSpacing = ((screenWidth - scoreScreenWidth) / 2.0f) + screenWidth*((1.0f-scoreScreenPercent)/2.0f);
	float heightSpacing = ((screenHeight - scoreScreenHeight) / 2.0f) + screenHeight*((1.0f-scoreScreenPercent)/2.0f);;


	CUSTOMQUAD tmpQuad_scoreScreenQuadMain =
    {
		{ right - widthSpacing, top    + heightSpacing, 0.5f, 1.0f, 1.0f, 0.0f },
		{ right - widthSpacing, bottom - heightSpacing, 0.5f, 1.0f, 1.0f, 1.0f },
		{ left  + widthSpacing, top    + heightSpacing, 0.5f, 1.0f, 0.0f, 0.0f },
		{ left  + widthSpacing, bottom - heightSpacing, 0.5f, 1.0f, 0.0f, 1.0f },
    };

	scoreScreenAR = 16.0f/10.0f;
	if ( screenAR > scoreScreenAR) { //screen wider than scoreScreen
		scoreScreenHeight = (float) screenHeight;
		scoreScreenWidth = scoreScreenHeight*scoreScreenAR;
	} else { //screen narrower than scoreScreen
		scoreScreenWidth = (float) screenWidth;
		scoreScreenHeight = scoreScreenWidth/scoreScreenAR;
	}
	scoreScreenPercent = 1.0f;
	widthSpacing = ((screenWidth - scoreScreenWidth) / 2.0f) + screenWidth*((1.0f-scoreScreenPercent)/2.0f);
	heightSpacing = ((screenHeight - scoreScreenHeight) / 2.0f) + screenHeight*((1.0f-scoreScreenPercent)/2.0f);

	CUSTOMQUAD tmpQuad_splashScreenQuadMain =
    {
		{ right - widthSpacing, top    + heightSpacing, 0.5f, 1.0f, 1.0f, 0.0f },
		{ right - widthSpacing, bottom - heightSpacing, 0.5f, 1.0f, 1.0f, 1.0f },
		{ left  + widthSpacing, top    + heightSpacing, 0.5f, 1.0f, 0.0f, 0.0f },
		{ left  + widthSpacing, bottom - heightSpacing, 0.5f, 1.0f, 0.0f, 1.0f },
    };

    
    
 	float alert_scale = sqrt((float)(Gbls::thePresentParams.BackBufferWidth * Gbls::thePresentParams.BackBufferHeight))/sqrt((float)(1920*1080));
	float alert_scale_h = (float) Gbls::thePresentParams.BackBufferHeight/1080;
	float alert_scale_w = (float) Gbls::thePresentParams.BackBufferWidth/1920;
	float alert_size = 64*alert_scale;
	const float alert_skew_h = -400;
	const float alert_skew_w = 0;
	float alert_left = (Gbls::thePresentParams.BackBufferWidth/2 - 0.5f) + alert_scale_w*alert_skew_w - alert_size/2;
	float alert_right = (alert_left + alert_size);
	float alert_bottom = (((float) Gbls::thePresentParams.BackBufferHeight) - 0.5f) + alert_scale_h*alert_skew_h;
	float alert_top = alert_bottom - alert_size;	

	CUSTOMQUAD tmpQuad_alertQuad =
    {
		{ alert_right, alert_top,    0.5f, 1.0f, 1.0f, 0.0f },
		{ alert_right, alert_bottom, 0.5f, 1.0f, 1.0f, 1.0f },
		{ alert_left,  alert_top,    0.5f, 1.0f, 0.0f, 0.0f },
		{ alert_left,  alert_bottom, 0.5f, 1.0f, 0.0f, 1.0f },
    };


	const float icon_scale = 1.0f; //alert_scale;
	float icon_size = 260*icon_scale;
	float icon_scale_h = alert_scale_h;
	float icon_scale_w = alert_scale_w;
	const float icon_skew_h = -75;
	const float icon_skew_w = 0;
	float icon_left;
	float icon_right;
	float icon_bottom = (((float) Gbls::thePresentParams.BackBufferHeight) - 0.5f) + icon_skew_h;
	float icon_top = icon_bottom - icon_size;

	float icon_fract_array[4] = {
		1.0f/9.0f,
		3.0f/9.0f,
		6.0f/9.0f,
		8.0f/9.0f
	};

	for(unsigned int i = 0; i < 4; i++) {
		icon_left = icon_fract_array[i]*(Gbls::thePresentParams.BackBufferWidth - 0.5f) + icon_scale_w*icon_skew_w - icon_size/2;
		icon_right = icon_left + icon_size;
		CUSTOMQUAD tmp =
		{
			{ icon_right, icon_top,    0.5f, 1.0f, 1.0f, 0.0f },
			{ icon_right, icon_bottom, 0.5f, 1.0f, 1.0f, 1.0f },
			{ icon_left,  icon_top,    0.5f, 1.0f, 0.0f, 0.0f },
			{ icon_left,  icon_bottom, 0.5f, 1.0f, 0.0f, 1.0f },
		};
		playerIconHudQuad[i] = tmp;		
	}


	fsQuad = tmpQuad_fsQuad;
	scoreScreenQuad[0] = tmpQuad_scoreScreenQuad1;
	scoreScreenQuad[1] = tmpQuad_scoreScreenQuad2;
	scoreScreenQuad[2] = tmpQuad_scoreScreenQuad3;
	scoreScreenQuad[3] = tmpQuad_scoreScreenQuad4;
	playerSplashQuad = tmpQuad_splashScreenQuadMain;
	alertQuad = tmpQuad_alertQuad;
	scoreScreenQuadMain = tmpQuad_scoreScreenQuadMain;

	bloomQuad = tmpQuad_bloomQuad;

	

	//// create a vertex buffer interface called v_buffer
	//if (!fsQuadVBuffer) {
	//	Gbls::pd3dDevice->CreateVertexBuffer(4*sizeof(CUSTOMVERTEX),
	//							   0,
	//							   TEXTOSCREENFVF,
	//							   D3DPOOL_MANAGED,
	//							   &fsQuadVBuffer,
	//							   NULL);
	//}
 //   VOID* pVoid;    // a void pointer
 //   // lock v_buffer and load the vertices into it
 //   fsQuadVBuffer->Lock(0, 0, (void**)&pVoid, 0);
 //   std::memcpy(pVoid, vertices1, sizeof(vertices1));
 //   fsQuadVBuffer->Unlock();
	//
	//
	//right = ((float) Gbls::thePresentParams.BackBufferWidth) / 2.0f - 0.5f;
	//bottom = ((float) Gbls::thePresentParams.BackBufferHeight) / 2.0f - 0.5f;
	//CUSTOMVERTEX vertices2[] =
 //   {
	//	{ right, top,    0.5f, 1.0f, 1.0f, 0.0f },
	//	{ right, bottom, 0.5f, 1.0f, 1.0f, 1.0f },
	//	{ left,  top,    0.5f, 1.0f, 0.0f, 0.0f },
	//	{ left,  bottom, 0.5f, 1.0f, 0.0f, 1.0f },
 //   };
	//// create a vertex buffer interface called v_buffer
	//if (!bloomQuadVBuffer) {
	//	Gbls::pd3dDevice->CreateVertexBuffer(4*sizeof(CUSTOMVERTEX),
	//							   0,
	//							   TEXTOSCREENFVF,
	//							   D3DPOOL_MANAGED,
	//							   &bloomQuadVBuffer,
	//							   NULL);
	//}
 //   // lock v_buffer and load the vertices into it
 //   bloomQuadVBuffer->Lock(0, 0, (void**)&pVoid, 0);
 //   std::memcpy(pVoid, vertices2, sizeof(vertices2));
 //   bloomQuadVBuffer->Unlock();
	//

	// Create score screen texture
	if (pScoreScreenTexture) {
		pScoreScreenTexture->Release();
		pScoreScreenTexture = NULL;
	}
	Gbls::pd3dDevice->CreateTexture(
		scoreScreenTexW, //1920, //(UINT) scoreScreenWidth,
		scoreScreenTexH, //1080, //(UINT) scoreScreenHeight,
        1,
        D3DUSAGE_RENDERTARGET,
        /*D3DFMT_X8R8G8B8, /*/D3DFMT_A8R8G8B8, //D3DFMT_R5G6B5,
        D3DPOOL_DEFAULT,
        &pScoreScreenTexture,
        NULL);

	// Create glowmap texture
	if (pGlowmapTexture) {
		pGlowmapTexture->Release();
		pGlowmapTexture = NULL;
	}
	Gbls::pd3dDevice->CreateTexture(
		Gbls::thePresentParams.BackBufferWidth / 2,
		Gbls::thePresentParams.BackBufferHeight / 2,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DFMT_R5G6B5,
        D3DPOOL_DEFAULT,
        &pGlowmapTexture,
        NULL);

		
	// Create glowmap texture
	if (pTmpBlurTexture) {
		pTmpBlurTexture->Release();
		pTmpBlurTexture = NULL;
	}
	Gbls::pd3dDevice->CreateTexture(
		Gbls::thePresentParams.BackBufferWidth / 2,
		Gbls::thePresentParams.BackBufferHeight / 2,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DFMT_R5G6B5,
        D3DPOOL_DEFAULT,
        &pTmpBlurTexture,
        NULL);

	// Create default render texture
	if (pDefaultRenderTexture) {
		pDefaultRenderTexture->Release();
		pDefaultRenderTexture = NULL;
	}
	Gbls::pd3dDevice->CreateTexture(
		Gbls::thePresentParams.BackBufferWidth,
		Gbls::thePresentParams.BackBufferHeight,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DFMT_R5G6B5,
        D3DPOOL_DEFAULT,
        &pDefaultRenderTexture,
        NULL);

	// Tell the device to automatically normalize surface normals to keep them normal after scaling
	//Gbls::pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// Create lights for scene and set light properties
	//hres = GameResources::initLights();
	//if(FAILED (hres))
		//return hres;

	// init particle system vertex buffer
	hres = partSystem->init(Gbls::pd3dDevice);
	if(FAILED (hres))
		return hres;



	//set backface cullling off TODO remove after models are fixed
	//Gbls::pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	Gbls::pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	
	return S_OK;
}

HRESULT GameResources::loadEffect(ID3DXEffect ** pEffect, std::wstring effectLoc) {
	//init effect file
	DWORD shaderFlags = 0;
    //shaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    //shaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    shaderFlags |= D3DXSHADER_NO_PRESHADER;

	ID3DXBuffer *pBuffer = NULL;
	HRESULT hres = D3DXCreateEffectFromFile(Gbls::pd3dDevice,
		effectLoc.c_str(),
        NULL, // CONST D3DXMACRO* pDefines,
        NULL, // LPD3DXINCLUDE pInclude,
        shaderFlags, 
		NULL, // LPD3DXEFFECTPOOL pPool,
        pEffect,
        &pBuffer );
	if (FAILED(hres)) {
		std::wostringstream str;
		if(pBuffer) {
			char * bufData = (char *) pBuffer->GetBufferPointer();
			str << L"Failed to load effect file " << effectLoc << L": " << bufData;
		} else {
			str << L"Failed to load effect file " << effectLoc << L": Check shader filepath.";
		}
        MessageBox( NULL, str.str().c_str(), L"CRUSH.exe", MB_OK );
		return hres;
	}
	return S_OK;
}

HRESULT GameResources::initMeshes()
{
	HRESULT hres;

	//do for all needed meshes
	if(FAILED(hres = Gbls::shipMesh.CreateBlank(Gbls::shipMeshFilepath)))
		return hres;
	if(FAILED(hres = Gbls::mothershipMesh.CreateBlank(Gbls::mothershipMeshFilepath)))
		return hres;
	for (int i = 0; i < Gbls::numAsteroidMeshes; i++) {
		if(FAILED(hres = Gbls::asteroidMesh[i].Create(Gbls::asteroidMeshFilepath[i])))
			return hres;
	}
	if(FAILED(hres = Gbls::resourceMesh.Create(Gbls::resourceMeshFilepath)))
			return hres;
	if(FAILED(hres = Gbls::extractorMesh.Create(Gbls::extractorMeshFilepath)))
			return hres;
	if(FAILED(hres = Gbls::powerupMesh.CreateBlank(Gbls::powerupMeshFilepath)))
			return hres;
	
	if (FAILED(hres = D3DXCreateSphere(Gbls::pd3dDevice, 3.0f, 40, 40, &sunMesh, NULL)))
		return hres;
	if (FAILED(hres = D3DXCreateSphere(Gbls::pd3dDevice, 1.0f, 25, 25, &shieldMesh, NULL)))
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
	//HRESULT hres = D3DXCreateTextureFromFileEx(Gbls::pd3dDevice, filepath.c_str(), D3DX_DEFAULT, D3DX_DEFAULT,
	//	D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
	//	0, NULL, NULL, pTextureOut);

	HRESULT hres = D3DXCreateTextureFromFile( Gbls::pd3dDevice,
                                                    filepath.c_str(),
                                                    pTextureOut );
	if (FAILED(hres)) {
		MessageBox( NULL, (L"Could not find "+filepath).c_str(), L"CRUSH.exe", MB_OK );
        return hres;
	}
	return S_OK;
}

HRESULT GameResources::loadTextureWithFormat(LPDIRECT3DTEXTURE9 * pTextureOut, std::wstring filepath, _D3DFORMAT format) {
	//HRESULT hres = D3DXCreateTextureFromFileEx(
	//	Gbls::pd3dDevice,
	//	filepath.c_str(),
	//	D3DX_DEFAULT,
	//	D3DX_DEFAULT,
	//	D3DX_DEFAULT,
	//	0,
	//	/*D3DFMT_UNKNOWN,*/format,
	//	D3DPOOL_MANAGED,
	//	D3DX_DEFAULT,
	//	D3DX_DEFAULT,
	//	0,
	//	NULL,
	//	NULL,
	//	pTextureOut
	//);

	HRESULT hres = D3DXCreateTextureFromFileEx(
		Gbls::pd3dDevice,
		filepath.c_str(),
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_FROM_FILE,
		0,
		D3DFMT_FROM_FILE,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		0,
		NULL,
		NULL,
		pTextureOut
	);

	if (FAILED(hres)) {
		std::wstring tmpStr = L"loadTextureWithFormat() failed. Error: " + Util::DXErrorToString(hres);
		MessageBox( NULL, tmpStr.c_str(), L"CRUSH.exe", MB_OK );
		//MessageBox( NULL, (L"Could not find "+filepath).c_str(), L"CRUSH.exe", MB_OK );
        return hres;
	}
	return S_OK;
}

HRESULT GameResources::initAdditionalTextures()
{
	HRESULT hres;

	_D3DFORMAT compressedFormat = D3DFMT_DXT3;
	
	// load powerup texture
	hres = loadTextureWithFormat(&Gbls::powerupTexture, Gbls::powerupTextureFilepath, compressedFormat);
	if (FAILED(hres)) {
		return hres;
	}
	// load ship textures
	hres = loadTextureWithFormat(&Gbls::shipTexture1, Gbls::shipTexFilepath1, compressedFormat);
	if (FAILED(hres)) {
		return hres;
	}
	//D3DSURFACE_DESC desc;
	//Gbls::shipTexture1->GetLevelDesc(0, &desc);
	hres = loadTextureWithFormat(&Gbls::shipTexture2, Gbls::shipTexFilepath2, compressedFormat);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTextureWithFormat(&Gbls::shipTexture3, Gbls::shipTexFilepath3, compressedFormat);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTextureWithFormat(&Gbls::shipTexture4, Gbls::shipTexFilepath4, compressedFormat);
	if (FAILED(hres)) {
		return hres;
	}

	// load ship textures
	hres = loadTextureWithFormat(&Gbls::mothershipTexture1, Gbls::mothershipTexFilepath1, compressedFormat);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTextureWithFormat(&Gbls::mothershipTexture2, Gbls::mothershipTexFilepath2, compressedFormat);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTextureWithFormat(&Gbls::mothershipTexture3, Gbls::mothershipTexFilepath3, compressedFormat);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTextureWithFormat(&Gbls::mothershipTexture4, Gbls::mothershipTexFilepath4, compressedFormat);
	if (FAILED(hres)) {
		return hres;
	}

	// load score screen sprites
	hres = loadTextureWithFormat(&Gbls::scoreScreenTexture[0], Gbls::scoreScreenTexFilepath1, compressedFormat);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTextureWithFormat(&Gbls::scoreScreenTexture[1], Gbls::scoreScreenTexFilepath2, compressedFormat);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTextureWithFormat(&Gbls::scoreScreenTexture[2], Gbls::scoreScreenTexFilepath3, compressedFormat);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTextureWithFormat(&Gbls::scoreScreenTexture[3], Gbls::scoreScreenTexFilepath4, compressedFormat);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTextureWithFormat(&Gbls::pScoreScrenAlphaTexture, Gbls::pScoreScrenAlphaTexFilepath, compressedFormat);
	if (FAILED(hres)) {
		return hres;
	}

	//load player HUD sprites
	hres = loadTexture(&playerHudArray_insig[0], Gbls::player1HudFilepath_insig);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&playerHudArray_insig[1], Gbls::player2HudFilepath_insig);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&playerHudArray_insig[2], Gbls::player3HudFilepath_insig);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&playerHudArray_insig[3], Gbls::player4HudFilepath_insig);
	if (FAILED(hres)) {
		return hres;
	}


	// load arrow spirte
	hres = loadTexture(&shipEIDTexture_resource, Gbls::shipEIDTextureFilepath_resource);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&ship1EIDTexture_insig, Gbls::ship1EIDTextureFilepath_insig);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&ship1EIDTexture_arrow, Gbls::ship1EIDTextureFilepath_arrow);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&ship2EIDTexture_insig, Gbls::ship2EIDTextureFilepath_insig);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&ship2EIDTexture_arrow, Gbls::ship2EIDTextureFilepath_arrow);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&ship3EIDTexture_insig, Gbls::ship3EIDTextureFilepath_insig);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&ship3EIDTexture_arrow, Gbls::ship3EIDTextureFilepath_arrow);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&ship4EIDTexture_insig, Gbls::ship4EIDTextureFilepath_insig);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&ship4EIDTexture_arrow, Gbls::ship4EIDTextureFilepath_arrow);
	if (FAILED(hres)) {
		return hres;
	}

	hres = loadTexture(&alertTexture, Gbls::alertTextureFilepath);
	if (FAILED(hres)) {
		return hres;
	}
	

	//hres = loadTexture(&extractorEIDTextureOnScreen, Gbls::extractorEIDTextureOnScreenFilepath);
	//if (FAILED(hres)) {
	//	return hres;
	//}
	hres = loadTexture(&extractorEIDTextureOffScreen_arrow, Gbls::extractorEIDTextureOffScreenFilepath_arrow);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&extractorEIDTextureOffScreen_insig, Gbls::extractorEIDTextureOffScreenFilepath_insig);
	if (FAILED(hres)) {
		return hres;
	}


	//load splash screens

	hres = loadTexture(&playerSplashTexture[0], Gbls::player1SplashTextureFilepath);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&playerSplashTexture[1], Gbls::player2SplashTextureFilepath);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&playerSplashTexture[2], Gbls::player3SplashTextureFilepath);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&playerSplashTexture[3], Gbls::player4SplashTextureFilepath);
	if (FAILED(hres)) {
		return hres;
	}


	//load powerup reps
	hres = loadTexture(&powerupConsumedTexture[0], Gbls::consumedPowerupTexture1Filepath);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&powerupConsumedTexture[1], Gbls::consumedPowerupTexture2Filepath);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&powerupConsumedTexture[2], Gbls::consumedPowerupTexture3Filepath);
	if (FAILED(hres)) {
		return hres;
	}

	hres = loadTexture(&powerupTextureArray[0], Gbls::powerupTexture1Filepath);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&powerupTextureArray[1], Gbls::powerupTexture2Filepath);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&powerupTextureArray[2], Gbls::powerupTexture3Filepath);
	if (FAILED(hres)) {
		return hres;
	}
	
	// load ship arrow spirte
	hres = loadTexture(&mothershipEIDTexture_arrow, Gbls::mothershipEIDTextureFilepath_arrow);
	if (FAILED(hres)) {
		return hres;
	}
	hres = loadTexture(&mothershipEIDTexture_insig, Gbls::mothershipEIDTextureFilepath_insig);
	if (FAILED(hres)) {
		return hres;
	}

	hres = loadTexture(&resourceEIDTexture, Gbls::resourceEIDTextureFilepath);
	if (FAILED(hres)) {
		return hres;
	}

	// load tractor beam particle spirte
	hres = loadTexture(&tBeamPartTexture, Gbls::tBeamPartTexFilepath);
	if (FAILED(hres)) {
		return hres;
	}

	// load normal engine particle spirte
	hres = loadTexture(&EnginePartNormTexture, Gbls::enginePartTexNormFilepath);
	if (FAILED(hres)) {
		return hres;
	}

	// load boost engine particle spirte
	hres = loadTexture(&EnginePartSpeedupTexture, Gbls::enginePartTexSpeedupFilepath);
	if (FAILED(hres)) {
		return hres;
	}

	return S_OK;
}

void GameResources::releaseAdditionalTextures() {
	if (pScoreScreenTexture) {
		pScoreScreenTexture->Release();
		pScoreScreenTexture = NULL;
	}
	if (pGlowmapTexture) { //actually init in reInitState
		pGlowmapTexture->Release();
		pGlowmapTexture = NULL;
	}
	if (pTmpBlurTexture) { //actually init in reInitState
		pTmpBlurTexture->Release();
		pTmpBlurTexture = NULL;
	}
	if (pDefaultRenderTexture) { //actually init in reInitState
		pDefaultRenderTexture->Release();
		pDefaultRenderTexture = NULL;
	}
	if (tBeamPartTexture) {
		tBeamPartTexture->Release();
		tBeamPartTexture = NULL;
	}
	if (mothershipEIDTexture_arrow) {
		mothershipEIDTexture_arrow->Release();
		mothershipEIDTexture_arrow = NULL;
	}
	if (mothershipEIDTexture_insig) {
		mothershipEIDTexture_insig->Release();
		mothershipEIDTexture_insig = NULL;
	}

	shipEIDTexture_resource = NULL;
	if (shipEIDTexture_resource) {
		shipEIDTexture_resource->Release();
		shipEIDTexture_resource = NULL;
	}

	if(resourceEIDTexture) {
		resourceEIDTexture->Release();
		resourceEIDTexture = NULL;
	}

	if(extractorEIDTextureOnScreen) {
		extractorEIDTextureOnScreen->Release();
		extractorEIDTextureOnScreen = NULL;
	}

	if(extractorEIDTextureOffScreen_arrow) {
		extractorEIDTextureOffScreen_arrow->Release();
		extractorEIDTextureOffScreen_arrow = NULL;
	}
	if(extractorEIDTextureOffScreen_insig) {
		extractorEIDTextureOffScreen_insig->Release();
		extractorEIDTextureOffScreen_insig = NULL;
	}

	if(shipEIDTexture_resource) {
		shipEIDTexture_resource->Release();
		shipEIDTexture_resource = NULL;
	}

	for(unsigned int i = 0; i < 4; i++) {
		if(playerSplashTexture[i]) {
			playerSplashTexture[i]->Release();
			playerSplashTexture[i] = NULL;
		}
	}

	for(unsigned int i = 0; i < 4; i++) {
		if(playerHudArray_insig[i]) {
			playerHudArray_insig[i]->Release();
			playerHudArray_insig[i] = NULL;
		}
	}

	for(unsigned int i = 0; i < 3; i++) {
		if(powerupConsumedTexture[i]) {
			powerupConsumedTexture[i]->Release();
			powerupConsumedTexture[i] = NULL;
		}
		if(powerupTextureArray[i]) {
			powerupTextureArray[i]->Release();
			powerupTextureArray[i] = NULL;
		}
	}
	
	for(unsigned int i = 0; i < 4; i++) {
		if(*shipEIDTextureArray_arrow[i]){
			(*shipEIDTextureArray_arrow[i])->Release();
			(*shipEIDTextureArray_arrow[i]) = NULL;
		}
	}

	for(unsigned int i = 0; i < 4; i++) {
		if(*shipEIDTextureArray_insig[i]) {
			(*shipEIDTextureArray_insig[i])->Release();
			(*shipEIDTextureArray_insig[i]) = NULL;
		}
	}
}

HRESULT GameResources::initLights() {
	// Tell the device not to do any dynamic lighting
	//Gbls::pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// turn on specular highlights
	//Gbls::pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

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
	//Gbls::pd3dDevice->SetMaterial(&MATERIAL_WHITE);

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
}

void GameResources::releaseBurstPowerupParticles() {
	for (UINT i = 0; i < shipList.size(); i++) {
		C_Ship * ship = shipList[i];
		if(ship->m_hasPowerup && ship->m_powerupType == PULSE && ship->m_powerupStateType == CONSUMED) {
			GameResources::burstPowerupPGroup->releasePos = ship->m_pos;
			GameResources::partSystem->releaseBurst(GameResources::burstPowerupPGroup);
		}
	}
}

void GameResources::drawAllShields() {
	if (shieldVisToggle && playerShip) {
		playerShip->m_hasPowerup = TRUE;
		playerShip->m_powerupType = SHIELD;
		playerShip->m_powerupStateType = CONSUMED;
	}
	for (UINT i = 0; i < shipList.size(); i++) {
		C_Ship * ship = shipList[i];
		if(ship->m_hasPowerup && ship->m_powerupType == SHIELD && ship->m_powerupStateType == CONSUMED) {
			drawShield(ship);
		}
	}
}

void GameResources::drawAllTractorBeams() {
	// render particles
	for (UINT i = 0; i < tractorBeamList.size(); i++) {
		if(tractorBeamList[i]->m_isOn) {
			tBeamPGroup->m_curColor = shipColorsInt[i];
			tBeamPGroup->tBeamEnt = tractorBeamList[i];
			tBeamPGroup->updateGroup();
			partSystem->render(Gbls::pd3dDevice, tBeamPGroup);
		}
	}
}

void GameResources::drawAllEngines() {
	// render particles
	if (speedupVisToggle && playerShip) {
		playerShip->m_hasPowerup = TRUE;
		playerShip->m_powerupType = SPEEDUP;
		playerShip->m_powerupStateType = CONSUMED;
	}
	for (UINT i = 0; i < enginePGroupList.size(); i++) {
		enginePGroupList[i]->updateGroup();
		partSystem->render(Gbls::pd3dDevice, enginePGroupList[i]);
	}
}

void GameResources::drawAllPowerups() {
	powerupPGroup;
	// render particles
	for (UINT i = 0; i < powerupList.size(); i++) {
		powerupPGroup->pEnt = powerupList[i];
		powerupPGroup->updateGroup();
		partSystem->render(Gbls::pd3dDevice, powerupPGroup);
	}
}

void GameResources::drawAllEID() {

	HRESULT hResult = pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	if(SUCCEEDED(hResult)) {
		for (UINT i = 0; i < eIDList.size(); i++) {
			//TODO add this back in when ID's work correctly
			if(debugCamOn || eIDList[i]->targetEntity != playerShip) {
				//if (eIDList[i]->targetEntity != motherShip || playership->HAS_RESOURCE) {
					eIDList[i]->draw(curCam, pd3dSprite);
				//}
			}
		}
		pd3dSprite->End();
	}

}

void GameResources::placeTextCenter(LPCWSTR str, UINT x, UINT y) {
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
		
	pd3dSprite->SetTransform(&mat);
	RECT rect = {0, 0, 0, 0};
	pd3dFont->DrawText(pd3dSprite, str, -1, &rect, DT_CALCRECT,
		NULL);
	rect.top += y - (rect.bottom/2);;
	rect.bottom += y - (rect.bottom/2);;
	rect.left += x - (rect.right/2);
	rect.right += x - (rect.right/2);
	pd3dFont->DrawText(pd3dSprite, str, -1, &rect, DT_CENTER | DT_WORDBREAK,
		D3DCOLOR_XRGB(255, 255, 255));
}

void GameResources::placeTextCenterCeiling(LPCWSTR str, UINT x) {
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
		
	pd3dSprite->SetTransform(&mat);
	RECT rect = {0, 0, 0, 0};
	pd3dFont->DrawText(pd3dSprite, str, -1, &rect, DT_CALCRECT,
		NULL);
	rect.left += x - (rect.right/2);
	rect.right += x - (rect.right/2);
	pd3dFont->DrawText(pd3dSprite, str, -1, &rect, DT_CENTER | DT_WORDBREAK,
		D3DCOLOR_XRGB(255, 255, 255));
}

void GameResources::placeTextCenterFloor(LPCWSTR str, UINT x) {
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
		
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

void GameResources::drawFlashingSprite(Sprite const &sprite, CUSTOMQUAD &location, long long const &time, unsigned int const &flash_period) {
	static long long last_time = timeGetTime();
	static bool draw_on = true;
	unsigned int elapsed_time = (unsigned int) (time - last_time);
	if(elapsed_time < flash_period/2) {
		Gbls::pd3dDevice->EndScene();
		Gbls::pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		drawTexToSurface(sprite.m_pTexture, &location, true);
		Gbls::pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		Gbls::pd3dDevice->BeginScene();	
	} else if(elapsed_time >= flash_period) {
		last_time = timeGetTime();
	}
}

void GameResources::drawStaticHudElements() {
	HRESULT hres = pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	if(SUCCEEDED(hres))
	{
		if(playerShip && playerShip->m_hasPowerup) {
			Sprite powerupSprite;
			if(playerShip->m_powerupStateType == CONSUMED) {
				powerupSprite.m_pTexture = powerupConsumedTexture[playerShip->m_powerupType];
			} else {
				powerupSprite.m_pTexture = powerupTextureArray[playerShip->m_powerupType];
			}
			powerupSprite.setCenterToTopRight();
			float scale = (float)(sqrt((float)(Gbls::thePresentParams.BackBufferWidth * Gbls::thePresentParams.BackBufferHeight))/sqrt((float)(1920*1080)));
			D3DXVECTOR2 transV, centerV(powerupSprite.m_vCenter.x, powerupSprite.m_vCenter.y), scaleV(scale,scale);
			transV.x = (float)(Gbls::thePresentParams.BackBufferWidth - powerupSprite.m_vCenter.x);
			transV.y = (float)(Gbls::thePresentParams.BackBufferHeight/10.0);
			D3DXMATRIX mat;
			D3DXMatrixIdentity(&mat);
			D3DXMatrixTransformation2D(&mat, &centerV, 0.0f, &scaleV, NULL, 0.0f, &transV);
			D3DXVECTOR3 centerV3(powerupSprite.m_vCenter.x, powerupSprite.m_vCenter.y, 0);
			pd3dSprite->SetTransform(&mat);
			pd3dSprite->Draw(powerupSprite.m_pTexture, NULL, &centerV3, NULL, 0XFFFFFFFF);
		}
		if(playerShip && playerShip->m_isLockOnTarget) { 
			Sprite alertSprite;
			alertSprite.m_pTexture = alertTexture;
			drawFlashingSprite(alertSprite, alertQuad, timeGetTime(), 250);
		}
		Gbls::pd3dDevice->EndScene();
		for(unsigned int i = 0; i < 4; i++) {
			drawTexToSurface(playerHudArray_insig[i], &playerIconHudQuad[i], true);
		}
		Gbls::pd3dDevice->BeginScene();	
		placeTextCenterCeiling(timeStr.c_str(), Gbls::thePresentParams.BackBufferWidth/2);
		placeTextCenterFloor((L"Player 1\n" + std::to_wstring((long long)playerScore[0])).c_str(), (UINT) (Gbls::thePresentParams.BackBufferWidth * (1.0f/9.0f)));
		placeTextCenterFloor((L"Player 2\n" + std::to_wstring((long long)playerScore[1])).c_str(), (UINT) (Gbls::thePresentParams.BackBufferWidth * (3.0f/9.0f)));
		placeTextCenterFloor((L"Player 3\n" + std::to_wstring((long long)playerScore[2])).c_str(), (UINT) (Gbls::thePresentParams.BackBufferWidth * (6.0f/9.0f)));
		placeTextCenterFloor((L"Player 4\n" + std::to_wstring((long long)playerScore[3])).c_str(), (UINT) (Gbls::thePresentParams.BackBufferWidth * (8.0f/9.0f)));
		// End sprite rendering
		pd3dSprite->End();
	}
}

void GameResources::drawModel(C_Entity * cEnt) {
	D3DXMATRIX tmp;
	pEffectDefault->SetMatrix("World", &cEnt->worldMat);
	float det = D3DXMatrixDeterminant(&cEnt->worldMat);
	D3DXMatrixTranspose(&tmp, D3DXMatrixInverse(&tmp, &det, &cEnt->worldMat));
	
	pEffectDefault->SetMatrix("WorldInverseTranspose", &tmp);
	pEffectDefault->SetTexture("ModelTexture", cEnt->m_pMesh->m_pMeshTextures[0]);
	pEffectDefault->CommitChanges();
	cEnt->m_pMesh->m_pMesh->DrawSubset(0);
}

static void drawFastModel(C_Entity * cEnt, ID3DXEffect * pEffect) {
	pEffect->SetMatrix("World", &cEnt->worldMat);
	pEffect->CommitChanges();
	cEnt->m_pMesh->m_pMesh->DrawSubset(0);
}

static void drawGlowmapSpecModel(C_Entity * cEnt, ID3DXEffect * pEffect) {
	D3DXMATRIX tmp;
	pEffect->SetMatrix("World", &cEnt->worldMat);
	float det = D3DXMatrixDeterminant(&cEnt->worldMat);
	D3DXMatrixTranspose(&tmp, D3DXMatrixInverse(&tmp, &det, &cEnt->worldMat));
	
	pEffect->SetMatrix("WorldInverseTranspose", &tmp);
	pEffect->CommitChanges();
	cEnt->m_pMesh->m_pMesh->DrawSubset(0);
}

void GameResources::drawAllModels() {
	
	Gbls::pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//Gbls::pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	D3DXMATRIX tmp;
	pEffectDefault->SetMatrix("View", curCam->getViewMatrix(tmp));
	pEffectDefault->SetMatrix("Projection", curCam->getProjMatrix(tmp)); 
	D3DXVECTOR4 viewVec = curCam->m_vAt - curCam->m_vEye;
	D3DXVec4Normalize(&viewVec, &viewVec);
	pEffectDefault->SetVector("ViewVector", &viewVec);
	
    pEffectDefault->SetTechnique( "Shiny" ); // used to be shiny, handled in glowmap shader now
	UINT cPasses;
	pEffectDefault->Begin(&cPasses, 0);
	for (UINT iPass = 0; iPass < cPasses; iPass++)
	{
		pEffectDefault->BeginPass(iPass);
		for (UINT i = 0; i < shipList.size(); i++) {
			drawModel(shipList[i]);
		}
		for (UINT i = 0; i < mothershipList.size(); i++) {
			drawModel(mothershipList[i]);
		}
		for (UINT i = 0; i < resourceList.size(); i++) {
			drawModel(resourceList[i]);
		}
		for (UINT i = 0; i < extractorList.size(); i++) {
			drawModel(extractorList[i]);
		}
		for (UINT i = 0; i < powerupList.size(); i++) {
			drawModel(powerupList[i]);
		}
//begin
 // used to be the only dull section, but now shineyness is handled in the glowmap shader
		pEffectDefault->EndPass();
	}
	pEffectDefault->End();

	
    pEffectDefault->SetTechnique( "Dull" );
	pEffectDefault->Begin(&cPasses, 0);
	for (UINT iPass = 0; iPass < cPasses; iPass++)
	{
		pEffectDefault->BeginPass(iPass);
//end
		for (UINT i = 0; i < asteroidList.size(); i++) {
			drawModel(asteroidList[i]);
		}
		pEffectDefault->EndPass();
	}
	pEffectDefault->End();
	
	Gbls::pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Gbls::pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Gbls::pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (playerShip) {
		pEffectDefault->SetTechnique("Shield");
		pEffectDefault->Begin(&cPasses, 0);
		for (UINT iPass = 0; iPass < cPasses; iPass++)
		{
			pEffectDefault->BeginPass(iPass);
			drawAllShields();
			pEffectDefault->EndPass();
		}
		pEffectDefault->End();
	}

	//Gbls::pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	Gbls::pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	Gbls::pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//Gbls::pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void GameResources::DrawBGSun() {
	UINT cPasses;
	D3DXMATRIX tmp;

	GameResources::curCam->setCenteredView();
	pEffectGlowmap->SetMatrix("View", curCam->getViewMatrix(tmp));
	pEffectGlowmap->SetMatrix("Projection", curCam->getProjMatrix(tmp));
	
	Gbls::pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE ); // sun is behind ALL
    pEffectGlowmap->SetTechnique( "Emissive" );
	cPasses;
	pEffectGlowmap->Begin(&cPasses, 0);
	//D3DXMATRIX scaleMat;
	//float scale = 1.0f;
	//D3DXMatrixScaling(&scaleMat, scale*1.01f, scale*0.98f, scale*1.01f);
	for (UINT iPass = 0; iPass < cPasses; iPass++)
	{
		pEffectGlowmap->BeginPass(iPass);
		pEffectGlowmap->SetMatrix("World", &(/*scaleMat*/sunWorldMat));
		pEffectGlowmap->CommitChanges();
		sunMesh->DrawSubset(0);
		pEffectGlowmap->EndPass();
	}
	pEffectGlowmap->End();
	Gbls::pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE ); // sun is behind ALL

	GameResources::curCam->updateView();
}

void GameResources::createGlowmap() {
	
    Gbls::pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );

	//render-to-texture
	//set new render target
	Gbls::pd3dDevice->SetRenderTarget(0,pGlowmapSurface);
	Gbls::pd3dDevice->Clear(0,
                           NULL,
                           D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                           D3DCOLOR_XRGB(0,0,0),
                           1.0f,
                           0);
	HRESULT hResult = Gbls::pd3dDevice->BeginScene();
	if(FAILED(hResult))
	{
		std::wstring tmpStr = L"BeginScene() failed. Error: " + Util::DXErrorToString(hResult);
		MessageBox( NULL, tmpStr.c_str(), L"CRUSH.exe", MB_OK );
		return;
	}

	UINT cPasses;
	D3DXMATRIX tmp;

	GameResources::curCam->setCenteredView();
	pEffectGlowmap->SetMatrix("View", curCam->getViewMatrix(tmp));
	pEffectGlowmap->SetMatrix("Projection", curCam->getProjMatrix(tmp));
	D3DXVECTOR4 viewVec = curCam->m_vAt - curCam->m_vEye;
	D3DXVec4Normalize(&viewVec, &viewVec);
	pEffectGlowmap->SetVector("ViewVector", &viewVec);
	
	Gbls::pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE ); // sun is behind ALL
    pEffectGlowmap->SetTechnique( "Emissive" );
	cPasses;
	pEffectGlowmap->Begin(&cPasses, 0);
	for (UINT iPass = 0; iPass < cPasses; iPass++)
	{
		pEffectGlowmap->BeginPass(iPass);
		pEffectGlowmap->SetMatrix("World", &sunWorldMat);
		pEffectGlowmap->CommitChanges();
		sunMesh->DrawSubset(0);
		pEffectGlowmap->EndPass();
	}
	pEffectGlowmap->End();
	
	Gbls::pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE ); // sun is behind ALL
	GameResources::curCam->updateView();
	pEffectGlowmap->SetMatrix("View", curCam->getViewMatrix(tmp));

    pEffectGlowmap->SetTechnique( "Black" );
	cPasses;
	pEffectGlowmap->Begin(&cPasses, 0);
	for (UINT iPass = 0; iPass < cPasses; iPass++)
	{
		pEffectGlowmap->BeginPass(iPass);
		for (UINT i = 0; i < powerupList.size(); i++) {
			drawFastModel(powerupList[i], pEffectGlowmap);
		}
		for (UINT i = 0; i < asteroidList.size(); i++) {
			drawFastModel(asteroidList[i], pEffectGlowmap);
		}
		pEffectGlowmap->EndPass();
	}
	pEffectGlowmap->End();

	pEffectGlowmap->SetTechnique( "Specular" );
	cPasses;
	pEffectGlowmap->Begin(&cPasses, 0);
	for (UINT iPass = 0; iPass < cPasses; iPass++)
	{
		pEffectGlowmap->BeginPass(iPass);
		for (UINT i = 0; i < shipList.size(); i++) {
			drawGlowmapSpecModel(shipList[i], pEffectGlowmap);
		}
		for (UINT i = 0; i < mothershipList.size(); i++) {
			drawGlowmapSpecModel(mothershipList[i], pEffectGlowmap);
		}
		for (UINT i = 0; i < resourceList.size(); i++) {
			drawGlowmapSpecModel(resourceList[i], pEffectGlowmap);
		}
		for (UINT i = 0; i < extractorList.size(); i++) {
			drawGlowmapSpecModel(extractorList[i], pEffectGlowmap);
		}
		pEffectGlowmap->EndPass();
	}
	pEffectGlowmap->End();

	
 //   Gbls::pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE ); // sun is behind ALL
	//GameResources::curCam->setCenteredView();
	//pEffectGlowmap->SetMatrix("View", curCam->getViewMatrix(tmp));
 //   pEffectGlowmap->SetTechnique( "Emissive" );
	//cPasses;
	//pEffectGlowmap->Begin(&cPasses, 0);
	//for (UINT iPass = 0; iPass < cPasses; iPass++)
	//{
	//	pEffectGlowmap->BeginPass(iPass);
	//	pEffectGlowmap->SetMatrix("World", &sunWorldMat);
	//	pEffectGlowmap->CommitChanges();
	//	sunMesh->DrawSubset(0);
	//	pEffectGlowmap->EndPass();
	//}
	//pEffectGlowmap->End();
	//GameResources::curCam->updateView();

	Gbls::pd3dDevice->EndScene();
	
	// return to normal state
    Gbls::pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	//Gbls::pd3dDevice->SetRenderTarget(0,pBackBuffer);
}

void GameResources::blurTexture(LPDIRECT3DTEXTURE9 tex1, LPDIRECT3DTEXTURE9 tex2) {
	// Turn off Z-Buff
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	LPDIRECT3DSURFACE9 surface;
	tex1->GetSurfaceLevel(0, &surface);
	D3DSURFACE_DESC desc;
	surface->GetDesc(&desc);
	float texelSize[2] = {1.0f/desc.Width, 1.0f/desc.Height};
	surface->Release();

	// First Pass
    pEffectBlur->SetTechnique( "Horizontal" );
	tex2->GetSurfaceLevel(0, &surface);
	Gbls::pd3dDevice->SetRenderTarget(0,surface);

	HRESULT hResult = Gbls::pd3dDevice->BeginScene();
	if(FAILED(hResult))
	{
		std::wstring tmpStr = L"BeginScene() failed. Error: " + Util::DXErrorToString(hResult);
		MessageBox( NULL, tmpStr.c_str(), L"CRUSH.exe", MB_OK );
		return;
	}
	

	pEffectBlur->SetTexture("ModelTexture", tex1);
	//pEffectBlur->SetInt("Orientation", 0);
	pEffectBlur->SetFloatArray("TexelSize", texelSize, 2);

	UINT cPasses;

	pEffectBlur->Begin(&cPasses, 0);
	for (UINT iPass = 0; iPass < cPasses; iPass++) {
		pEffectBlur->BeginPass(iPass);
		Gbls::pd3dDevice->SetFVF(TEXTOSCREENFVF);
		//Gbls::pd3dDevice->SetStreamSource(0, bloomQuadVBuffer, 0, sizeof(CUSTOMVERTEX));
		//Gbls::pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		Gbls::pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (void *) &bloomQuad, sizeof(CUSTOMVERTEX));
		pEffectBlur->EndPass();
	}
	pEffectBlur->End();
    Gbls::pd3dDevice->EndScene();
	surface->Release();
	
	// Second Pass
	
    pEffectBlur->SetTechnique( "Vertical" );
	tex1->GetSurfaceLevel(0, &surface);
	Gbls::pd3dDevice->SetRenderTarget(0,surface);
	hResult = Gbls::pd3dDevice->BeginScene();
	pEffectBlur->SetTexture("ModelTexture", tex2);
	//pEffectBlur->SetInt("Orientation", 1);
	if(FAILED(hResult))
	{
		std::wstring tmpStr = L"BeginScene() failed. Error: " + Util::DXErrorToString(hResult);
		MessageBox( NULL, tmpStr.c_str(), L"CRUSH.exe", MB_OK );
		return;
	}
	pEffectBlur->Begin(&cPasses, 0);
	for (UINT iPass = 0; iPass < cPasses; iPass++) {
		pEffectBlur->BeginPass(iPass);
		Gbls::pd3dDevice->SetFVF(TEXTOSCREENFVF);
		//Gbls::pd3dDevice->SetStreamSource(0, bloomQuadVBuffer, 0, sizeof(CUSTOMVERTEX));
		//Gbls::pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		Gbls::pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (void *) &bloomQuad, sizeof(CUSTOMVERTEX));
		pEffectBlur->EndPass();
	}
	pEffectBlur->End();
    Gbls::pd3dDevice->EndScene();
	surface->Release();

	// Turn on Z-Buff 
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

void GameResources::blendTexesToSurface(LPDIRECT3DTEXTURE9 tex1, LPDIRECT3DTEXTURE9 tex2, CUSTOMQUAD * pQuad, bool copyAlpha) {
	if (copyAlpha) { // copy color from tex1 and alpha channel from tex2
		pEffectBlend->SetTechnique("CopyAlpha");
	} else { // blend colors and alphas from tex1 & tex2 additively
		pEffectBlend->SetTechnique("AdditiveBlend");
	}
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
   	HRESULT hResult = Gbls::pd3dDevice->BeginScene();
	if(FAILED(hResult))
	{
		std::wstring tmpStr = L"BeginScene() failed. Error: " + Util::DXErrorToString(hResult);
		MessageBox( NULL, tmpStr.c_str(), L"CRUSH.exe", MB_OK );
		return;
	}
	
	pEffectBlend->SetTexture("ModelTexture1", tex1);
	pEffectBlend->SetTexture("ModelTexture2", tex2);

	UINT cPasses;
	pEffectBlend->Begin(&cPasses, 0);
	for (UINT iPass = 0; iPass < cPasses; iPass++) {
		pEffectBlend->BeginPass(iPass);
		Gbls::pd3dDevice->SetFVF(TEXTOSCREENFVF);
		//Gbls::pd3dDevice->SetStreamSource(0, quadVBuffer, 0, sizeof(CUSTOMVERTEX));
		//Gbls::pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		Gbls::pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pQuad, sizeof(CUSTOMVERTEX));
		pEffectBlend->EndPass();
	}
	pEffectBlend->End();
    Gbls::pd3dDevice->EndScene();

	
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void GameResources::drawTexToSurface(LPDIRECT3DTEXTURE9 tex, CUSTOMQUAD * pQuad, bool useTexAlpha) {

	if (useTexAlpha) {
		pEffectTexToScreen->SetTechnique("UseTexAlpha");
	} else {
		pEffectTexToScreen->SetTechnique("SolidAlpha");
	}

	//Gbls::pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
   	HRESULT hResult = Gbls::pd3dDevice->BeginScene();
	if(FAILED(hResult))
	{
		std::wstring tmpStr = L"BeginScene() failed. Error: " + Util::DXErrorToString(hResult);
		MessageBox( NULL, tmpStr.c_str(), L"CRUSH.exe", MB_OK );
		return;
	}

	pEffectTexToScreen->SetTexture("ModelTexture", tex);

	UINT cPasses;
	pEffectTexToScreen->Begin(&cPasses, 0);
	for (UINT iPass = 0; iPass < cPasses; iPass++) {
		pEffectTexToScreen->BeginPass(iPass);
		Gbls::pd3dDevice->SetFVF(TEXTOSCREENFVF);
		//Gbls::pd3dDevice->SetStreamSource(0, quadVBuffer, 0, sizeof(CUSTOMVERTEX));
		//Gbls::pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		Gbls::pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pQuad, sizeof(CUSTOMVERTEX));
		pEffectTexToScreen->EndPass();
	}
	pEffectTexToScreen->End();
    Gbls::pd3dDevice->EndScene();

	
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void GameResources::drawShield(C_Ship * ship) {
	D3DXMATRIX tmp;
	float scale = 7.0;
	D3DXMatrixScaling(&tmp, scale, scale, scale);
	tmp *= ship->worldMat;
	pEffectDefault->SetMatrix("World", &tmp);
	float det = D3DXMatrixDeterminant(&tmp);
	D3DXMatrixTranspose(&tmp, D3DXMatrixInverse(&tmp, &det, &tmp));
	
	pEffectDefault->SetMatrix("WorldInverseTranspose", &tmp);
	pEffectDefault->SetFloatArray("ShieldColor", shipColors[ship->m_playerNum], 4);
	pEffectDefault->CommitChanges();
	shieldMesh->DrawSubset(0);
}

void GameResources::drawScoreScreen() {
	Gbls::pd3dDevice->SetRenderTarget(0,pScoreScreenSurface); // set render target back to back buf
	for (UINT i = 0; i < 4; i++) {
		drawTexToSurface(Gbls::scoreScreenTexture[winnerList[i].first], &scoreScreenQuad[i], false);
	}
	Gbls::pd3dDevice->SetRenderTarget(0,pBackBuffer); // set render target back to back buf
	Gbls::pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Gbls::pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Gbls::pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	blendTexesToSurface(pScoreScreenTexture, Gbls::pScoreScrenAlphaTexture, &scoreScreenQuadMain, true);
	//Gbls::pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//Gbls::pd3dDevice->SetRenderTarget(0,pScoreScreenSurface); // set render target back to back buf
	//Gbls::pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(100, 255, 100, 100), 0.0f, 0);
	/*D3DSURFACE_DESC desc;
	pScoreScreenTexture->GetLevelDesc(0, &desc);
	UINT h = desc.Height;
	UINT w = desc.Width;*/
	float top = scoreScreenQuadMain.topLeft.Y;
	float left = scoreScreenQuadMain.topLeft.X;
	float bottom = scoreScreenQuadMain.bottomRight.Y;
	float right = scoreScreenQuadMain.bottomRight.X;
	float height = bottom - top;
	float width = right - left;
	Gbls::pd3dDevice->BeginScene();
	HRESULT hres = pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	if(SUCCEEDED(hres))
	{
		std::wostringstream str;
		for (int i = 0; i < 4; i++) {
			str << L"Player " << winnerList[i].first + 1;
			placeTextCenter(str.str().c_str(), left + width*0.6f, top + (1.0f + i*2.0f)*(height/8.0f));
			str.str(L"");
			str << winnerList[i].second;
			placeTextCenter(str.str().c_str(), left + width*0.80f, top + (1.0f + i*2.0f)*(height/8.0f));
			str.str(L"");
		}
		pd3dSprite->End();
	}
	Gbls::pd3dDevice->EndScene();
	//Gbls::pd3dDevice->SetRenderTarget(0,pBackBuffer); // set render target back to back buf
	//drawTexToSurface(pScoreScreenTexture, &scoreScreenQuadMain, true);
}

void GameResources::drawAll()
{
	Gbls::pd3dDevice->GetRenderTarget(0,&pBackBuffer);
	pDefaultRenderTexture->GetSurfaceLevel(0,&pDefaultRenderSurface);
	pGlowmapTexture->GetSurfaceLevel(0,&pGlowmapSurface);
	pScoreScreenTexture->GetSurfaceLevel(0,&pScoreScreenSurface);
	
	//pDefaultRenderTexture->Release();

	
	createGlowmap();
	blurTexture(pGlowmapTexture, pTmpBlurTexture);

	//// Clear the screen
	//Gbls::pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
	//	D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);


	//render-to-texture
	//set new render target
	Gbls::pd3dDevice->SetRenderTarget(0,pDefaultRenderSurface);
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	Gbls::pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	Gbls::pd3dDevice->Clear(0,
                           NULL,
                           D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                           D3DCOLOR_XRGB(0,0,0),
                           1.0f,
                           0);

	// Tell the device we want to start rendering
	HRESULT hResult = Gbls::pd3dDevice->BeginScene();
	if(FAILED(hResult))
	{
		std::wstring tmpStr = L"BeginScene() failed. Error: " + Util::DXErrorToString(hResult);
		MessageBox( NULL, tmpStr.c_str(), L"CRUSH.exe", MB_OK );
		return;
	}

	Skybox::drawSkybox();
	DrawBGSun();

	// TODO this draws objects in no particular order, resulting in many loads and unloads (probably) for textures and models. Should be fixed if performance becomes an issue.
	// Loop through all lists. Set up shaders, etc, as needed for each.
	//for( map<int,C_Entity*>::iterator ii=entityMap.begin(); ii!=entityMap.end(); ++ii)
	//{
	//	(*ii).second->draw();
	//}

	drawAllModels();

	if (renderCBWireframe) {
		Gbls::pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
		Gbls::pd3dDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
		Gbls::pd3dDevice->SetMaterial(&MATERIAL_WHITE);
		for( map<int,C_Entity*>::iterator ii=entityMap.begin(); ii!=entityMap.end(); ++ii)
		{
			drawCollisionBounds((*ii).second->m_pFront, (*ii).second->m_pBack, (*ii).second->m_radius);
		}
		Gbls::pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
	}

	// set state for particle effects
	Gbls::pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	Gbls::pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    Gbls::pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    Gbls::pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	// Render tractor beams
	drawAllTractorBeams();

	//Render all engine exhausts
	drawAllEngines();

	drawAllPowerups();

	//Render burst push effect TODO fix to work with actual powerup
	partSystem->render(Gbls::pd3dDevice, burstPowerupPGroup);
	
	// unset state for particle effects
    Gbls::pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	Gbls::pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    Gbls::pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

	Gbls::pd3dDevice->EndScene();

	Gbls::pd3dDevice->SetRenderTarget(0,pBackBuffer); // set render target back to back buf
	blendTexesToSurface(pDefaultRenderTexture, pGlowmapTexture, &fsQuad, false);
	
	if (gameOver) { // draw winner screen
		drawScoreScreen();

	} else if (splashStart) {
		Gbls::pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Gbls::pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		Gbls::pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		drawTexToSurface(playerSplashTexture[playerNum], &playerSplashQuad, true);
		Gbls::pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	} else {
		Gbls::pd3dDevice->BeginScene();

		// Render entity indicators
		drawAllEID();

		// Render static hud elements
		drawStaticHudElements();

		Gbls::pd3dDevice->EndScene();

	}

	if (pGlowmapSurface) {
		pGlowmapSurface->Release();
		pGlowmapSurface = NULL;
	}
	if (pScoreScreenSurface) {
		pScoreScreenSurface->Release();
		pScoreScreenSurface = NULL;
	}

	if (pDefaultRenderSurface) {
		pDefaultRenderSurface->Release();
		pDefaultRenderSurface = NULL;
	}

	if (pBackBuffer) {
		pBackBuffer->Release();
		pBackBuffer = NULL;
	}
}

// called each frame to update the state of all game sounds
void GameResources::playSounds(vector<shared_ptr<GEvent> > events)
{
	if (sound->isValid) {
		//Engine sounds
		for (UINT i = 0; i < shipList.size(); i++) {
			sound->playEngine(shipList[i]);
		}

		//Tractor Beam Sounds
		for (UINT i = 0; i < tractorBeamList.size(); i++) {
			sound->playTractorBeam(tractorBeamList[i]);
		}

		for (UINT i = 0; i < events.size(); i++) {
			sound->playEvent(events[i]);
		}

		sound->cleanEvents();
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
	curCam->updateView();
	curCam->updateProjection();
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

static bool score_t_comp(pair<unsigned int, int> i, pair<unsigned int, int> j) {
	return (i.second > j.second);
}

void GameResources::updateGameState(GameState<Entity> & newGameState) {

	//double curTime = timeGetTime();
	long long curTime = newGameState.getServerTime();
	if (curTime < 0) return;
	static long long s_lastTime = curTime; // first time initialization, static otherwise
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
			entityMap[id]->updateWorldMat();
		}
		splashStart = newGameState.isShowSplash();
		if(newGameState.isGameOver()/* && !gameOver*/) {
			gameOver = TRUE;
			for (int i = 0; i < 4; i++) {
				winnerList[i].first = i;
				winnerList[i].second = playerScore[i];
			}
			sort(winnerList.begin(), winnerList.end(), score_t_comp);
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
	releaseBurstPowerupParticles();
	partSystem->update(tBeamPGroup, elapsedTime);
	partSystem->update(burstPowerupPGroup, elapsedTime);
	partSystem->update(powerupPGroup, elapsedTime);
	for (UINT i = 0; i < enginePGroupList.size(); i++) {
		partSystem->update(enginePGroupList[i], elapsedTime);
	}

	//Update the sound engine on the changes
	playSounds(newGameState.getEvents());
}

void GameResources::resetGameState() {
	gameOver = FALSE;
	playerShip = NULL;
	playerMothership = NULL;
	shipList.clear();
	asteroidList.clear();
	mothershipList.clear();
	tractorBeamList.clear();
	resourceList.clear();
	extractorList.clear();
	powerupList.clear();
	for (UINT i = 0; i < 4; i++) {
		playerScore[i] = 0;
	}
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
		EntityIdentifier * shipEID_insig = new EntityIdentifier();
		shipEID->targetEntity = tmp;
		shipEID_insig->targetEntity = tmp;
		shipEID_insig->rotateOn = false;

		shipEID->m_altOffScreenSprite.setTexture(shipEIDTexture_resource);
		shipEID->m_altOnScreenSprite.setTexture(shipEIDTexture_resource);
		shipEID->m_altOffScreenSprite.setCenterToTextureMidpoint();
		shipEID->m_altOnScreenSprite.setCenterToTextureMidpoint();
		shipEID->enableAltSprite = true;

		shipEID->m_onScreenSprite.setTexture(*shipEIDTextureArray_arrow[tmp->m_playerNum]);
		shipEID_insig->m_onScreenSprite.setTexture(*shipEIDTextureArray_insig[tmp->m_playerNum]);
		
		shipEID->m_onScreenSprite.setCenterToTextureMidpoint();
		shipEID_insig->m_onScreenSprite.setCenterToTextureMidpoint();
		
		shipEID->m_offScreenSprite.setTexture(*shipEIDTextureArray_arrow[tmp->m_playerNum]);
		shipEID_insig->m_offScreenSprite.setTexture(*shipEIDTextureArray_insig[tmp->m_playerNum]);
		
		shipEID->m_offScreenSprite.setCenterToTextureMidpoint();
		shipEID_insig->m_offScreenSprite.setCenterToTextureMidpoint();
		
		eIDList.push_back(shipEID);
		eIDList.push_back(shipEID_insig);
		EnginePGroup * epg = new EnginePGroup(EnginePartNormTexture, EnginePartSpeedupTexture);
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
		if (tmp->m_playerNum == playerNum) {
			playerMothership = tmp;
			EntityIdentifier * mShipEID = new EntityIdentifier();
			EntityIdentifier * mShipEID_insig = new EntityIdentifier();
			mShipEID->targetEntity = tmp;
			mShipEID->m_onScreenSprite.setTexture(mothershipEIDTexture_arrow);
			mShipEID->m_onScreenSprite.setCenterToTextureMidpoint();
			mShipEID->m_offScreenSprite.setTexture(mothershipEIDTexture_arrow);
			mShipEID->m_offScreenSprite.setCenterToTextureMidpoint();

			mShipEID_insig->targetEntity = tmp;
			mShipEID_insig->rotateOn = false;
			mShipEID_insig->m_onScreenSprite.setTexture(mothershipEIDTexture_insig);
			mShipEID_insig->m_onScreenSprite.setCenterToTextureMidpoint();
			mShipEID_insig->m_offScreenSprite.setTexture(mothershipEIDTexture_insig);
			mShipEID_insig->m_offScreenSprite.setCenterToTextureMidpoint();
		
			eIDList.push_back(mShipEID);
			eIDList.push_back(mShipEID_insig);
		}
		ret = tmp;
		}
		break;
	case RESOURCE :
		{
		C_Resource * tmp = new C_Resource(newEnt);
		EntityIdentifier * mResourceEID = new EntityIdentifier();
		mResourceEID->targetEntity = tmp;
		mResourceEID->m_onScreenSprite.setTexture(resourceEIDTexture);
		mResourceEID->m_onScreenSprite.setCenterToTextureMidpoint();
		mResourceEID->m_offScreenSprite.setTexture(NULL);
		//mResourceEID->m_offScreenSprite.setCenterToTextureMidpoint();
		eIDList.push_back(mResourceEID);
		resourceList.push_back(tmp);		
		ret = tmp;
		}
		break;
	case EXTRACTOR :
		{
		C_Extractor * tmp = new C_Extractor(newEnt);
		EntityIdentifier * mExtractorEID = new EntityIdentifier();
		EntityIdentifier * mExtractorEID_insig = new EntityIdentifier();

		mExtractorEID->targetEntity = tmp;
		mExtractorEID->m_onScreenSprite.setTexture(extractorEIDTextureOffScreen_arrow);
		mExtractorEID->m_onScreenSprite.setCenterToTextureMidpoint();
		mExtractorEID->m_offScreenSprite.setTexture(extractorEIDTextureOffScreen_arrow);
		mExtractorEID->m_offScreenSprite.setCenterToTextureMidpoint();

		mExtractorEID_insig->targetEntity = tmp;
		mExtractorEID_insig->rotateOn = false;
		mExtractorEID_insig->m_onScreenSprite.setTexture(extractorEIDTextureOffScreen_insig);
		mExtractorEID_insig->m_onScreenSprite.setCenterToTextureMidpoint();
		mExtractorEID_insig->m_offScreenSprite.setTexture(extractorEIDTextureOffScreen_insig);
		mExtractorEID_insig->m_offScreenSprite.setCenterToTextureMidpoint();
	
		eIDList.push_back(mExtractorEID);
		eIDList.push_back(mExtractorEID_insig);
		extractorList.push_back(tmp);
		ret = tmp;
		}
		break;
	case POWERUP :
		{
		C_Powerup * tmp = new C_Powerup(newEnt);
		powerupList.push_back(tmp);
		ret = tmp;
		}
		break;
	}
	//ret->updated = true; 	//used for individual deletes if we ever implement that
	return ret;
}