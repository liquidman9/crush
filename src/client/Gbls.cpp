#include <client/Gbls.h>

LPDIRECT3D9 Gbls::pD3D = NULL;
LPDIRECT3DDEVICE9 Gbls::pd3dDevice = NULL;
D3DPRESENT_PARAMETERS Gbls::thePresentParams;

float Gbls::debugCamMoveSpeed = 0.5f;
float Gbls::debugCamTurnSpeed = 2.0f;

float Gbls::debugCamMaxPitch = D3DXToRadian(89.0f);
float Gbls::debugCamMinPitch = D3DXToRadian(-89.0f);

Mesh Gbls::shipMesh1;
Mesh Gbls::shipMesh2;

// Should be changed to load in from a config file later
std::wstring Gbls::shipMeshFilepath_1 = L"player_ship_001.x";
std::wstring Gbls::shipMeshFilepath_2 = L"tiny.x";

std::wstring Gbls::skyboxTextureFilepath_Front = L"SkyBox_Front.jpg";
std::wstring Gbls::skyboxTextureFilepath_Back = L"SkyBox_Back.jpg";
std::wstring Gbls::skyboxTextureFilepath_Left = L"SkyBox_Left.jpg";
std::wstring Gbls::skyboxTextureFilepath_Right = L"SkyBox_Right.jpg";
std::wstring Gbls::skyboxTextureFilepath_Top = L"SkyBox_Top.jpg";
std::wstring Gbls::skyboxTextureFilepath_Bottom = L"SkyBox_Bottom.jpg";

D3DXCOLOR Gbls::lightDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
D3DXCOLOR Gbls::lightSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
D3DXVECTOR3 Gbls::lightDirection(1.0f, 0.5f, 1.0f);