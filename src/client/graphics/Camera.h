//=================================================================================================
// Camera.h Camera class for storing view/projection matricies and appropriate functions
//=================================================================================================

#ifndef Camera_H_INCLUDED
#define Camera_H_INCLUDED

#define WIN32_LEAN_AND_MEAN

// Global includes
#include <d3dx9.h>

class Camera {
public:
	Camera();
	~Camera();

	// projection fields
	float m_fov;
	float m_nearPlane;
	float m_farPlane;

	// Used mostly for debug cam
	float m_yaw;
	float m_pitch;

	// View fields
	D3DXVECTOR3 m_vEye;      // Camera position
	D3DXVECTOR3 m_vAt;           // Camera look-at position
	D3DXVECTOR3 m_vUp;        // Camera "up" direction

	// Matricies
	D3DXMATRIX viewMat;
	D3DXMATRIX projMat;
	
	// Create the view matrix, and set it on the device
	void updateView();

	// Create the projection matrix, and set it on the device
	void updateProjection();


	// Set a location 0 view matrix with correct direction for use with skybox (and sun)
	void Camera::setCenteredView();
	
	D3DXMATRIX * Camera::getViewMatrix(D3DXMATRIX & matView);
	D3DXMATRIX * Camera::getProjMatrix(D3DXMATRIX & matProj);
};



#endif // Camera_H_INCLUDED
