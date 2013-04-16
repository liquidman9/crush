//=================================================================================================
// D3DWindow.cpp - D3D Window wrapper class
//=================================================================================================

// Global includes
#include <sstream>

// Project includes
#include <client/graphics/D3DWindow.h>
#include <client/graphics/Util.h>
#include <client/GameResources.h>

#pragma comment(lib, "d3d9.lib")
#ifdef _DEBUG
#	pragma comment(lib, "d3dx9d.lib")
#else
#	pragma comment(lib, "d3dx9.lib")
#endif

//initialize class statics
HINSTANCE D3DWindow::s_hInstance = NULL;
HWND D3DWindow::s_hWnd = NULL;
//LPDIRECT3D9 D3DWindow::s_pD3D = NULL;
//LPDIRECT3DDEVICE9 D3DWindow::s_pd3dDevice = NULL;
//D3DPRESENT_PARAMETERS D3DWindow::s_thePresentParams;
std::wstring D3DWindow::s_strError;
bool D3DWindow::s_bQuit = false;
bool D3DWindow::s_bActive = false;
bool D3DWindow::s_bDeviceLost = false;
DWORD D3DWindow::s_dwStartTime = 0;
//POINT s_ptWindowPreFullscreen;

//consts
static const wchar_t* s_WindowClassname = L"MainWindowClass";
static const wchar_t* s_WindowTitle = L"Game Window";

static const int s_WindowWidth = 640;
static const int s_WindowHeight = 480;

//=================================================================================================
// D3DX does not currently provide an operator* for aligned matrices, only for unaligned ones.
// Using the default D3DXMATRIX::operator*() will give the following warning in the debug output on
// SSE2 capable CPUs, since the temporary matrix, matT will not be 16-byte aligned:
// D3DX: Matrix should be 16-byte aligned for better performance
D3DXINLINE D3DXMATRIXA16 operator*(CONST D3DXMATRIXA16& matA, CONST D3DXMATRIXA16& matB)
{
	D3DXMATRIXA16 matT;
	D3DXMatrixMultiply(&matT, &matA, &matB);
	return matT;
}

//=================================================================================================

//D3DWindow::D3DWindow() :
//	m_hInstance(NULL),
//	m_hWnd(NULL),
//	m_pD3D(NULL),
//	m_pd3dDevice(NULL),
//	m_pVB(NULL),
//	m_pIB(NULL),
//	m_pTexture(NULL),
//	m_bQuit(false),
//	m_bActive(false),
//	m_bDeviceLost(false),
//	m_dwStartTime(0)
//{
//	memset(&s_thePresentParams, 0, sizeof(s_thePresentParams));
//}

//D3DWindow::~D3DWindow()
//{
//	Destroy();
//}

//=================================================================================================

bool D3DWindow::Create(HINSTANCE hInstance)
{
	// Register window class
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = StaticWndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wc.lpszClassName = s_WindowClassname;
	wc.hIconSm = wc.hIcon;
	if(!RegisterClassEx(&wc))
	{
		s_strError = L"RegisterClassEx() failed. Error: " +
			Util::Win32ErrorToString(GetLastError());
		return false;
	}

	// Get the window size for the requested client area size
	DWORD dwStyle = (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~WS_THICKFRAME;
	SIZE sizeWindow;
	RECT rc;
	rc.top = rc.left = 0;
	rc.right = s_WindowWidth;
	rc.bottom = s_WindowHeight;
	AdjustWindowRect(&rc, dwStyle, FALSE);
	sizeWindow.cx = rc.right - rc.left;
	sizeWindow.cy = rc.bottom - rc.top;

	// Create window
	s_hWnd = CreateWindow(s_WindowClassname, s_WindowTitle, dwStyle, CW_USEDEFAULT,
		CW_USEDEFAULT, sizeWindow.cx, sizeWindow.cy, NULL, NULL, hInstance, NULL);
	if(!s_hWnd)
	{
		s_strError = L"CreateWindowEx() failed. Error: " +
			Util::Win32ErrorToString(GetLastError());
		UnregisterClass(s_WindowClassname, hInstance);
		return false;
	}

	// Init D3D device
	SIZE sizeBackBuffer;
	sizeBackBuffer.cx = s_WindowWidth;
	sizeBackBuffer.cy = s_WindowHeight;
	if(!InitD3DDevice(sizeBackBuffer))
	{
		Destroy();
		return false;
	}

	// Done - copy HINSTANCE variable and record current time
	s_hInstance = hInstance;
	s_dwStartTime = GetTickCount();
	return true;
}

void D3DWindow::Destroy()
{
	// Always clean up D3D before destroying the window it's using!
	ShutdownD3DDevice();

	// Cleanup window handle
	if(s_hWnd)
		DestroyWindow(s_hWnd);
}

//=================================================================================================

bool D3DWindow::Tick()
{
	// See if the window is being destroyed
	if(!IsWindow(s_hWnd) || s_bQuit)
		return s_bQuit;

	//// See if the window isn't in focus. If not, free up some CPU time.
	//BOOL bMinimized = IsIconic(m_hWnd);
	//if(!m_bActive || bMinimized)
	//{
	//	// Put this thread to sleep for 50ms
	//	Sleep(50);

	//	// If we're minimized, we can't call TestCooperativeLevel() (It'll fail)
	//	if(bMinimized)
	//		return m_bQuit;
	//}

	// Test cooperative level first
	HRESULT hResult = Gbls::pd3dDevice->TestCooperativeLevel();
	if(FAILED(hResult))
	{
		// Delegate to HandlePresentRetval and see if it knows how to handle it
		hResult = HandlePresentRetval(hResult);
		if(FAILED(hResult))
		{
			s_strError = L"TestCooperativeLevel() failed. Error: " + Util::DXErrorToString(hResult);
			s_bQuit = true;
			return s_bQuit;
		}

		// Are we in a non-renderable state?
		else if(hResult == S_FALSE)
		{
			return s_bQuit;
		}
	}

	// Render
	DrawFrame();
	return s_bQuit;
}

//=================================================================================================

LRESULT CALLBACK D3DWindow::StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Get pointer to window
	//D3DWindow* pParent;
	//if(uMsg == WM_NCCREATE)
	//{
	//	// This is the WM_NCCREATE message, so we can grab the "Extra" parameter and save
	//	// it as extra data for this HWND handle.
	//	pParent = (D3DWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
	//	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)(LONG_PTR)pParent);
	//}
	//else
	//{
	//	// See if we have a stored pointer for this HWND handle. If not, then this is a
	//	// message that's come along before WM_NCCREATE, so just do default handling.
	//	pParent = (D3DWindow*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	//	if(!pParent)
	//		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	//}

	//pParent->s_hWnd = hWnd;

	switch(uMsg)
	{

	// Handle window input cases
	
	case WM_DESTROY:
		s_bQuit = true;
		break;

	//case WM_KEYDOWN:
	//	if(wParam == VK_RETURN)
	//		ToggleFullscreen();
	//	else if(wParam == VK_ESCAPE)
	//		m_bQuit = true;
	//	break;

	//case WM_SIZE:
	//	if(wParam == SIZE_MAXIMIZED)
	//		ToggleFullscreen();
	//	break;

	case WM_ACTIVATE:
		s_bActive = (wParam != WA_INACTIVE);
		break;

	case WM_ENTERSIZEMOVE:
		SetTimer(s_hWnd, 1, USER_TIMER_MINIMUM, NULL);
		break;

	case WM_EXITSIZEMOVE:
		KillTimer(s_hWnd, 1);
		break;

	case WM_TIMER:
		DrawFrame();
		break;
	
	// Handle keyboard input cases

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_UP:
			GameResources::m_ks.upDown = true;
			break;

		case VK_LEFT:
			GameResources::m_ks.leftDown = true;
			break;

		case VK_DOWN:
			GameResources::m_ks.downDown = true;
			break;
			
		case VK_RIGHT:
			GameResources::m_ks.rightDown = true;
			break;
			
		case 'W':
			GameResources::m_ks.wDown = true;
			break;

		case 'A':
			GameResources::m_ks.aDown = true;
			break;

		case 'S':
			GameResources::m_ks.sDown = true;
			break;

		case 'D':
			GameResources::m_ks.dDown = true;
			break;

		}
		break;

	case WM_KEYUP:
		switch(wParam)
		{
		case VK_UP:
			GameResources::m_ks.upUp = true;
			GameResources::m_ks.upDown = false;
			break;

		case VK_LEFT:
			GameResources::m_ks.leftUp = true;
			GameResources::m_ks.leftDown = false;
			break;

		case VK_DOWN:
			GameResources::m_ks.downUp = true;
			GameResources::m_ks.downDown = false;
			break;
			
		case VK_RIGHT:
			GameResources::m_ks.rightUp = true;
			GameResources::m_ks.rightDown = false;
			break;
			
		case 'W':
			GameResources::m_ks.wUp = true;
			GameResources::m_ks.wDown = false;
			break;

		case 'A':
			GameResources::m_ks.aUp = true;
			GameResources::m_ks.aDown = false;
			break;

		case 'S':
			GameResources::m_ks.sUp = true;
			GameResources::m_ks.sDown = false;
			break;

		case 'D':
			GameResources::m_ks.dUp = true;
			GameResources::m_ks.dDown = false;
			break;

		}
		break;

	}


	return DefWindowProc(hWnd, uMsg, wParam, lParam);
	//return pParent->WndProc(uMsg, wParam, lParam);
}
//
//LRESULT D3DWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch(uMsg)
//	{
//	case WM_DESTROY:
//		m_bQuit = true;
//		break;
//
//	case WM_KEYDOWN:
//		if(wParam == VK_RETURN)
//			ToggleFullscreen();
//		else if(wParam == VK_ESCAPE)
//			m_bQuit = true;
//		break;
//
//	case WM_SIZE:
//		if(wParam == SIZE_MAXIMIZED)
//			ToggleFullscreen();
//		break;
//
//	case WM_ACTIVATE:
//		m_bActive = (wParam != WA_INACTIVE);
//		break;
//
//	case WM_ENTERSIZEMOVE:
//		SetTimer(m_hWnd, 1, USER_TIMER_MINIMUM, NULL);
//		break;
//
//	case WM_EXITSIZEMOVE:
//		KillTimer(m_hWnd, 1);
//		break;
//
//	case WM_TIMER:
//		DrawFrame();
//		break;
//	}
//
//	return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
//}

//=================================================================================================

bool D3DWindow::InitD3DDevice(const SIZE& sizeBackBuffer)
{
	// First, get a D3D pointer
	Gbls::pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(!Gbls::pD3D)
	{
		s_strError = L"Direct3DCreate9() failed. Error: " + Util::DXErrorToString(GetLastError());
		return false;
	}

	// Grab the current desktop format
	D3DFORMAT fmtBackbuffer;
	D3DDISPLAYMODE mode;
	HRESULT hResult = Gbls::pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
	if(FAILED(hResult))
	{
		s_strError = L"GetAdapterDisplayMode() failed. Error: " + Util::DXErrorToString(hResult);
		Gbls::pD3D->Release();
		Gbls::pD3D = NULL;
		return false;
	}
	fmtBackbuffer = mode.Format;

	// Need to see if this format is ok as a backbuffer format in this adapter mode
	hResult = Gbls::pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		mode.Format, D3DUSAGE_RENDERTARGET, D3DRTYPE_SURFACE, fmtBackbuffer);
	if(FAILED(hResult))
	{
		s_strError = L"Unable to choose a display format!";
		Gbls::pD3D->Release();
		Gbls::pD3D = NULL;
		return false;
	}

	// Check that this backbuffer size is a supported fullscreen resolution too.
	// Get the number of display modes supported by this adapter in this bit depth
	UINT nAdapterModes = Gbls::pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, fmtBackbuffer);

	// Go through each of the modes and get details about it
	bool bResolutionFound = false;
	for(UINT i=0; i<nAdapterModes; ++i)
	{
		// Grab this display mode
		hResult = Gbls::pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT, fmtBackbuffer, i, &mode);
		if(FAILED(hResult))
		{
			// We failed to get this mode. Just continue to the next mode
			continue;
		}

		// Is is a match?
		if(mode.Width == (UINT)sizeBackBuffer.cx && mode.Height == (UINT)sizeBackBuffer.cy)
		{
			bResolutionFound = true;
			break;
		}
	}

	// Did we find the mode?
	if(!bResolutionFound)
	{
		std::wostringstream str;
		str << L"This graphics card / monitor does not support a resolution of " <<
			sizeBackBuffer.cx << L"x" << sizeBackBuffer.cy;
		s_strError = str.str();
		Gbls::pD3D->Release();
		Gbls::pD3D = NULL;
		return false;
	}

	// Get capabilities for this device
	D3DCAPS9 caps;
	hResult = Gbls::pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if(FAILED(hResult))
	{
		s_strError = L"GetDeviceCaps() failed. Error: " + Util::DXErrorToString(hResult);
		Gbls::pD3D->Release();
		Gbls::pD3D = NULL;
		return false;
	}

	// Test for support of some depth buffer formats
	D3DFORMAT fmtDepths[] = {D3DFMT_D32,
		D3DFMT_D24X8, D3DFMT_D24S8, D3DFMT_D24X4S4,
		D3DFMT_D16, D3DFMT_D15S1};
	D3DFORMAT fmtDepthSelected = D3DFMT_UNKNOWN;
	size_t nDepthFormats = sizeof(fmtDepths) / sizeof(fmtDepths[0]);
	for(size_t i=0; i<nDepthFormats; ++i)
	{
		// First, see if this format is supported
		hResult = Gbls::pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			mode.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, fmtDepths[i]);
		if(FAILED(hResult))
		{
			// Graphics card does not support this Z-buffer format
			continue;
		}

		// Then, check that this format can be used with our backbuffer format
		// Some cards may not like e.g. a 32-bit depth buffer with a 16-bit backbuffer.
		hResult = Gbls::pD3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			mode.Format, fmtBackbuffer, fmtDepths[i]);
		if(FAILED(hResult))
		{
			// Graphics card does not support this Z-buffer
			// format being used with this backbuffer format
			continue;
		}

		// This format is usable, take it.
		fmtDepthSelected = fmtDepths[i];
		break;
	}

	// Did we find a suitable format?
	if(fmtDepthSelected == D3DFMT_UNKNOWN)
	{
		s_strError = L"Could not find an appropriate depth buffer format";
		Gbls::pD3D->Release();
		Gbls::pD3D = NULL;
		return false;
	}

	// Test for support of some texture formats
	D3DFORMAT fmtTextures[] = {D3DFMT_DXT1,
		D3DFMT_A8R8G8B8, D3DFMT_X8R8G8B8, D3DFMT_A8B8G8R8, D3DFMT_X8B8G8R8,
		D3DFMT_R8G8B8, D3DFMT_R5G6B5, D3DFMT_X1R5G5B5, D3DFMT_A1R5G5B5,
		D3DFMT_A4R4G4B4, D3DFMT_X4R4G4B4, D3DFMT_A8R3G3B2};
	D3DFORMAT fmtTextureSelected = D3DFMT_UNKNOWN;
	size_t nTextureFormats = sizeof(fmtTextures) / sizeof(fmtTextures[0]);
	for(size_t i=0; i<nTextureFormats; ++i)
	{
		// First, see if this format is supported
		hResult = Gbls::pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			mode.Format, 0, D3DRTYPE_TEXTURE, fmtTextures[i]);
		if(FAILED(hResult))
		{
			// Graphics card does not support this texture format
			continue;
		}

		// This format is usable, take it.
		fmtTextureSelected = fmtTextures[i];
		break;
	}

	// Did we find a suitable format?
	if(fmtTextureSelected == D3DFMT_UNKNOWN)
	{
		s_strError = L"Could not find an appropriate texture format";
		Gbls::pD3D->Release();
		Gbls::pD3D = NULL;
		return false;
	}

	// Fill in present parameters
	Gbls::thePresentParams.BackBufferWidth = sizeBackBuffer.cx;
	Gbls::thePresentParams.BackBufferHeight = sizeBackBuffer.cy;
	Gbls::thePresentParams.BackBufferFormat = fmtBackbuffer;
	Gbls::thePresentParams.BackBufferCount = 1;
	Gbls::thePresentParams.MultiSampleType = D3DMULTISAMPLE_NONE;
	Gbls::thePresentParams.MultiSampleQuality = 0;
	Gbls::thePresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Gbls::thePresentParams.hDeviceWindow = s_hWnd;
	Gbls::thePresentParams.Windowed = TRUE;
	Gbls::thePresentParams.EnableAutoDepthStencil = TRUE;
	Gbls::thePresentParams.AutoDepthStencilFormat = fmtDepthSelected;
	Gbls::thePresentParams.Flags = 0;
	Gbls::thePresentParams.FullScreen_RefreshRateInHz = 0;
	Gbls::thePresentParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// See if hardware vertex processing is available
	DWORD dwFlags;
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		dwFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		dwFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Create the device
	hResult = Gbls::pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, s_hWnd,
		dwFlags, &Gbls::thePresentParams, &Gbls::pd3dDevice);
	if(FAILED(hResult))
	{
		s_strError = L"CreateDevice() failed. Error: " + Util::DXErrorToString(hResult);
		Gbls::pD3D->Release();
		Gbls::pD3D = NULL;
		return false;
	}

	return true;
}

void D3DWindow::ShutdownD3DDevice()
{
	GameResources::releaseResources();

	if(Gbls::pd3dDevice)
		Gbls::pd3dDevice->Release();
	Gbls::pd3dDevice = NULL;

	if(Gbls::pD3D)
		Gbls::pD3D->Release();
	Gbls::pD3D = NULL;

	// Also nullify present params, since they don't mean anything any more.
	memset(&Gbls::thePresentParams, 0, sizeof(Gbls::thePresentParams));
}

//=================================================================================================

HRESULT D3DWindow::SetupState() //currently does nothing
{
	//TODO fix all this nonsense, this function might still be useful though

	return S_OK;
}

//=================================================================================================

void D3DWindow::DrawFrame()
{
	// Clear the screen
	Gbls::pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	// Tell the device we want to start rendering
	HRESULT hResult = Gbls::pd3dDevice->BeginScene();
	if(FAILED(hResult))
	{
		s_strError = L"BeginScene() failed. Error: " + Util::DXErrorToString(hResult);
		return;
	}

	GameResources::drawAll();

	Gbls::pd3dDevice->EndScene();

	// Present
	hResult = Gbls::pd3dDevice->Present(NULL, NULL, NULL, NULL);
	if(FAILED(hResult))
	{
		// Delegate to HandlePresentRetval and see if it knows how to handle it
		hResult = HandlePresentRetval(hResult);
		if(FAILED(hResult))
		{
			s_strError = L"Present() failed. Error: " + Util::DXErrorToString(hResult);
			s_bQuit = true;
			return;
		}
	}
}

//=================================================================================================
//
//void D3DWindow::ToggleFullscreen()
//{
//	// Change the present params windowed flag
//	Gbls::thePresentParams.Windowed = !Gbls::thePresentParams.Windowed;
//
//	// If we're going to fullscreen mode, save the current window position. If not, restore it
//	if(Gbls::thePresentParams.Windowed)
//	{
//		DWORD dwStyle = (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~WS_THICKFRAME;
//		RECT rc;
//		rc.top = rc.left = 0;
//		rc.right = s_nWindowWidth;
//		rc.bottom = s_nWindowHeight;
//		AdjustWindowRect(&rc, dwStyle, FALSE);
//
//		SetWindowPos(m_hWnd, NULL, m_ptWindowPreFullscreen.x, m_ptWindowPreFullscreen.y,
//			rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER);
//	}
//	else
//	{
//		RECT rcWnd;
//		GetWindowRect(m_hWnd, &rcWnd);
//		m_ptWindowPreFullscreen.x = rcWnd.left;
//		m_ptWindowPreFullscreen.y = rcWnd.top;
//	}
//
//	// Change window style to remove border in fullscreen mode
//	DWORD dwStyle;
//	if(Gbls::thePresentParams.Windowed)
//		dwStyle = (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~WS_THICKFRAME;
//	else
//		dwStyle = WS_POPUP;
//	SetWindowLongPtr(m_hWnd, GWL_STYLE, dwStyle);
//
//	// Make sure the changes are flushed
//	SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
//		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
//
//	// Reset the device using the new present params
//	HRESULT hResult = m_pd3dDevice->Reset(&Gbls::thePresentParams);
//	if(FAILED(hResult))
//	{
//		m_strError = L"Reset() failed. Error: " + Util::DXErrorToString(hResult);
//		m_bQuit = true;
//		return;
//	}
//
//	// The device has been reset and lost all of it's state. Set the device state up again
//	SetupState();
//}

//=================================================================================================

HRESULT D3DWindow::HandlePresentRetval(HRESULT hResult)
{
	// If the device is lost, it's not an error. But we're not in a fit state to render either,
	// so we return a success code; S_FALSE.
	if(hResult == D3DERR_DEVICELOST)
	{
		// Call OnLostDevice() the first time we detect a lost device
		if(!s_bDeviceLost)
			OnLostDevice();
		s_bDeviceLost = true;
		return S_FALSE;
	}

	// Is the device needing reset?
	else if(hResult == D3DERR_DEVICENOTRESET)
	{
		// Check if we detected a lost device, and call OnLostDevice() if not
		if(!s_bDeviceLost)
			OnLostDevice();

		// Reset the device
		HRESULT hResultReset = Gbls::pd3dDevice->Reset(&Gbls::thePresentParams);
		if(FAILED(hResultReset))
		{
			// Failed to reset the device, return this as a new error code
			return hResultReset;
		}

		// Reset has succeeded, restore device state and return no error
		SetupState();
		OnResetDevice();
		s_bDeviceLost = false;
		return D3D_OK;
	}

	// Unknown or unhandled error
	return hResult;
}

//=================================================================================================

void D3DWindow::OnLostDevice()
{
}

void D3DWindow::OnResetDevice()
{
}

//=================================================================================================