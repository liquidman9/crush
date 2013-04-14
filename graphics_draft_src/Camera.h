//=================================================================================================
// Camera.h Camera class for storing view/projection matricies and appropriate functions
//=================================================================================================

#ifndef Camera_H_INCLUDED
#define Camera_H_INCLUDED

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
	
	// Create the projection matrix, and set it on the device
	void updateProjection();

	// Create the view matrix, and set it on the device
	void updateView();
};



#endif // Camera_H_INCLUDED
