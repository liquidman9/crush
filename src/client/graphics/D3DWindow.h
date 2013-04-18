//=================================================================================================
// D3DWindow.h - D3D Window wrapper class
//=================================================================================================

#ifndef D3DWINDOW_H_INCLUDED
#define D3DWINDOW_H_INCLUDED

// Defines
#define WIN32_LEAN_AND_MEAN

// Global includes
//#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

// Project includes
#include <client/Gbls.h>


class D3DWindow
{
public:
	// Create the window
	static bool Create(HINSTANCE hInstance);

	// Destroy this window. Window is automatically destroyed when destructor is called
	static void Destroy();

	// Run one update loop. Returns true of the app should quit
	static bool Tick();

	// Get the error message (If there is one)
	static const std::wstring& GetError() { return s_strError; }

	
	static HRESULT SetupState();

private:
	//TODO remove
	/*struct Vertex
	{
		Vertex() {}
		Vertex(float x, float y, float z, D3DCOLOR dwColour, float u, float v) :
			vPos(x, y, z), dwColour(dwColour), uv(u, v) {}

		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

		D3DXVECTOR3 vPos;
		D3DCOLOR dwColour;
		D3DXVECTOR2 uv;
	};*/

	// Static and non-static window procedures
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	static bool InitD3DDevice(const SIZE& sizeBackBuffer);
	static void ShutdownD3DDevice();
	static void DrawFrame();
	//static void ToggleFullscreen();
	static HRESULT HandlePresentRetval(HRESULT hResult);
	static void OnLostDevice();
	static void OnResetDevice();

	static HINSTANCE s_hInstance;                         // Instance handle used to create the window
	static HWND s_hWnd;                                   // Window handle
	//static LPDIRECT3D9 s_pD3D;                            // D3D interface pointer
	//static LPDIRECT3DDEVICE9 s_pDevice;                   // D3D device pointer
	//static D3DPRESENT_PARAMETERS s_thePresentParams;      // Present params used to create the device
	//static LPDIRECT3DVERTEXBUFFER9 m_pVB;                 // The buffer containing our vertices
	//static LPDIRECT3DINDEXBUFFER9 m_pIB;                  // The buffer containing our indices
	//static LPDIRECT3DTEXTURE9 m_pTexture;                 // The loaded texture

	static std::wstring s_strError;
	static bool s_bQuit;
	static bool s_bActive;
	static bool s_bDeviceLost;
	static DWORD s_dwStartTime;
	//static POINT m_ptWindowPreFullscreen;
};



#endif // D3DWINDOW_H_INCLUDED
