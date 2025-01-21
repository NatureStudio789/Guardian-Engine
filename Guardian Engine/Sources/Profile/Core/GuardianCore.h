#ifndef _GE_GUARDIANCORE_H_
#define _GE_GUARDIANCORE_H_
#include <Windows.h>
#include <string>
#include <exception>
#include <sstream>
#include <memory>
#include <functional>
#include <wrl/client.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>
#include <fstream>
#include <DirectXTex.h>
#include <filesystem>
#include <map>
using namespace Microsoft;
using namespace DirectX;

#ifdef GE_PLATFORM_WINDOWS
	
	#ifdef GE_BUILD_DLL
		#define GUARDIAN_API _declspec(dllexport)
	#else
		#define GUARDIAN_API _declspec(dllimport)
	#endif

	#else 
		#error Guardian Engine only supports Windows platform!

#endif

#define null NULL
#define GFailed FAILED
#define GSucceeded SUCCEEDED

namespace guardian
{
	using GString = std::string;
	using GWideString = std::wstring;
	
	using GWindowHandle = HWND;

	enum GUARDIAN_API GuardianResult
	{
		GE_FAILURE = -1,
		GE_SUCCESS = 0,
	};
}

#endif