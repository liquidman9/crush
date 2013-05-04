//=================================================================================================
// Camera.h Camera class for storing view/projection matricies and appropriate functions
//=================================================================================================

// Project includes
#include <client/graphics/Camera.h>
#include <client/Gbls.h>

Camera::Camera() :
	m_fov(45),
	m_nearPlane(0.1f),
	m_farPlane(100000.0f),
	m_vEye(0, 0, -3.0f),
	m_vAt(0, 0, 0),
	m_vUp(0, 1.0f, 0),
	m_yaw(0.0f),
	m_pitch(0.0f)
{
}

Camera::~Camera() 
{
}

// Create the projection matrix, and set it on the device
void Camera::updateProjection() {
	D3DXMATRIXA16 matProj;
	float fAspect = (float)Gbls::thePresentParams.BackBufferWidth /
		(float)Gbls::thePresentParams.BackBufferHeight;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(m_fov), fAspect, m_nearPlane, m_farPlane);
	Gbls::pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

// Create the view matrix, and set it on the device
void Camera::updateView() {
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vAt, &m_vUp);
	Gbls::pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
}

D3DXMATRIX * Camera::getViewMatrix(D3DXMATRIX & matView) {
	return D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vAt, &m_vUp);
}

D3DXMATRIX * Camera::getProjMatrix(D3DXMATRIX & matView) {
	float fAspect = (float)Gbls::thePresentParams.BackBufferWidth /
		(float)Gbls::thePresentParams.BackBufferHeight;
	return D3DXMatrixPerspectiveFovLH(&matView, D3DXToRadian(m_fov), fAspect, m_nearPlane, m_farPlane);
}

void Camera::setSkyboxView() {
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 vZeroEye(0.0f, 0.0f, 0.0f);

	D3DXMatrixLookAtLH(&matView, &vZeroEye, &(m_vAt-m_vEye), &m_vUp);
	Gbls::pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
}
