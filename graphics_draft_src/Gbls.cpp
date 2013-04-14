#include "Gbls.h"

LPDIRECT3D9 Gbls::pD3D = NULL;
LPDIRECT3DDEVICE9 Gbls::pd3dDevice = NULL;
D3DPRESENT_PARAMETERS Gbls::thePresentParams;

float Gbls::debugCamMoveSpeed = 0.5f;
float Gbls::debugCamTurnSpeed = 2.0f;

float Gbls::debugCamMaxPitch = D3DXToRadian(89.0f);
float Gbls::debugCamMinPitch = D3DXToRadian(-89.0f);

Mesh Gbls::shipMesh1;
Mesh Gbls::shipMesh2;