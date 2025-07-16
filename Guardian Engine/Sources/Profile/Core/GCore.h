#ifndef _GE_GCORE_H_
#define _GE_GCORE_H_
#include <Windows.h>
#include <string>
#include <memory>
#include <map>
#include <exception>
#include <fstream>
#include <sstream>
#include <vector>
#include <format>
#include <Uxtheme.h>
#include <dxgi.h>
#include <d3d12.h>
#include <DirectX 12/d3dx12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <random>
#include <map>
#include <functional>
#include <DirectXMath.h>
#include <d3dcompiler.h>
using namespace Microsoft;
using namespace DirectX;

#ifdef GE_PLATFORM_WINDOWS
	
		#ifdef GE_BUILD_DLL
			#define GUARDIAN_API _declspec(dllexport)
		#else
			#define GUARDIAN_API _declspec(dllimport)
		#endif

	#else 
		#error Guardian Engine Only Supports Windows Platform!
#endif

#define null nullptr

#define GUARDIAN_CLEAR_MEMORY(object) ZeroMemory(&object, sizeof(object))
#define GUARDIAN_ARRAYSIZE(array) ARRAYSIZE(array)

#define GUARDIAN_FAILED(hr) FAILED(hr)
#define GUARDIAN_SUCCEEDED(hr) SUCCEEDED(hr)

namespace GE
{

}

#endif