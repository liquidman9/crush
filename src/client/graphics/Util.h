//=================================================================================================
// Util.h - Utility functions
//=================================================================================================

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

// Global includes
//#include <windows.h>
#include <string>

namespace Util
{
	// Convert a Windows error to a string
	std::wstring Win32ErrorToString(DWORD dwErrorCode);

	// Convert a D3D error to a string
	std::wstring DXErrorToString(HRESULT hResult);
}

#endif // UTIL_H_INCLUDED
